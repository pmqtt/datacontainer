#include <iostream>
#include <algorithm>
#include <map>
#include "Storage.h"
#include "ConfigurationFile.h"

/*
 * configure datatype MESSAGE_COUNT
 *  STRING | MESSAGE_TYPE | KEY
 *  INT  |  COUNT
 *
 *
 */


int main(int argc, char ** argv) {

    ConfigurationFile file(argv[1]);
    auto configs = file.load();
    Storage storage;
    for(auto & config : configs) {

        StorageNode table = storage.addTable(config.key);
        for (auto &iter: config.values) {
            table.addField(iter.key,
                           iter.value->getTypeKind(),
                           iter.value->createProperty());
        }
        table.insertRow({
                                {"UID",          DataType(new StringType("012345678"), TYPE_KIND::STRING)},
                                {"DESTINATIONS", DataType(new StringType("11511;11211"), TYPE_KIND::STRING)},
                                {"LOOP",         DataType(new IntType(1), TYPE_KIND::INT)},
                                {"ALGO",         DataType(new StringType("ANY"), TYPE_KIND::STRING)},
                        });

        auto tableValues = table.getRow(DataType(new StringType("012345678"), TYPE_KIND::STRING));


        for (const auto &iter: tableValues) {
            iter.print();
            std::cout << std::endl;
        }
    }


    return 0;
}
