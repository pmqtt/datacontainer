//
// Created by cefour on 22.04.22.
//
#include "ConfigurationFile.h"
#include "Property.h"
#include <iostream>
#include <assert.h>
#include <vector>



// Load the yaml configuration file
YamlTypeNode ConfigurationFile::load(){
    static std::map<std::string,YAML_VALUE_TYPE> strToType;
    strToType["string"] = YAML_VALUE_TYPE::Y_STRING;
    strToType["int"] = YAML_VALUE_TYPE::Y_INT;
    strToType["real"] = YAML_VALUE_TYPE::Y_REAL;

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
                            valuesNode.value = std::make_shared<YamlStringType>();
                        }
                        if(valueType == "int"){
                            valuesNode.value = std::make_shared<YamlIntType>();
                        }
                        if(valueType == "real"){
                            valuesNode.value = std::make_shared<YamlRealType>();
                        }
                        if(valueType == "index"){
                            valuesNode.value = std::make_shared<YamlIndexType>();
                        }
                    }else{
                        for(const auto & valueDef: valueDefs.second){
                            std::string valueType = valueDef.first.as<std::string>();
                            if(valueType == "list"){
                                std::string delimiter = valueDef.second["delimiter"].as<std::string>();
                                std::string type = valueDef.second["type"].as<std::string>();
                                auto list = std::make_shared<YamlListType>();
                                list->delimiter = delimiter;
                                list->type = strToType[type];
                                valuesNode.value = list;

                            }
                            if(valueType == "enum"){
                                std::vector<std::string> enumItems;
                                for(const auto & enumerations : valueDef.second){
                                    enumItems.push_back(enumerations.Scalar());
                                }
                                auto enumValue = std::make_shared<YamlEnumType>();
                                enumValue->enumItems = enumItems;
                                valuesNode.value = enumValue;
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