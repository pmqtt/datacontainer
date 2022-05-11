//
// Created by cefour on 12.04.22.
//
#include "DataContainerService.h"


#include <iostream>
#include <regex>
#include <thread>

int indexStartValue(const std::string & item){
    std::size_t pos = item.find(")");
    std::size_t start = item.find("ink_index(");
    std::string value = item.substr(start+10,pos- ( start+10));
    return std::atoi(value.c_str());
}

std::vector<std::pair<std::string,DataType>> DataContainerService::createRow(std::map<std::string,std::shared_ptr<FormatType>> formatMap){
    std::shared_ptr<StorageNode> table = this->storage.getTable(config[0].key);
    std::vector<std::pair<std::string,DataType>> result;
    for ( auto & iter: this->config[0].readEvent.insertions){
        if(iter.second.find("ink_index(") != std::string::npos ){
            std::string index = table->createIndex(indexStartValue(iter.second));
            result.emplace_back(iter.first,DataType(new StringType(index)));
        }else if(iter.second.find("timestamp") != std::string::npos){
            result.emplace_back(iter.first,DataType(new StringType("2022-05-10 10:05:12.122")));
        }else{
            std::smatch sm;
            std::regex x( "\\$[0-9]+");
            std::regex_match (iter.second,sm,x);
            result.emplace_back(iter.first,formatMap[sm[0]]->createDataType());
        }
    }
    return result;
}

void DataContainerService::run(){
    this->messengers[config[0].typeName+"_READ"] = std::make_shared<MqttMessenger<2>>(config[0].readEvent.brokerAdr);
    this->messengers[config[0].typeName+"_READ"]->subscribe(config[0].readEvent.subscribe);

    this->messengers[config[0].typeName+"_SEND"] = std::make_shared<MqttMessenger<2>>(config[0].readEvent.brokerAdr);

    SmartThread<std::thread> s1([this](){
        this->messengers[config[0].typeName+"_READ"]->disconnect();
    },
    [this](){
        auto messenger = this->messengers[config[0].typeName+"_READ"];
        while(1) {
            std::pair<std::string, std::string> message = messenger->getMessage();
            if(message.first != ""){
                std::shared_ptr<StorageNode> table = this->storage.getTable(config[0].key);

                Format format(config[0].readEvent.format);

                if(format.interpret(message.second)){
                    auto arguments = format.getArgumentsMap();
                    auto row = createRow(arguments);
                    table->insertRow(row);
                }else{
                    std::cout<<"Wrong format in message"<<std::endl;
                }
                std::cout<<"===================================================================================="<<std::endl;
                table->print();
                std::cout<<"==================================================================================="<<std::endl;
            }
        }
    });

    SmartThread<std::thread> s2([this](){
        this->messengers[config[0].typeName+"_SEND"]->disconnect();
    },
    [this](){
        auto messenger = this->messengers[config[0].typeName+"_SEND"];
        std::shared_ptr<StorageNode> table = this->storage.getTable(config[0].key);
        while(1) {
            std::string event =  table->getEvent();
            std::string cmdRaw = config[0].sendEvent.prepare;
            if(cmdRaw.find("mean(") != std::string::npos){
                std::size_t pos = cmdRaw.find("mean(") + 5;
                std::size_t posEnd = cmdRaw.find(")");

                std::string columnName = cmdRaw.substr(pos, posEnd - pos);
                double result = 0;
                int count = 0;
                table->readColumns(columnName,[&](DataType & type){
                    result += type.as<double>();
                    count++;
                });
                double mean = result / count;

                messenger->publish(config[0].sendEvent.topic,std::to_string(mean));
            }else{
                messenger->publish(config[0].sendEvent.topic,event);
            }


        }
    });

    s1.get().join();
    s2.get().join();

}