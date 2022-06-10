//
// Created by cefour on 03.05.22.
//

#ifndef DATACONTAINER_DATA_CONTAINER_SERVICE_H
#define DATACONTAINER_DATA_CONTAINER_SERVICE_H

#include "../api/format.h"
#include "../api/smart_thread.h"
#include "../connector/mqtt_messenger.h"
#include "../storage/storage.h"
#include "../typeconfig/type_defintion_file.h"

#include <map>
#include <tuple>
#include <string>
#include <vector>

class data_container_service{
private:
    std::vector<yaml_type_node> config;
    chakra::storage_manager & db_manager;
    std::map<std::string,std::shared_ptr<mqtt_messenger<2>>> messengers;
public:
    data_container_service(const std::vector<yaml_type_node> & cfg,  chakra::storage_manager & mgr) : config(cfg), db_manager(mgr){ }
    void run();
    std::vector<std::pair<std::string,base_storage_object>> createRow(chakra::storage_table & tbl,const std::string &msg);
    std::vector<std::shared_ptr<smart_thread<std::thread>>> send_threads;
    std::vector<std::shared_ptr<smart_thread<std::thread>>> received_threads;

};

#endif //DATACONTAINER_DATA_CONTAINER_SERVICE_H
