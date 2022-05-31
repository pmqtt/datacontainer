//
// Created by cefour on 03.05.22.
//

#ifndef DATACONTAINER_DATA_CONTAINER_SERVICE_H
#define DATACONTAINER_DATA_CONTAINER_SERVICE_H

#include "typeconfig/type_defintion_file.h"
#include "api/format.h"
#include "connector/mqtt_messenger.h"
#include "api/smart_thread.h"
#include "storage/storage.h"
#include <map>
#include <tuple>
#include <string>
#include <vector>

class data_container_service{
private:
    std::vector<yaml_type_node> config;
    storage db_container;
    std::map<std::string,std::shared_ptr<mqtt_messenger<2>>> messengers;
public:
    data_container_service(const std::vector<yaml_type_node> & cfg, const storage & store_container) : config(cfg), db_container(store_container){ }
    void run();
    std::vector<std::pair<std::string,data_type>> createRow(std::map<std::string,std::shared_ptr<format_type>> formatMap);

};

#endif //DATACONTAINER_DATA_CONTAINER_SERVICE_H
