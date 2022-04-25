//
// Created by cefour on 22.04.22.
//
#include "ConfigurationFile.h"
#include <iostream>
#include <assert.h>
#include <vector>


// Load the yaml configuration file
YamlTypeNode ConfigurationFile::load(){
    static std::map<std::string,YAML_VALUE_TYPE> strToType;
    strToType["string"] = YAML_VALUE_TYPE::STRING;
    strToType["int"] = YAML_VALUE_TYPE::INT;
    strToType["real"] = YAML_VALUE_TYPE::REAL;

    YAML::Node config = YAML::LoadFile(this->fileName);
    YAML::Node typesNode = config["types"];



    YAML::Node typeNode = config["type"];
    YamlTypeNode yamlTypeNode;
    yamlTypeNode.expire_in = typeNode["expire_in"].as<std::string>();
    yamlTypeNode.typeName = typeNode["name"].as<std::string>();
    yamlTypeNode.key = typeNode["key"].as<std::string>();
    for(const auto & values : typeNode["values"] ){
        YamlValuesNode valuesNode;
        switch(values.Type()){
            case YAML::NodeType::Null : break;
            case YAML::NodeType::Scalar:
                std::cout<<values<<" S"<<std::endl;
                break;
            case YAML::NodeType::Sequence:
                std::cout<<values<<" SEQ"<<std::endl;
                break;
            case YAML::NodeType::Map:
                for(const auto & valueDefs : values){
                    valuesNode.key = valueDefs.first.as<std::string>();
                    if(valueDefs.second.IsScalar()){
                        std::string valueType = valueDefs.second.as<std::string>();
                        if(valueType == "string"){
                            YamlValueTypeNode valueTypeNode;
                            valueTypeNode.type = YAML_VALUE_TYPE::STRING;
                            valueTypeNode.value = std::make_shared<YamlStringType>();
                            valuesNode.value = valueTypeNode;

                        }
                        if(valueType == "int"){
                            YamlValueTypeNode valueTypeNode;
                            valueTypeNode.type = YAML_VALUE_TYPE::INT;
                            valueTypeNode.value = std::make_shared<YamlIntType>();
                            valuesNode.value = valueTypeNode;

                        }
                        if(valueType == "real"){
                            YamlValueTypeNode valueTypeNode;
                            valueTypeNode.type = YAML_VALUE_TYPE::REAL;
                            valueTypeNode.value = std::make_shared<YamlRealType>();
                            valuesNode.value = valueTypeNode;

                        }
                    }else{
                        for(const auto & valueDef: valueDefs.second){
                            std::string valueType = valueDef.first.as<std::string>();
                            if(valueType == "list"){
                                YamlValueTypeNode valueTypeNode;

                                std::string delimiter = valueDef.second["delimiter"].as<std::string>();
                                std::string type = valueDef.second["type"].as<std::string>();
                                auto list = std::make_shared<YamlListType>();
                                list->delimiter = delimiter;
                                list->type = strToType[type];
                                valueTypeNode.type = YAML_VALUE_TYPE::LIST;
                                valueTypeNode.value = list;
                                valuesNode.value = valueTypeNode;


                            }
                            if(valueType == "enum"){
                                YamlValueTypeNode valueTypeNode;
                                std::vector<std::string> enumItems;
                                for(const auto & enumerations : valueDef.second){
                                    enumItems.push_back(enumerations.Scalar());

                                }
                                auto enumValue = std::make_shared<YamlEnumType>();
                                enumValue->enumItems = enumItems;
                                valueTypeNode.type = YAML_VALUE_TYPE::ENUM;
                                valueTypeNode.value = enumValue;
                                valuesNode.value = valueTypeNode;

                            }
                        }
                    }
                }

                break;
            case YAML::NodeType::Undefined: break;
        }
        yamlTypeNode.values.push_back(valuesNode);
    }
    return yamlTypeNode;
}