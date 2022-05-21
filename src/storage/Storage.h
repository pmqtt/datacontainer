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
#include "../api/MessageQueue.h"
#include "Property.h"
#include "../datastructure/Table.h"

struct StorageField{
    std::string name;
    TYPE_KIND kind;
    std::shared_ptr<FieldProperty>  property;
    std::size_t positionInData;

};

class StorageNode{
public:
    StorageNode() { index = INT_MAX;}
    void addField(const std::string & name,TYPE_KIND kind,const std::shared_ptr<FieldProperty> & prop);
    void insertRow(const std::vector<std::pair<std::string,DataType>> & row);

    std::vector<DataType> getRow(const DataType & index){
        return datas[index];
    }

    void print(){
        datas.updateAllItems([](const DataType & key, std::vector<DataType> & rest){
            key.print();
            for(auto & item : rest){
               item.print();
           }
        });
    }

    std::string createIndex(int value){
        if(index == INT_MAX){
            index = value;
        }
        std::string val = std::to_string(index);
        index++;
        return val;
    }

    std::string getEvent(){
        return queue.pop();
    }

    void readColumns(const std::string & columnName,const std::function<void(DataType & item)> & func){
        std::size_t pos = fields[columnName].positionInData;
        std::list<std::vector<DataType>> allItems = datas.convertToList();
        for(auto & iter : allItems){
            func(iter[pos]);
        }

    }

private:
    int index;
    std::map<std::string,StorageField> fields;
    Table<DataType,std::vector<DataType>> datas;
    MessageQueue<std::string> queue;
};

class Storage{
public:
    Storage() = default;
    std::shared_ptr<StorageNode> &addTable(const std::string & name){
        if(storage.count(name) == 0){
            storage[name] = std::make_shared<StorageNode>();
            return storage[name];
        }else{
            exit(-3);
        }
    }
    std::shared_ptr<StorageNode> &getTable(const std::string & name){
        return storage[name];
    }

private:
    std::map<std::string,std::shared_ptr<StorageNode>> storage;
};





#endif //DATACONTAINER_STORAGE_H
