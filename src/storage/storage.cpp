
#include "storage.h"
#include "table_trigger.h"
#include "../api/variant.h"
#include "../exceptions/storage_node_insertion_exception.h"



#define INSERT_TO_KEY_CONTAINER(FUNCTION) \
    insert_to_key_container_kind(entry,[&](auto & key,auto & content){ \
        FUNCTION;\
    })




chakra::base_catalog_item chakra::make_catalog_item(chakra::CATALOG_ITEM_TYPE item) {
    switch (item) {
        case CATALOG_ITEM_TYPE::CATALOG_LIST: {
            base_catalog_item i = chakra::list_container{};
            return i;
        }
        case CATALOG_ITEM_TYPE::CATALOG_KEY_VALUE: {
            base_catalog_item i = chakra::key_value_container{};
            return i;
        }
        case CATALOG_ITEM_TYPE::CATALOG_TIME_SERIE: {
            base_catalog_item i = chakra::time_series_container{};
            return i;
        }
        case CATALOG_ITEM_TYPE::CATALOG_TABLE: {
            base_catalog_item i = chakra::table_container{};
            return i;
        }
        case CATALOG_ITEM_TYPE::CATALOG_KEY_TABLE:{
            base_catalog_item i = chakra::key_table_container{};
            return i;
        }
        default:
            break;
    }
    base_catalog_item i = chakra::key_value_container{};
    return i;
}

chakra::header_desc chakra::make_header(const std::vector<std::pair<std::string, base_storage_object>> &header) {
    chakra::header_desc description;
    for (std::size_t i = 0; i < header.size(); ++i) {
        description[header[i].first] = std::pair<std::size_t, base_storage_object>(i, header[i].second);
    }
    return description;
}

void chakra::storage_manager::add_catalog_entry(const std::string &entry_name, CATALOG_ITEM_TYPE item) {
    tbl_catalog[entry_name] = make_catalog_item(item);
}

chakra::storage_table &chakra::storage_manager::get_table(const std::string &name) {
    if (storage_heap.count(name) == 0) {
        storage_table tbl(&tbl_catalog[name]);
        storage_heap[name] = tbl;
    }
    return storage_heap[name];
}

void chakra::storage_table::insert_header_description(const chakra::header_desc &desc) {
    header_description = desc;
    header_size = 0;
    for (auto [x, y]: header_description) {
        UNUSED(x);
        UNUSED(y);
        header_size++;
    }

}

std::size_t chakra::storage_table::find_column_index(const std::string &name) {
    if (header_description.lookup(name)) {
        return header_description[name].first;
    }
    throw std::out_of_range("Column name " + name + " not found in header description");
}

chakra::storage_table::storage_table(){}

chakra::storage_table::storage_table(base_catalog_item *item)
        : tbl(item) {
    index = 0;
}


//list case
void chakra::storage_table::insert(const base_storage_object &obj) {
    if (std::holds_alternative<list_container>(*tbl)) {
        list_container &l = std::get<list_container>(*tbl);
        l.emplace_back(obj);
    } else {
        throw std::runtime_error("method call is not allowed, beacause hash_map is not a list");
    }
}


void chakra::storage_table::insert(const std::vector<std::pair<std::string, base_storage_object>> &entry) {
    if(std::holds_alternative<key_value_container>(*tbl)){
        INSERT_TO_KEY_CONTAINER(std::get<key_value_container>(*tbl)[key] = content);
    }else if (std::holds_alternative<key_table_container>(*tbl)){
        INSERT_TO_KEY_CONTAINER(std::get<key_table_container>(*tbl)[key].push_back(content));
    }else{
        throw not_allowed_method_call_excaption("method call is not allowed, beacause hash_map is not a chakra hash_map");
    }

}

std::vector<base_storage_object>
chakra::storage_table::create_row_content(const std::vector<std::pair<std::string, base_storage_object>> &entry) {
    std::vector<base_storage_object> content;
    content.resize(header_size);
    for (const auto &x: entry) {
        content[header_description[x.first].first] = x.second;
    }
    return content;
}

void chakra::storage_table::notify_trigger() {
    for(auto & trigger_item: trigger_container){
        if(trigger_item->trigger(*this)){
            queue.push(trigger_item);
        }
    }
}

std::vector<base_storage_object> chakra::storage_table::find(const index_value_type &key) {
    if (std::holds_alternative<key_value_container>(*tbl)) {
        return std::get<key_value_container>(*tbl)[key];
    } else {
        throw not_allowed_method_call_excaption("method call is not allowed, beacause hash_map is not a chakra hash_map");
    }
}

std::vector<base_storage_object> chakra::storage_table::find(const std::chrono::milliseconds &key) {
    if (std::holds_alternative<time_series_container>(*tbl)) {
        return std::get<time_series_container>(*tbl).find_exact(key).value;
    } else {
        throw not_allowed_method_call_excaption("method call is not allowed, beacause hash_map is not a chakra hash_map");
    }
}

std::string chakra::storage_table::create_index(int value) {
    if (index == INT_MAX) {
        index = value;
    }
    std::string val = std::to_string(index);
    index++;
    return val;
}

void chakra::storage_table::aggregate_table(const std::string &column_name,
                                            const std::function<void(base_storage_object &item)> &func) {
    auto key_value_store = std::get<key_value_container>(*tbl);
    std::size_t pos = find_column_index(column_name);
    for( auto iter : key_value_store){
        auto &x = iter.second[pos];
        func(x);
    }

    /*
    std::list<std::vector<base_storage_object>> allItems = key_value_store.convert_to_list();
    for (auto &iter: allItems) {
        func(iter[pos]);
    }*/
}

chakra::base_catalog_item *chakra::storage_table::get_inner_table() const {
    return tbl;
}

std::shared_ptr<table_trigger> chakra::storage_table::get_event() {
    return queue.pop();
}

void chakra::storage_table::print() {


    auto header_as_list = header_description.convert_to_list();
    std::vector<std::string> header_keys(header_as_list.size());

    auto iter = header_as_list.begin();
    for(auto [key,value] : header_description ){
        header_keys[iter->first] = key;
        ++iter;
    }
    std::cout<< "-----------------------------------------------------------------------------------"<< std::endl;
    if(std::holds_alternative<key_value_container>(*tbl)) {
        for (auto key: header_keys) {
            std::cout << key << " | ";
        }
        std::cout << "\n=================================================================================="<< std::endl;

        auto key_value_store = std::get<key_value_container>(*tbl);
        for (auto [key, value]: key_value_store) {
            for (auto &item: value) {
                std::visit([](auto &i) { std::cout << i.get_value() << " | "; }, item);
            }
            std::cout << "\n";
        }
    }else if (std::holds_alternative<key_table_container>(*tbl)){
        auto key_table_store = std::get<key_table_container>(*tbl);
        for (auto [key, value]: key_table_store) {
           std::cout<<"Primary Key |";
            for (auto k: header_keys) {
                std::cout << k << " | ";
            }
            std::cout << "\n=================================================================================="<< std::endl;
            std::visit([](auto &k){ std::cout<<k.get_value() << " | "; },key);

            for (auto &row: value) {
                for(auto & col: row) {
                    std::visit([](auto &i) { std::cout << i.get_value() << " | "; }, col);
                }
                std::cout << "\n\t";
            }
        }
    }

    std::cout
            << "-----------------------------------------------------------------------------------"
            << std::endl;
}

void chakra::storage_table::create_trigger(const ASTNodePtr & condition,const std::vector<ASTNodePtr> & preparation){
    auto trigger = std::make_shared<table_trigger>(condition, preparation);
    trigger_container.push_back(trigger);
}

chakra::storage_table::~storage_table(){
    queue.deactivate();
}

void chakra::storage_table::disable_queue(){
    queue.deactivate();
}