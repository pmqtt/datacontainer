#include <iostream>
#include "DataTypes.h"
#include <algorithm>
#include <map>
#include "Storage.h"

/*
 * configure datatype MESSAGE_COUNT
 *  STRING | MESSAGE_TYPE | KEY
 *  INT  |  COUNT
 *
 *
 */




int main() {

    std::map<DataType, std::string,DataTypeComparator> testMap;
    DataType  plz1 ( new StringType("47803"),TYPE_KIND::STRING );
    DataType  plz2 ( new StringType("47804"),TYPE_KIND::STRING );
    DataType  plz3 ( new StringType("47805"),TYPE_KIND::STRING );
    DataType  tel( new IntType(123456),TYPE_KIND::INT);

    testMap[plz1] = "Krefeld1";
    testMap[plz2] = "Krefeld2";
    testMap[plz3] = "Krefeld3";
    testMap[tel] = "Michael";

    std::cout<<"TestMap:"<<testMap[DataType(new StringType("47804"),TYPE_KIND::STRING)]<<std::endl;
    std::cout<<"TestMap:"<<testMap[DataType(new IntType(123456),TYPE_KIND::INT)]<<std::endl;

    createD01Table();


    return 0;
}
