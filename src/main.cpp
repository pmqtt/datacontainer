#include <iostream>
#include <algorithm>
#include <map>
#include "storage/Storage.h"
#include "ConfigurationFile.h"
#include "DataContainerService.h"




int main(int argc, char ** argv) {

    ConfigurationFile file(argv[1]);
    auto configs = file.load();
    Storage storage;
    for(auto & config : configs) {
        std::shared_ptr<StorageNode> table = storage.addTable(config.key);
        for (auto &iter: config.values) {
            table->addField(iter.key,
                           iter.value->getTypeKind(),
                           iter.value->createProperty());
        }
    }

    DataContainerService service(configs,storage);
    service.run();


    return 0;
}


