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

TYPE_KIND convertYamlTypeToTypeKind(YAML_VALUE_TYPE  type){
    switch(type){
        case YAML_VALUE_TYPE::Y_INT : return TYPE_KIND::INT;
        case YAML_VALUE_TYPE::Y_REAL : return TYPE_KIND::REAL;
        case YAML_VALUE_TYPE::Y_STRING : return TYPE_KIND::STRING;
    }
}


int main(int argc, char ** argv) {
#if 0
    std::map<DataType, std::string,DataTypeComparator> testMap;
    DataType  plz1 ( new StringType("47803"),TYPE_KIND::STRING );
    DataType  plz2 ( new StringType("47804"),TYPE_KIND::STRING );
    DataType  plz3 ( new StringType("47805"),TYPE_KIND::STRING );
    DataType  tel( new IntType(123456),TYPE_KIND::INT);



    std::cout<<std::hash<DataType>{}(plz1);
    testMap[plz1] = "Krefeld1";
    testMap[plz2] = "Krefeld2";
    testMap[plz3] = "Krefeld3";
    testMap[tel] = "Michael";

    std::cout<<"TestMap:"<<testMap[DataType(new StringType("47804"),TYPE_KIND::STRING)]<<std::endl;
    std::cout<<"TestMap:"<<testMap[DataType(new IntType(123456),TYPE_KIND::INT)]<<std::endl;

    createD01Table();

    if(plz1 == DataType(new StringType("47803"),TYPE_KIND::STRING)){
        std::cout<<" REIGHT " << std::endl;
    }
#endif
    ConfigurationFile file(argv[1]);
    YamlTypeNode config = file.load();

    Storage storage;
    StorageNode table = storage.addTable(config.key);

    std::vector<YamlValuesNode> values = config.values;
    for(int i = 0; i < values.size(); i++) {
        if ( i == 0) {
            TYPE_KIND kind = convertYamlTypeToTypeKind(values[i].value.type);
            auto property = std::make_shared<KeyField>();
            table.addField(values[i].key, kind, property);
        }else{
            if(values[i].value.type == YAML_VALUE_TYPE::Y_ENUM || values[i].value.type == YAML_VALUE_TYPE::Y_LIST){
                TYPE_KIND kind = TYPE_KIND::STRING;
                std::shared_ptr<FieldProperty> property = nullptr;
                if(values[i].value.type == YAML_VALUE_TYPE::Y_ENUM){
                    YamlEnumType* enumDef = dynamic_cast<YamlEnumType*>(values[i].value.value.get());
                    property = std::make_shared<EnumField>(enumDef->enumItems);
                }else{
                    YamlListType* listDef = dynamic_cast<YamlListType*>(values[i].value.value.get());
                    property = std::make_shared<ListField>(listDef->delimiter);
                }
                table.addField(values[i].key, kind, property);

            }else{
                TYPE_KIND kind = convertYamlTypeToTypeKind(values[i].value.type);
                auto property = std::make_shared<SimpleField>();
                table.addField(values[i].key, kind, property);
            }
        }
    }
    table.insertRow({
                            {"UID",DataType(new StringType("012345678"),TYPE_KIND::STRING)},
                            {"DESTINATIONS",DataType(new StringType("11511;11211"),TYPE_KIND::STRING)},
                            {"LOOP",DataType(new IntType(1),TYPE_KIND::INT)},
                            {"ALGO",DataType(new StringType("ANY"),TYPE_KIND::STRING)},
                    });

    auto tableValues = table.getRow(DataType(new StringType("012345678"),TYPE_KIND::STRING));


    for(const auto & iter : tableValues){
        iter.print();
        std::cout<<std::endl;
    }



    return 0;
}
