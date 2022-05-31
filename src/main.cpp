#include <iostream>
#include <algorithm>
#include <map>
#include "storage/storage.h"
#include "typeconfig/type_defintion_file.h"
#include "data_container_service.h"
#include "api/logger.h"



int main(int argc, char ** argv) {
    UNUSED(argc);
    UNUSED(argv);
    logger::logger l;
    l.log(logger::LEVEL::INFO,"PROGRAM START");
    type_defintion_file file(argv[1]);
    auto configs = file.load();
    storage storage;
    for(auto & config : configs) {
        std::shared_ptr<storage_node> table = storage.add_table(config.key);
        for (auto &iter: config.values) {
            table->add_field(iter.key,
                             iter.value->get_type_kind(),
                             iter.value->create_property());
        }
    }

    data_container_service service(configs, storage);
    service.run();


    return 0;
}


