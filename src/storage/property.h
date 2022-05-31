//
// Created by cefour on 13.04.22.
//

#ifndef DATACONTAINER_PROPERTY_H
#define DATACONTAINER_PROPERTY_H

#include <vector>
#include <string>

class field_property{
public:
    field_property() = default;
    virtual ~field_property() = default;
    virtual bool is(const std::string & property) = 0;

};

class simple_field: public field_property{
public:
    simple_field() = default;
    ~simple_field() override = default;
    bool is(const std::string & property) override{
        return property == "simple";
    }
};

class key_field: public field_property{
public:
    key_field() = default;
    ~key_field() override = default;
    bool is(const std::string & property) override{
        return property == "key";
    }

};

class list_field: public field_property{
private:
    std::string delimiter;
public:
    list_field(const std::string& delimiter) : delimiter(delimiter) {}
    std::string get_delimiter() const { return delimiter; }
    bool is(const std::string & property) override{
        return property == "list";
    }

};

class enum_field: public field_property{
private:
    std::vector<std::string> enumerations;
public:
    enum_field(const std::vector<std::string> & enumerations) : enumerations(enumerations) {}
    bool is(const std::string & property) override{
        return property == "enum";
    }
};
#endif //DATACONTAINER_PROPERTY_H
