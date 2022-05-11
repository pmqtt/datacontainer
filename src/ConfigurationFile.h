//
// Created by cefour on 22.04.22.
//

#ifndef DATACONTAINER_CONFIGURATIONFILE_H
#define DATACONTAINER_CONFIGURATIONFILE_H
#include <string>
#include "yaml-cpp/yaml.h"
#include "storage/DataTypes.h"

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

struct YamlReadEventNode{
    std::string connectionType;
    std::string brokerAdr;
    std::string subscribe;
    std::string format;
    std::map<std::string,std::string> insertions;
    std::vector<YamlReadEventNode> nextEvents;
};

struct YamlSendEventNode{
    std::string connectionType;
    std::string brokerAdr;
    std::string topic;
    std::string prepare;
};

namespace YAML {
    template<>
    struct convert<YamlReadEventNode> {
        static Node encode(const YamlReadEventNode& rhs) {
            Node node;
           /* node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);*/
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

}



struct YamlValuesNode{
    std::string key;
    std::shared_ptr<YamlValueTypeBase> value;

};

struct YamlTypeNode{
    std::string expire_in;
    std::string typeName;
    std::string key;
    std::vector<YamlValuesNode> values;
    YamlReadEventNode readEvent;
    YamlSendEventNode sendEvent;
};




class ConfigurationFile{
public:
    ConfigurationFile(const std::string & fileName){
        this->fileName = fileName;
    }
    std::vector<YamlTypeNode> load();

private:
    std::string fileName;
};

#endif //DATACONTAINER_CONFIGURATIONFILE_H
