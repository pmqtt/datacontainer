//
// Created by cefour on 13.04.22.
//

#ifndef DATACONTAINER_PROPERTY_H
#define DATACONTAINER_PROPERTY_H

#include <vector>
#include <string>

class FieldProperty{
public:
    FieldProperty() = default;
    virtual ~FieldProperty() = default;
    virtual bool is(const std::string & property) = 0;

};

class SimpleField: public FieldProperty{
public:
    SimpleField() = default;
    ~SimpleField() override = default;
    bool is(const std::string & property) override{
        return property == "simple";
    }
};

class KeyField: public FieldProperty{
public:
    KeyField() = default;
    ~KeyField() override = default;
    bool is(const std::string & property) override{
        return property == "key";
    }

};

class ListField: public FieldProperty{
private:
    std::string delimiter;
public:
    ListField(const std::string& delimiter) : delimiter(delimiter) {}
    std::string getDelimiter() const { return delimiter; }
    bool is(const std::string & property) override{
        return property == "list";
    }

};

class EnumField: public FieldProperty{
private:
    std::vector<std::string> enumerations;
public:
    EnumField(const std::vector<std::string> & enumerations) : enumerations(enumerations) {}
    bool is(const std::string & property) override{
        return property == "enum";
    }
};
#endif //DATACONTAINER_PROPERTY_H
