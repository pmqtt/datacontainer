//
// Created by cefour on 22.04.22.
//

#ifndef DATACONTAINER_CONFIGURATIONFILE_H
#define DATACONTAINER_CONFIGURATIONFILE_H
#include <string>
#include "yaml-cpp/yaml.h"
#include "DataTypes.h"

enum YAML_VALUE_TYPE{
    Y_LIST,
    Y_STRING,
    Y_INT,
    Y_REAL,
    Y_ENUM,
    Y_INDEX
};


struct YamlIntType;
struct YamlRealType;
struct YamlStringType;
struct YamlListType;
struct YamlEnumType;
struct YamlIndexType;
struct FieldProperty;


struct YamlValueTypeBase{
    YamlValueTypeBase() = default;
    virtual ~YamlValueTypeBase() = default;
    virtual std::shared_ptr<FieldProperty> createProperty()= 0;
    virtual TYPE_KIND getTypeKind() = 0;
};

struct YamlIntType : public YamlValueTypeBase{
    std::shared_ptr<FieldProperty> createProperty();
    TYPE_KIND getTypeKind();

};
struct YamlRealType : public YamlValueTypeBase{
    std::shared_ptr<FieldProperty> createProperty();
    TYPE_KIND getTypeKind();
};
struct YamlStringType : public YamlValueTypeBase{
    std::shared_ptr<FieldProperty> createProperty();
    TYPE_KIND getTypeKind();
};
struct YamlListType : public YamlValueTypeBase{
    std::string delimiter;
    YAML_VALUE_TYPE type;
    std::shared_ptr<FieldProperty> createProperty();
    TYPE_KIND getTypeKind();
};
struct YamlEnumType : public YamlValueTypeBase{
    std::vector<std::string> enumItems;
    std::shared_ptr<FieldProperty> createProperty();
    TYPE_KIND getTypeKind();
};

struct YamlIndexType : public YamlValueTypeBase{
    std::shared_ptr<FieldProperty> createProperty();
    TYPE_KIND getTypeKind();
};



struct YamlValuesNode{
    std::string key;
    std::shared_ptr<YamlValueTypeBase> value;
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
