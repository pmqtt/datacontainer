//
// Created by cefour on 12.04.22.
//

#ifndef DATACONTAINER_STORAGE_H
#define DATACONTAINER_STORAGE_H

#include "../api/message_queue.h"
#include "../datastructure/table.h"
#include "../datastructure/time_tree.h"
#include "data_types.h"
#include "property.h"
#include "storage_object.h"

#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>




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

    void print() {
        for(auto [key,value] : datas){
            key.print();
            for ( auto &item: value) {
                item.print();
            }
        }

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
    ptl::table<data_type,std::vector<data_type>> datas;
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



namespace chakra{
    using list_container = std::list<base_storage_object>;
    using key_value_container = ptl::table<base_storage_object,std::vector<base_storage_object>>;
    using time_series_container = ptl::time_tree<std::vector<base_storage_object >>;
    using table_container = std::vector<std::vector<base_storage_object >>;

    using base_catalog_item = std::variant<list_container,key_value_container,time_series_container,table_container>;

    enum CATALOG_ITEM_TYPE{
        CATALOG_LIST,
        CATALOG_KEY_VALUE,
        CATALOG_TIME_SERIE,
        CATALOG_TABLE,
    };
    base_catalog_item make_catalog_item(CATALOG_ITEM_TYPE item);


    using header_desc = ptl::table<std::string,std::pair<std::size_t,base_storage_object>>;
    header_desc make_header(const std::vector<std::pair<std::string,base_storage_object>> & header);





    struct storage_table{
    private:
        base_catalog_item *tbl;
        header_desc header_description;
        std::size_t header_size;

    public:
        void create_header(const header_desc & desc){
            header_description = desc;
            header_size = 0;
            for( auto [x,y] : header_description){
                UNUSED(x);
                UNUSED(y);
                header_size++;
            }
        }
    private:
        std::size_t find_column_index(const std::string& name){
            if(header_description.lookup(name)){
                return header_description[name].first;
            }
            throw std::out_of_range("Column name " + name + " not found in header description" );
        }
    public:
        storage_table( base_catalog_item * item) : tbl(item){ }
        //list case
        void insert(const base_storage_object & obj) {
            if (std::holds_alternative<list_container>(*tbl)) {
                list_container &l = std::get<list_container>(*tbl);
                l.emplace_back(obj);
            } else {
                throw std::runtime_error("method call is not allowed, beacause table is not a list");
            }
        }

        void insert(const std::vector<std::pair<std::string, base_storage_object>> & entry){
            if (std::holds_alternative<key_value_container>(*tbl)) {
                key_value_container & ctbl = std::get<key_value_container>(*tbl);
                auto key = entry[0].second;
                if(ctbl[key].empty()){
                    std::vector<base_storage_object> content;
                    content.resize(header_size);
                    for(const auto & x: entry){
                        content[header_description[x.first].first] =  x.second;
                    }
                    std::get<key_value_container>(*tbl)[key] = content;
                }
            } else {
                throw std::runtime_error("method call is not allowed, beacause table is not a chakra table");
            }
        }

        std::vector<base_storage_object> find(const base_storage_object & key){
            if (std::holds_alternative<key_value_container>(*tbl)) {
                return std::get<key_value_container>(*tbl)[key];
            } else {
                throw std::runtime_error("method call is not allowed, beacause table is not a chakra table");
            }
        }

        std::vector<base_storage_object> find(const std::chrono::milliseconds & key){
            if (std::holds_alternative<time_series_container>(*tbl)) {
                return std::get<time_series_container>(*tbl).find_exact(key).value;
            } else {
                throw std::runtime_error("method call is not allowed, beacause table is not a chakra table");
            }
        }



        base_catalog_item *get_inner_table()const{
            return tbl;
        }

    };

    class storage_manager{
    public:
        storage_manager() = default;
        void add_catalog_entry(const std::string &entry_name,CATALOG_ITEM_TYPE item){
            tbl_catalog[entry_name] = make_catalog_item(item);
        }

        storage_table  get_table(const std::string & name){
            storage_table tbl(&tbl_catalog[name]);
            return tbl;
        }
    private:
        std::map<std::string,base_catalog_item> tbl_catalog;
    };
}




#endif //DATACONTAINER_STORAGE_H
