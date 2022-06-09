//
// Created by cefour on 22.04.22.
//

#ifndef DATACONTAINER_TYPE_DEFINTION_FILE_H
#define DATACONTAINER_TYPE_DEFINTION_FILE_H

#include "../exceptions/typedef_exception.h"
#include "../parser/constraint_grammar.h"
#include "../storage/data_types.h"
#include "../storage/storage_object.h"
#include "../server/value_command.h"
#include "yaml-cpp/yaml.h"
#include <string>


template<class T>
void load_node_optional(const std::string &node_name, T* value, const YAML::Node & node){
    *value = node[node_name].as<T>();
}

template<class T>
T load_node(const std::string &node_name, const YAML::Node & node){
    if(!node[node_name].IsDefined()){
        throw typedef_exception("Missing " + node_name + "for correct type definition!");
    }
    return node[node_name].as<T>();
}



enum YAML_VALUE_TYPE{
    Y_LIST,
    Y_STRING,
    Y_INT,
    Y_REAL,
    Y_ENUM,
    Y_INDEX
};


struct yaml_int_type;
struct yaml_real_type;
struct yaml_string_type;
struct yaml_list_type;
struct yaml_enum_type;
struct yaml_index_type;
class field_property;


struct yaml_value_type_base{
    yaml_value_type_base() = default;
    virtual ~yaml_value_type_base() = default;
    virtual std::shared_ptr<field_property> create_property()= 0;
    virtual TYPE_KIND get_type_kind() = 0;
    virtual base_storage_object get_storage_value_type() = 0;
};

struct yaml_int_type : public yaml_value_type_base{
    std::shared_ptr<field_property> create_property()override;
    TYPE_KIND get_type_kind()override;

    base_storage_object get_storage_value_type() override;

};
struct yaml_real_type : public yaml_value_type_base{
    std::shared_ptr<field_property> create_property()override;
    TYPE_KIND get_type_kind()override;

    base_storage_object get_storage_value_type() override;
};
struct yaml_string_type : public yaml_value_type_base{
    std::shared_ptr<field_property> create_property()override;
    TYPE_KIND get_type_kind()override;

    base_storage_object get_storage_value_type() override;
};
struct yaml_list_type : public yaml_value_type_base{
    std::string delimiter;
    YAML_VALUE_TYPE type;
    std::shared_ptr<field_property> create_property()override;
    TYPE_KIND get_type_kind()override;

    base_storage_object get_storage_value_type() override;
};
struct yaml_enum_type : public yaml_value_type_base{
    std::vector<std::string> enum_items;
    std::shared_ptr<field_property> create_property()override;
    TYPE_KIND get_type_kind()override;

    base_storage_object get_storage_value_type() override;
};

struct yaml_index_type : public yaml_value_type_base{
    std::shared_ptr<field_property> create_property()override;
    TYPE_KIND get_type_kind() override;

    base_storage_object get_storage_value_type() override;
};

struct yaml_read_event_node{
    std::string connection_type;
    std::string broker_adr;
    std::string subscribe;
    std::string format_def;
    std::map<std::string,std::optional<std::shared_ptr<value_command>>> insertions;
    std::vector<yaml_read_event_node> nextEvents;
};

struct yaml_send_event_node{
    std::string connection_type;
    std::string broker_adr;
    std::string topic;
    std::vector<ast_node*>  prepare;
    std::string message;
    ast_node* when;
};

struct yaml_values_node{
    std::string key;
    std::shared_ptr<yaml_value_type_base> value;

};

struct yaml_type_node{
    std::string expire_in;
    std::string type_name;
    std::string key;
    std::vector<yaml_values_node> values;
    yaml_read_event_node read_event;
    yaml_send_event_node send_event;

};



class type_defintion_file{
public:
    type_defintion_file(const std::string & fname){
        this->filename = fname;
    }
    std::vector<yaml_type_node> load();

private:
    std::string filename;
};

#endif //DATACONTAINER_TYPE_DEFINTION_FILE_H
