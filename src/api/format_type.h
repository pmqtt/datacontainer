//
// Created by cefour on 17.05.22.
//

#ifndef DATACONTAINER_FORMAT_TYPE_H
#define DATACONTAINER_FORMAT_TYPE_H

#include "../storage/data_types.h"
#include "../storage/storage_object.h"
#include "macro_helper.h"
#include <map>
#include <memory>

struct format_type_visitor;

struct format_type{
    virtual ~format_type() = default;
    virtual bool accept(const char x) {
        UNUSED(x);
        throw std::invalid_argument("Accept not defined");
    }
    [[nodiscard]]virtual bool is_argument()const { return true;}
    virtual data_type create_data_type(){throw std::invalid_argument("CreateDataType not defined");}

    virtual base_storage_object create_base_storage_object(){
        throw std::invalid_argument("CreateDataType not defined");
    }

    virtual std::shared_ptr<format_type> create(const std::string & value = "") {
        UNUSED(value);
        throw std::invalid_argument("Create not defined");
    }
    virtual std::string format(const std::string & value) {
        UNUSED(value);
        throw std::invalid_argument("format_def not defined");
    }
    virtual void handle(format_type_visitor * visitor) {
        UNUSED(visitor);
    }
    virtual void clear() = 0;
};

struct format_type_word : public format_type{
    std::string toAcceptedWord;
    unsigned int index = 0;
    explicit format_type_word(const std::string & word) : toAcceptedWord(word){  }
    bool accept(const char x) override {
        if(index < toAcceptedWord.length()) {
            if (toAcceptedWord[index] == x) {
                index++;
                return true;
            }
        }
        return false;
    }
    [[nodiscard]] bool is_argument()const override {
        return false;
    }
    std::shared_ptr<format_type> create(const std::string & value = "")override{
        UNUSED(value);
        return std::make_shared<format_type_word>(value);
    }
    std::string format(const std::string &value) override{
        UNUSED(value);
        return toAcceptedWord;
    }
    void handle(format_type_visitor * visitor) override;
    void clear() override{
        index = 0;
        toAcceptedWord ="";
    }

};

struct format_type_real: public format_type{
    std::string argument;
    bool accept(const char x) override {
        if(x == '.'){
            argument += x;
            return true;
        }
        if(::isdigit(x)){
            argument += x;
            return true;
        }
        return false;
    }
    [[nodiscard]] bool is_argument() const override{
        return true;
    }
    data_type create_data_type() override{
        return {new real_type(std::atof(argument.c_str()))};
    }
    std::shared_ptr<format_type> create(const std::string & value = "")override{
        UNUSED(value);
        return std::make_shared<format_type_real>();
    }

    base_storage_object create_base_storage_object() override {
        return storage_real(std::atof(argument.c_str()));
    }
    void clear() override{
        argument="";
    }
};

struct format_type_string: public format_type{
    std::string argument;
    bool accept(const char x) override {
        if(::isblank(x)) return false;
        argument += x;
        return true;
    }
    [[nodiscard]] bool is_argument() const override{
        return true;
    }
    data_type create_data_type() override{
        return {new string_type(argument)};
    }
    std::shared_ptr<format_type> create(const std::string & value = "")override{
        UNUSED(value);
        return std::make_shared<format_type_string>();
    }
    base_storage_object create_base_storage_object() override {
        return storage_string(argument);
    }
    void clear() override{
        argument="";
    }
};

template<char X>
struct format_date_time_type : public format_type{
    std::shared_ptr<format_type> create(const std::string & value = "")override{
        UNUSED(value);
        return std::make_shared<format_date_time_type<X>>();
    }

    void handle(format_type_visitor * visitor) override;
    void clear() override{}
};


using format_type_year = format_date_time_type<'Y'>;
using format_type_short_year = format_date_time_type<'y'>;
using format_type_readable_month = format_date_time_type<'M'>;
using format_type_day = format_date_time_type<'d'>;
using format_type_time = format_date_time_type<'T'>;
using format_type_time_all = format_date_time_type<'A'>;
using format_type_time_hour = format_date_time_type<'h'>;
using format_type_time_second = format_date_time_type<'s'>;
using format_type_time_minute = format_date_time_type<'i'>;
using format_type_time_milli = format_date_time_type<'l'>;
using format_type_time_micro = format_date_time_type<'c'>;
using format_type_time_nano = format_date_time_type<'n'>;
using format_type_month = format_date_time_type<'m'>;

struct format_type_visitor{
    virtual ~format_type_visitor() = default;
    virtual void visit(format_type_year * f){UNUSED(f);}
    virtual void visit(format_type_readable_month * f){UNUSED(f);}
    virtual void visit(format_type_word * f){UNUSED(f);}
    virtual void visit(format_type_short_year * f){UNUSED(f);}
    virtual void visit(format_type_day * f){UNUSED(f);}
    virtual void visit(format_type_time * f){UNUSED(f);}
    virtual void visit(format_type_time_all * f){UNUSED(f);}
    virtual void visit(format_type_time_hour * f){UNUSED(f);}
    virtual void visit(format_type_time_second * f){UNUSED(f);}
    virtual void visit(format_type_time_minute * f){UNUSED(f);}
    virtual void visit(format_type_time_milli * f){UNUSED(f);}
    virtual void visit(format_type_time_micro * f){UNUSED(f);}
    virtual void visit(format_type_time_nano * f){UNUSED(f);}
    virtual void visit(format_type_month * f) { UNUSED(f); }
};

template<char X>
void format_date_time_type<X>::handle(format_type_visitor* v){
    v->visit(this);
}


#endif //DATACONTAINER_FORMAT_TYPE_H
