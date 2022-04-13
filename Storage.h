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


struct StorageField{
    std::string name;
    TYPE_KIND kind;
    std::shared_ptr<FieldProperty>  property;

};

class StorageNode{
public:
    StorageNode() = default;
    void addField(const std::string & name,TYPE_KIND kind,const std::shared_ptr<FieldProperty> & prop){
        StorageField field;
        field.name = name;
        field.kind = kind;
        field.property = prop;
        fields[name] = field;
    }
    void build(){

    }

    void insertRow(const std::vector<std::pair<std::string,DataType>> & row){

        if(row.size() != fields.size()){
            std::cout<<"-4:"<<row.size() << " | " <<fields.size()<<std::endl;
            exit(-4);
        }
        std::pair<std::string,DataType> p;
        std::vector<DataType> rest;
        for(auto & iter : row){

            StorageField field = fields[iter.first];
            if(field.property->is("key")){
                p = iter;
            }else{
                rest.push_back(iter.second);
            }
        }
        datas[p.second] = rest;

    }

    std::vector<DataType> getRow(const DataType & index){
        return datas[index];
    }


private:
    std::map<std::string,StorageField> fields;
    std::map<DataType,std::vector<DataType>,DataTypeComparator> datas;


};

class Storage{
public:
    Storage() = default;
    StorageNode & addTable(const std::string & name){
        if(storage.count(name) == 0){
            return storage[name];
        }else{
            std::cout<<"-3"<<std::endl;
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

void createD01Table(){
    Storage storage;
    StorageNode table = storage.addTable("D01");
    table.addField("UID",TYPE_KIND::STRING,std::make_shared<KeyField>());
    table.addField("DESTINATIONS",TYPE_KIND::STRING,std::make_shared<ListField>(';'));
    table.addField("LOOP",TYPE_KIND::INT,std::make_shared<SimpleField>());
    table.addField("ALGO",TYPE_KIND::STRING,std::make_shared<EnumField>(std::vector<std::string> { "PRIO","ANY" }));

    table.insertRow({
                        {"UID",DataType(new StringType("012345678"),TYPE_KIND::STRING)},
                        {"DESTINATIONS",DataType(new StringType("11511;11211"),TYPE_KIND::STRING)},
                        {"LOOP",DataType(new IntType(1),TYPE_KIND::INT)},
                        {"ALGO",DataType(new StringType("ANY"),TYPE_KIND::STRING)},
                    });

    auto values = table.getRow(DataType(new StringType("012345678"),TYPE_KIND::STRING));


    for(const auto & iter : values){
        iter.print();
        std::cout<<std::endl;
    }


}

#endif //DATACONTAINER_STORAGE_H
