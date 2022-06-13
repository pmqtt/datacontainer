//
// Created by cefour on 12.06.22.
//

#include "../storage/storage.h"
#include "../typeconfig/type_defintion_file.h"
#include "../server/data_container_service.h"
#include "../api/logger.h"

#include "service.h"


void service::run(int argc,char **argv){
    logger::logger l;
    if(argc < 2){
        l.log(logger::LEVEL::ERROR,"missing configuration file");
        exit(-1);
    }
    type_defintion_file file(argv[1]);
    auto configs = file.load();
    chakra::storage_manager mgr;
    for(auto & config : configs) {
        mgr.add_catalog_entry(config.key,config.type);
        auto & tbl = mgr.get_table(config.key);
        std::vector<chakra::header_item> header = config.create_header();
        tbl.insert_header_description(chakra::make_header(header));
    }
    data_container_service chakra_service(configs, mgr);
    chakra_service.run();
}


