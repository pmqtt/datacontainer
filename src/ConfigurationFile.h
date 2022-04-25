//
// Created by cefour on 22.04.22.
//

#ifndef DATACONTAINER_CONFIGURATIONFILE_H
#define DATACONTAINER_CONFIGURATIONFILE_H
#include <string>
#include "yaml-cpp/yaml.h"
enum YAML_VALUE_TYPE{
    Y_LIST,
    Y_STRING,
    Y_INT,
    Y_REAL,
    Y_ENUM
};

struct YamlIntType;
struct YamlRealType;
struct YamlStringType;
struct YamlListType;
struct YamlEnumType;


struct YamlValueTypeBase{
    YamlValueTypeBase() = default;
    virtual ~YamlValueTypeBase() = default;
};

struct YamlIntType : public YamlValueTypeBase{};
struct YamlRealType : public YamlValueTypeBase{};
struct YamlStringType : public YamlValueTypeBase{};
struct YamlListType : public YamlValueTypeBase{
    std::string delimiter;
    YAML_VALUE_TYPE type;
};
struct YamlEnumType : public YamlValueTypeBase{
    std::vector<std::string> enumItems;
};
struct YamlValueTypeNode{
    YAML_VALUE_TYPE type;
    std::shared_ptr<YamlValueTypeBase> value;
};

struct YamlValuesNode{
    std::string key;
    YamlValueTypeNode value;
};

struct YamlTypeNode{
    std::string expire_in;
    std::string typeName;
    std::string key;
    std::vector<YamlValuesNode> values;
};




class ConfigurationFile{
public:
    ConfigurationFile(const std::string & fileName){
        this->fileName = fileName;
    }
    YamlTypeNode load();

private:
    std::string fileName;
};

#endif //DATACONTAINER_CONFIGURATIONFILE_H
