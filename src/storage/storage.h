//
// Created by cefour on 12.04.22.
//

#ifndef DATACONTAINER_STORAGE_H
#define DATACONTAINER_STORAGE_H

#include "../api/message_queue.h"
#include "../datastructure/hash_map.h"
#include "../datastructure/time_tree.h"
#include "../exceptions/not_allowed_method_call_excaption.h"
#include "data_types.h"
#include "property.h"
#include "storage_object.h"

#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>



namespace chakra{
    using list_container = std::list<base_storage_object>;
    using key_value_container = ptl::hash_map<index_value_type,std::vector<base_storage_object>>;
    using time_series_container = ptl::time_tree<std::vector<base_storage_object >>;
    using table_container = std::vector<std::vector<base_storage_object >>;
    using header_item = std::pair<std::string,base_storage_object>;
    using header_desc = ptl::hash_map<std::string,std::pair<std::size_t,base_storage_object>>;
    header_desc make_header(const std::vector<header_item> & header);


    using base_catalog_item = std::variant<list_container,key_value_container,time_series_container,table_container>;

    enum CATALOG_ITEM_TYPE{
        CATALOG_LIST,
        CATALOG_KEY_VALUE,
        CATALOG_TIME_SERIE,
        CATALOG_TABLE,
    };
    base_catalog_item make_catalog_item(CATALOG_ITEM_TYPE item);





    struct storage_table{
    private:
        base_catalog_item *tbl;
        header_desc header_description;
        std::size_t header_size;
    private:
        int index;
        message_queue<std::string> queue;

    public:
        void insert_header_description(const header_desc & desc);

    private:
        std::size_t find_column_index(const std::string& name);
    public:
        storage_table() = default;
        storage_table( base_catalog_item * item);

        //list case
        void insert(const base_storage_object & obj);

        void insert(const std::vector<std::pair<std::string, base_storage_object>> & entry);

        std::vector<base_storage_object> find(const index_value_type & key);

        std::vector<base_storage_object> find(const std::chrono::milliseconds & key);

        std::string create_index(int value);
        void aggregate_table(const std::string & column_name, const std::function<void(base_storage_object & item)> & func);

        base_catalog_item *get_inner_table()const;
        std::string get_event();

        void print();

    };

    class storage_manager{
    public:
        storage_manager() = default;
        void add_catalog_entry(const std::string &entry_name,CATALOG_ITEM_TYPE item);
        storage_table  & get_table(const std::string & name);
    private:
        std::map<std::string,base_catalog_item> tbl_catalog;
        std::map<std::string,storage_table> storage_heap;
    };
}




#endif //DATACONTAINER_STORAGE_H
