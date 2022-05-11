//
// Created by cefour on 03.05.22.
//

#ifndef DATACONTAINER_DATACONTAINERSERVICE_H
#define DATACONTAINER_DATACONTAINERSERVICE_H

#include "ConfigurationFile.h"
#include "Format.h"
#include "MqttMessenger.h"
#include "SmartThread.h"
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
