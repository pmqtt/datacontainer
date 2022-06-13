//
// Created by cefour on 12.04.22.
//
#include "data_container_service.h"
#include "../storage/table_trigger.h"


#include <iostream>
#include <regex>
#include <thread>
#include "../api/date_time.h"
#include "value_command.h"


std::vector<std::pair<std::string, base_storage_object>>
data_container_service::createRow(chakra::storage_table & tbl,const std::string &msg) {
    std::vector<std::pair<std::string, base_storage_object>> result;
    for (auto &iter: this->config[0].read_event.insertions) {
        if (iter.second) {
            result.emplace_back(iter.first, (*(iter.second))->execute(tbl, msg));
        }
    }
    return result;
}

void data_container_service::run() {
    try {
        this->messengers[config[0].type_name + "_READ"] = std::make_shared<mqtt_messenger<2>>(
                config[0].read_event.broker_adr);
        this->messengers[config[0].type_name + "_READ"]->subscribe(config[0].read_event.subscribe);

        this->messengers[config[0].type_name + "_SEND"] = std::make_shared<mqtt_messenger<2>>(
                config[0].read_event.broker_adr);

        send_threads.push_back(std::make_shared<smart_thread<std::thread>>(
                [this]() {
                    this->messengers[config[0].type_name + "_READ"]->disconnect();
                    auto &tbl = this->db_manager.get_table(config[0].key);
                    tbl.disable_queue();
                },
                [this]() {
                    auto messenger = this->messengers[
                            config[0].type_name + "_READ"];
                    try {
                        while (1) {
                            std::pair<std::string, std::string> message = messenger->getMessage();
                            if (message.first != "") {
                                auto &tbl = this->db_manager.get_table(config[0].key);
                                auto row = createRow(tbl, message.second);
                                tbl.insert(row);
                                tbl.print();

                            }
                        }
                    } catch (mqtt_disconnect_exception &e) {
                        std::cout << "go out" << std::endl;
                    }
                }));

        received_threads.push_back(std::make_shared<smart_thread<std::thread>>(
                [this]() {
                    this->messengers[config[0].type_name + "_SEND"]->disconnect();
                    auto &tbl = this->db_manager.get_table(config[0].key);
                    tbl.disable_queue();
                },
                [this]() {
                    auto messenger = this->messengers[config[0].type_name + "_SEND"];
                    auto &tbl = this->db_manager.get_table(config[0].key);
                    auto condition = config[0].send_event.when;
                    auto preparation = config[0].send_event.prepare;
                    tbl.create_trigger(condition, preparation);
                    try {
                        while (1) {
                            auto trigger = tbl.get_event();
                            auto message = trigger->get_message(config[0].send_event.message);
                            messenger->publish(config[0].send_event.topic, message);

                        }
                    } catch (message_queue_deactivate_exception &e) {
                        std::cout << "go out" << std::endl;
                    }
                }));
        received_threads[0]->get().join();
        send_threads[0]->get().join();
    }catch(mqtt::exception & e){
        std::cout<<"mqtt connection to broker "<<config[0].read_event.broker_adr << "not established: "<<e.what()<<std::endl;
    }

}