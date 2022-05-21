//
// Created by cefour on 22.04.22.
//
#include "ConfigurationFile.h"
#include "../storage/Property.h"
#include <iostream>
#include <assert.h>
#include <vector>
#include "../exceptions/TypedefException.h"


namespace YAML {
    template<>
    struct convert<YamlReadEventNode> {
        static Node encode(const YamlReadEventNode& rhs) {
            Node node;
            return node;
        }

        static bool decode(const Node& node, YamlReadEventNode& rhs) {
            rhs.connectionType = node["type"].as<std::string>();
            rhs.brokerAdr = node["broker"].as<std::string>();
            rhs.subscribe = node["subscribe"].as<std::string>();
            rhs.format = node["format"].as<std::string>();
            std::remove(rhs.format.begin(), rhs.format.end(), '\"');

            for(int i = 0; i < node["insert"].size(); ++i){
                for(const auto & iter : node["insert"][i]){
                    rhs.insertions[iter.first.as<std::string>()] = iter.second.as<std::string>();
                }
            }
            if(node["read_event"].IsDefined()) {
                rhs.nextEvents.push_back(node["read_event"].as<YamlReadEventNode>());
            }
            return true;
        }
    };

    template<>
    struct convert<YamlSendEventNode> {
        static Node encode(const YamlSendEventNode& rhs) {
            Node node;
            return node;
        }

        static bool decode(const Node& node, YamlSendEventNode& rhs) {
/*
 *   send_event:
    - type: mqtt
    - broker: localhost:1883
    - topic: CALCULATED
    - prepare: mean(TEMPERATURE)
    - when:
        - count(TEMPERATURE) >= 3
        - reset: true
 */
            rhs.connectionType = node["type"].as<std::string>();
            rhs.brokerAdr = node["broker"].as<std::string>();
            rhs.topic = node["topic"].as<std::string>();
            rhs.prepare = node["prepare"].as<std::string>();
            return true;
        }
    };

    template<>
    struct convert<YamlListType>{
        static Node encode(const YamlListType& rhs) {
            Node node;
            return node;
        }
        static bool decode(const Node& listEntry, YamlListType& list) {
            static std::map<std::string,YAML_VALUE_TYPE> strToType;
            strToType["string"] = YAML_VALUE_TYPE::Y_STRING;
            strToType["int"] = YAML_VALUE_TYPE::Y_INT;
            strToType["real"] = YAML_VALUE_TYPE::Y_REAL;

            std::string delimiter = listEntry["delimiter"].as<std::string>();
            std::string type = listEntry["type"].as<std::string>();
            list.delimiter = delimiter;
            list.type = strToType[type];
            return true;
        }
    };

    template<>
    struct convert<YamlEnumType>{
        static Node encode(const YamlEnumType& rhs) {
            Node node;
            return node;
        }
        static bool decode(const Node& enumEntry, YamlEnumType& e) {
            std::vector<std::string> enumItems;
            for (const auto &enumerations: enumEntry) {
                enumItems.push_back(enumerations.Scalar());
            }
            e.enumItems = enumItems;
            return true;
        }
    };


    template<>
    struct convert<YamlValuesNode>{
        static Node encode(const YamlValuesNode& rhs) {
            Node node;
            return node;
        }

        static bool decode(const Node& values, YamlValuesNode& valuesNode) {
            if(YAML::NodeType::Map == values.Type()){
                for (const auto &valueDefs: values) {
                    valuesNode.key = valueDefs.first.as<std::string>();
                    if (valueDefs.second.IsScalar()) {
                        std::string valueType = valueDefs.second.as<std::string>();
                        if (valueType == "string") {
                            valuesNode.value = std::make_shared<YamlStringType>();
                        }
                        if (valueType == "int") {
                            valuesNode.value = std::make_shared<YamlIntType>();
                        }
                        if (valueType == "real") {
                            valuesNode.value = std::make_shared<YamlRealType>();
                        }
                        if (valueType == "index") {
                            valuesNode.value = std::make_shared<YamlIndexType>();
                        }
                    } else {
                        for (const auto &valueDef: valueDefs.second) {
                            std::string valueType = valueDef.first.as<std::string>();
                            if (valueType == "list") {
                                std::shared_ptr<YamlListType> list = std::make_shared<YamlListType>(valueDef.second.as<YamlListType>());
                                valuesNode.value = list;
                            }
                            if (valueType == "enum") {
                                std::shared_ptr<YamlEnumType> enumValue = std::make_shared<YamlEnumType>(valueDef.second.as<YamlEnumType>());
                                valuesNode.value = enumValue;
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
    struct convert<YamlTypeNode>{
        static Node encode(const YamlTypeNode& rhs) {
            Node node;
            return node;
        }

        static bool decode(const Node& typeNode, YamlTypeNode& yamlTypeNode) {
            loadNodeOptional<std::string>("expire_in",&(yamlTypeNode.expire_in),typeNode);
            yamlTypeNode.key = loadNode<std::string>("key",typeNode);
            YAML::Node valueItems = loadNode<YAML::Node>("values",typeNode);
            for (const auto &values: valueItems) {
                YamlValuesNode valuesNode = values.as<YamlValuesNode>();
                yamlTypeNode.values.push_back(valuesNode);
            }

            yamlTypeNode.readEvent = typeNode["read_event"].as<YamlReadEventNode>();
            yamlTypeNode.sendEvent = typeNode["send_event"].as<YamlSendEventNode>();
            return true;
        }
    };

}


// Load the yaml configuration file
std::vector<YamlTypeNode> ConfigurationFile::load(){

    YAML::Node config = YAML::LoadFile(this->fileName);
    YAML::Node typesNode = config["types"];
    std::vector<YamlTypeNode> result;
    for(const auto & typeDefNames: typesNode) {
        YamlTypeNode yamlTypeNode =  config[typeDefNames.as<std::string>()].as<YamlTypeNode>();
        yamlTypeNode.typeName = typeDefNames.as<std::string>();
        result.push_back(yamlTypeNode);
    }
    return result;
}

std::shared_ptr <FieldProperty> YamlIntType::createProperty() {
    return std::make_shared<SimpleField>();
}

std::shared_ptr <FieldProperty> YamlRealType::createProperty() {
    return std::make_shared<SimpleField>();
}

std::shared_ptr <FieldProperty> YamlStringType::createProperty() {
    return std::make_shared<SimpleField>();
}

std::shared_ptr <FieldProperty> YamlEnumType::createProperty() {
    return std::make_shared<EnumField>(this->enumItems);
}

std::shared_ptr <FieldProperty> YamlListType::createProperty() {
    return std::make_shared<ListField>(this->delimiter);
}

std::shared_ptr<FieldProperty> YamlIndexType::createProperty() {
    return std::make_shared<KeyField>();
}

TYPE_KIND YamlIntType::getTypeKind() {
    return TYPE_KIND::INT;
}
TYPE_KIND YamlRealType::getTypeKind() {
    return TYPE_KIND::REAL;
}
TYPE_KIND YamlStringType::getTypeKind() {
    return TYPE_KIND::STRING;
}
TYPE_KIND YamlListType::getTypeKind() {
    return TYPE_KIND::STRING;
}
TYPE_KIND YamlEnumType::getTypeKind() {
    return TYPE_KIND::STRING;
}

TYPE_KIND YamlIndexType::getTypeKind() {
    return TYPE_KIND::STRING;
}