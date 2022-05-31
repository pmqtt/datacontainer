//
// Created by cefour on 12.04.22.
//

#ifndef DATACONTAINER_STORAGE_H
#define DATACONTAINER_STORAGE_H

#include <map>
#include <string>
#include <vector>
#include <memory>
#include "data_types.h"
#include "../api/message_queue.h"
#include "property.h"
#include "../datastructure/table.h"

struct storage_field{
    std::string name;
    TYPE_KIND kind;
    std::shared_ptr<field_property>  property;
    std::size_t position_in_data;

};

class storage_node{
public:
    storage_node() { index = INT_MAX;}
    void add_field(const std::string & name, TYPE_KIND kind, const std::shared_ptr<field_property> & prop);
    void insert_row(const std::vector<std::pair<std::string,data_type>> & row);

    std::vector<data_type> get_row(const data_type & index){
        return datas[index];
    }

    void print(){
        datas.update_all_items([](const data_type &key, std::vector<data_type> &rest) {
            key.print();
            for (auto &item: rest) {
                item.print();
            }
        });
    }

    std::string create_index(int value){
        if(index == INT_MAX){
            index = value;
        }
        std::string val = std::to_string(index);
        index++;
        return val;
    }

    std::string get_event(){
        return queue.pop();
    }

    void read_columns(const std::string & columnName, const std::function<void(data_type & item)> & func){
        std::size_t pos = fields[columnName].position_in_data;
        std::list<std::vector<data_type>> allItems = datas.convert_to_list();
        for(auto & iter : allItems){
            func(iter[pos]);
        }

    }

private:
    int index;
    std::map<std::string,storage_field> fields;
    table<data_type,std::vector<data_type>> datas;
    message_queue<std::string> queue;
};

class storage{
public:
    storage() = default;
    std::shared_ptr<storage_node> &add_table(const std::string & name){
        if(container.count(name) == 0){
            container[name] = std::make_shared<storage_node>();
            return container[name];
        }else{
            exit(-3);
        }
    }
    std::shared_ptr<storage_node> &get_table(const std::string & name){
        return container[name];
    }

private:
    std::map<std::string,std::shared_ptr<storage_node>> container;
};





#endif //DATACONTAINER_STORAGE_H
