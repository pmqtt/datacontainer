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
    file.load();
    return 0;
}
