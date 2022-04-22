//
// Created by cefour on 12.04.22.
//

#ifndef DATACONTAINER_STORAGE_H
#define DATACONTAINER_STORAGE_H

#include <map>
#include <string>
#include <vector>
#include <memory>
#include "DataTypes.h"
#include "Property.h"
#include "Table.h"

struct StorageField{
    std::string name;
    TYPE_KIND kind;
    std::shared_ptr<FieldProperty>  property;

};

class StorageNode{
public:
    StorageNode() = default;
    void addField(const std::string & name,TYPE_KIND kind,const std::shared_ptr<FieldProperty> & prop);
    void insertRow(const std::vector<std::pair<std::string,DataType>> & row);

    std::vector<DataType> getRow(const DataType & index){
        return datas[index];
    }


private:
    std::map<std::string,StorageField> fields;
    //std::map<DataType,std::vector<DataType>,DataTypeComparator> datas;
    Table<DataType,std::vector<DataType>> datas;
};

class Storage{
public:
    Storage() = default;
    StorageNode & addTable(const std::string & name){
        if(storage.count(name) == 0){
            return storage[name];
        }else{
            exit(-3);
        }
    }

private:
    std::map<std::string,StorageNode> storage;
};


/*
 * D01
 *  UID|DESTINATIONS|LOOP|ALGO
 *
 * */

void createD01Table();

#endif //DATACONTAINER_STORAGE_H
