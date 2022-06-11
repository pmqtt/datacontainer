#include <iostream>
#include <algorithm>
#include <map>
#include "storage/storage.h"
#include "typeconfig/type_defintion_file.h"
#include "server/data_container_service.h"
#include "api/logger.h"

std::vector<chakra::header_item> create_header_from_config(yaml_type_node &config);
#if 0
void old_stuff(int argc, char **argv){
    UNUSED(argc);
    UNUSED(argv);
    logger::logger l;
    l.log(logger::LEVEL::INFO,"PROGRAM START");
    type_defintion_file file(argv[1]);
    auto configs = file.load();
    storage storage;
    for(auto & config : configs) {
        std::shared_ptr<storage_node> hash_map = storage.add_table(config.key);
        for (auto &iter: config.values) {
            hash_map->add_field(iter.key,
                             iter.value->get_type_kind(),
                             iter.value->create_property());
        }
    }

    data_container_service service(configs, storage);
    service.run();
}
#endif

std::vector<chakra::header_item> create_header_from_config(yaml_type_node &config) {
    std::vector<chakra::header_item> header;
    for (auto &iter: config.values) {
        header.push_back(chakra::header_item(iter.key,iter.value->get_storage_value_type()));
    }
    return header;
}


int main(int argc, char ** argv) {
    logger::logger l;
    if(argc < 2){
        l.log(logger::LEVEL::ERROR,"missing configuration file");
        return -1;
    }
    type_defintion_file file(argv[1]);
    auto configs = file.load();
    chakra::storage_manager mgr;
    for(auto & config : configs) {
        mgr.add_catalog_entry(config.key,config.type);
        auto & tbl = mgr.get_table(config.key);
        std::vector<chakra::header_item> header = create_header_from_config(config);
        tbl.insert_header_description(chakra::make_header(header));
    }
    data_container_service service(configs, mgr);
    service.run();


    return 0;
}



