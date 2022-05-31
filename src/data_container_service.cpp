//
// Created by cefour on 12.04.22.
//
#include "data_container_service.h"


#include <iostream>
#include <regex>
#include <thread>

int indexStartValue(const std::string & item){
    std::size_t pos = item.find(")");
    std::size_t start = item.find("ink_index(");
    std::string value = item.substr(start+10,pos- ( start+10));
    return std::atoi(value.c_str());
}

std::vector<std::pair<std::string,data_type>> data_container_service::createRow(std::map<std::string,std::shared_ptr<format_type>> formatMap){
    std::shared_ptr<storage_node> table = this->db_container.get_table(config[0].key);
    std::vector<std::pair<std::string,data_type>> result;
    for ( auto & iter: this->config[0].read_event.insertions){
        if(iter.second.find("ink_index(") != std::string::npos ){
            std::string index = table->create_index(indexStartValue(iter.second));
            result.emplace_back(iter.first, data_type(new string_type(index)));
        }else if(iter.second.find("timestamp") != std::string::npos){
            result.emplace_back(iter.first, data_type(new string_type("2022-05-10 10:05:12.122")));
        }else{
            std::smatch sm;
            std::regex x( "\\$[0-9]+");
            std::regex_match (iter.second,sm,x);
            result.emplace_back(iter.first, formatMap[sm[0]]->create_data_type());
        }
    }
    return result;
}

void data_container_service::run(){
    this->messengers[config[0].type_name + "_READ"] = std::make_shared<mqtt_messenger<2>>(config[0].read_event.broker_adr);
    this->messengers[config[0].type_name + "_READ"]->subscribe(config[0].read_event.subscribe);

    this->messengers[config[0].type_name + "_SEND"] = std::make_shared<mqtt_messenger<2>>(config[0].read_event.broker_adr);

    smart_thread<std::thread> s1([this](){
        this->messengers[config[0].type_name + "_READ"]->disconnect();
    },
                                 [this](){
        auto messenger = this->messengers[config[0].type_name + "_READ"];
        while(1) {
            std::pair<std::string, std::string> message = messenger->getMessage();
            if(message.first != ""){
                std::shared_ptr<storage_node> tbl = this->db_container.get_table(config[0].key);

                format fmt(config[0].read_event.format_def);

                if(fmt.interpret(message.second)){
                    auto arguments = fmt.get_arguments_map();
                    auto row = createRow(arguments);
                    tbl->insert_row(row);
                }else{
                    std::cout<<"Wrong format_def in message"<<std::endl;
                }
                std::cout<<"===================================================================================="<<std::endl;
                tbl->print();
                std::cout<<"==================================================================================="<<std::endl;
            }
        }
    });

    smart_thread<std::thread> s2([this](){
        this->messengers[config[0].type_name + "_SEND"]->disconnect();
    },
                                 [this](){
        auto messenger = this->messengers[config[0].type_name + "_SEND"];
        std::shared_ptr<storage_node> tbl = this->db_container.get_table(config[0].key);
        while(1) {
            std::string event = tbl->get_event();
            std::string cmd_raw = config[0].send_event.prepare;
            if(cmd_raw.find("mean(") != std::string::npos){
                std::size_t pos = cmd_raw.find("mean(") + 5;
                std::size_t posEnd = cmd_raw.find(")");

                std::string column_name = cmd_raw.substr(pos, posEnd - pos);
                double result = 0;
                int count = 0;
                tbl->read_columns(column_name, [&](data_type &type) {
                    result += type.as<double>();
                    count++;
                });
                double mean = result / count;

                messenger->publish(config[0].send_event.topic, std::to_string(mean));
            }else{
                messenger->publish(config[0].send_event.topic, event);
            }


        }
    });

    s1.get().join();
    s2.get().join();

}