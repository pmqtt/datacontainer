//
// Created by cefour on 03.05.22.
//

#ifndef DATACONTAINER_DATACONTAINERSERVICE_H
#define DATACONTAINER_DATACONTAINERSERVICE_H

#include "typeconfig/ConfigurationFile.h"
#include "api/Format.h"
#include "connector/MqttMessenger.h"
#include "api/SmartThread.h"
#include "storage/Storage.h"
#include <map>
#include <tuple>
#include <string>
#include <vector>

class DataContainerService{
private:
    std::vector<YamlTypeNode> config;
    Storage storage;
    std::map<std::string,std::shared_ptr<MqttMessenger<2>>> messengers;
public:
    DataContainerService(const std::vector<YamlTypeNode> & cfg, const Storage & storage) : config(cfg),storage(storage){ }
    void run();
    std::vector<std::pair<std::string,DataType>> createRow(std::map<std::string,std::shared_ptr<FormatType>> formatMap);

};

#endif //DATACONTAINER_DATACONTAINERSERVICE_H
