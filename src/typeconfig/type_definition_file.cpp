//
// Created by cefour on 22.04.22.
//

#include "../api/macro_helper.h"
#include "../exceptions/typedef_exception.h"
#include "../storage/property.h"
#include "type_defintion_file.h"

#include <vector>



namespace YAML {
    template<>
    struct convert<yaml_read_event_node> {
        static Node encode(const yaml_read_event_node& rhs) {
            UNUSED(rhs);
            Node node;
            return node;
        }

        static bool decode(const Node& node, yaml_read_event_node& rhs) {
            rhs.connection_type = node["type"].as<std::string>();
            rhs.broker_adr = node["broker"].as<std::string>();
            rhs.subscribe = node["subscribe"].as<std::string>();
            rhs.format_def = node["format"].as<std::string>();
            std::remove(rhs.format_def.begin(), rhs.format_def.end(), '\"');

            for(std::size_t i = 0; i < node["insert"].size(); ++i){
                for(const auto & iter : node["insert"][i]){
                    rhs.insertions[iter.first.as<std::string>()] = make_value_command(iter.second.as<std::string>(),rhs.format_def);
                }
            }
            if(node["read_event"].IsDefined()) {
                rhs.nextEvents.push_back(node["read_event"].as<yaml_read_event_node>());
            }
            return true;
        }
    };

    template<>
    struct convert<yaml_send_event_node> {
        static Node encode(const yaml_send_event_node& rhs) {
            UNUSED(rhs);
            Node node;
            return node;
        }

        static bool decode(const Node& node, yaml_send_event_node& rhs) {
/*
 *   send_event:
    - type: mqtt
    - broker: localhost:1883
    - topic: CALCULATED
    - prepare:
        - mean(TEMPERATURE)
        - mean(TEMPERATURE) +10
    - message = $1 $2
    - when: count(TEMPERATURE) >= 3
 */


            rhs.connection_type = node["type"].as<std::string>();
            rhs.broker_adr = node["broker"].as<std::string>();
            rhs.topic = node["topic"].as<std::string>();
            rhs.message = node["message"].as<std::string>();
            auto prepare_node = node["prepare"];
            for(const auto & item : prepare_node) {
                ASTNodePtr prepare_cmd;
                std::string input = item.as<std::string>();
                parse_grammar(input, constraint_grammar(), qi::space, prepare_cmd);
                rhs.prepare.push_back(prepare_cmd) ;
            }
            ASTNodePtr when_cmd;
            parse_grammar(node["when"].as<std::string>(), constraint_grammar(), qi::space, when_cmd);
            rhs.when = when_cmd;

            return true;
        }
    };

    template<>
    struct convert<yaml_list_type>{
        static Node encode(const yaml_list_type& rhs) {
            UNUSED(rhs);
            Node node;
            return node;
        }
        static bool decode(const Node& list_entry, yaml_list_type& list) {
            static std::map<std::string,YAML_VALUE_TYPE> str_to_type;
            str_to_type["string"] = YAML_VALUE_TYPE::Y_STRING;
            str_to_type["int"] = YAML_VALUE_TYPE::Y_INT;
            str_to_type["real"] = YAML_VALUE_TYPE::Y_REAL;

            std::string delimiter = list_entry["delimiter"].as<std::string>();
            std::string type = list_entry["type"].as<std::string>();
            list.delimiter = delimiter;
            list.type = str_to_type[type];
            return true;
        }
    };

    template<>
    struct convert<yaml_enum_type>{
        static Node encode(const yaml_enum_type& rhs) {
            UNUSED(rhs);
            Node node;
            return node;
        }
        static bool decode(const Node& enum_entry, yaml_enum_type& e) {
            std::vector<std::string> enum_items;
            for (const auto &enumerations: enum_entry) {
                enum_items.push_back(enumerations.Scalar());
            }
            e.enum_items = enum_items;
            return true;
        }
    };


    template<>
    struct convert<yaml_values_node>{
        static Node encode(const yaml_values_node& rhs) {
            UNUSED(rhs);
            Node node;
            return node;
        }

        static bool decode(const Node& values, yaml_values_node& values_node) {
            if(YAML::NodeType::Map == values.Type()){
                for (const auto &value_defs: values) {
                    values_node.key = value_defs.first.as<std::string>();
                    if (value_defs.second.IsScalar()) {
                        std::string valueType = value_defs.second.as<std::string>();
                        if (valueType == "string") {
                            values_node.value = std::make_shared<yaml_string_type>();
                        }
                        if (valueType == "int") {
                            values_node.value = std::make_shared<yaml_int_type>();
                        }
                        if (valueType == "real") {
                            values_node.value = std::make_shared<yaml_real_type>();
                        }
                        if (valueType == "index") {
                            values_node.value = std::make_shared<yaml_index_type>();
                        }
                    } else {
                        for (const auto &value_def: value_defs.second) {
                            std::string valueType = value_def.first.as<std::string>();
                            if (valueType == "list") {
                                std::shared_ptr<yaml_list_type> list = std::make_shared<yaml_list_type>(value_def.second.as<yaml_list_type>());
                                values_node.value = list;
                            }
                            if (valueType == "enum") {
                                std::shared_ptr<yaml_enum_type> enumValue = std::make_shared<yaml_enum_type>(value_def.second.as<yaml_enum_type>());
                                values_node.value = enumValue;
                            }
                        }
                    }
                }
                return true;
            }
            return false;
        }
    };

    template<>
    struct convert<yaml_type_node>{
        static Node encode(const yaml_type_node& rhs) {
            UNUSED(rhs);
            Node node;
            return node;
        }

        static bool decode(const Node& type_node, yaml_type_node& yaml_type) {
            load_node_optional<std::string>("expire_in", &(yaml_type.expire_in), type_node);
            yaml_type.key = load_node<std::string>("key", type_node);
            YAML::Node valueItems = load_node<YAML::Node>("values", type_node);
            for (const auto &values: valueItems) {
                yaml_values_node valuesNode = values.as<yaml_values_node>();
                yaml_type.values.push_back(valuesNode);
            }

            yaml_type.read_event = type_node["read_event"].as<yaml_read_event_node>();
            yaml_type.send_event = type_node["send_event"].as<yaml_send_event_node>();
            return true;
        }
    };

}


// Load the yaml configuration file
std::vector<yaml_type_node> type_defintion_file::load(){

    YAML::Node config = YAML::LoadFile(this->filename);
    YAML::Node types_node = config["types"];
    std::vector<yaml_type_node> result;
    for(const auto & type_def_names: types_node) {
        yaml_type_node yaml_type =  config[type_def_names.as<std::string>()].as<yaml_type_node>();
        yaml_type.type_name = type_def_names.as<std::string>();
        result.push_back(yaml_type);
    }
    return result;
}

std::shared_ptr <field_property> yaml_int_type::create_property() {
    return std::make_shared<simple_field>();
}

std::shared_ptr <field_property> yaml_real_type::create_property() {
    return std::make_shared<simple_field>();
}

std::shared_ptr <field_property> yaml_string_type::create_property() {
    return std::make_shared<simple_field>();
}

std::shared_ptr <field_property> yaml_enum_type::create_property() {
    return std::make_shared<enum_field>(this->enum_items);
}

std::shared_ptr <field_property> yaml_list_type::create_property() {
    return std::make_shared<list_field>(this->delimiter);
}

std::shared_ptr<field_property> yaml_index_type::create_property() {
    return std::make_shared<key_field>();
}

TYPE_KIND yaml_int_type::get_type_kind() {
    return TYPE_KIND::INT;
}

base_storage_object yaml_int_type::get_storage_value_type() {
    return storage_int();
}

TYPE_KIND yaml_real_type::get_type_kind() {
    return TYPE_KIND::REAL;
}

base_storage_object yaml_real_type::get_storage_value_type() {
    return storage_real();
}

TYPE_KIND yaml_string_type::get_type_kind() {
    return TYPE_KIND::STRING;
}

base_storage_object yaml_string_type::get_storage_value_type() {
    return storage_string();
}

TYPE_KIND yaml_list_type::get_type_kind() {
    return TYPE_KIND::STRING;
}

base_storage_object yaml_list_type::get_storage_value_type() {
    return storage_string();
}

TYPE_KIND yaml_enum_type::get_type_kind() {
    return TYPE_KIND::STRING;
}

base_storage_object yaml_enum_type::get_storage_value_type() {
    return storage_string();
}

TYPE_KIND yaml_index_type::get_type_kind() {
    return TYPE_KIND::STRING;
}

base_storage_object yaml_index_type::get_storage_value_type() {
    return storage_string();
}
