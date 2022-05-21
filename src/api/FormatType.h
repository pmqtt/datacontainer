//
// Created by cefour on 17.05.22.
//

#ifndef DATACONTAINER_FORMATTYPE_H
#define DATACONTAINER_FORMATTYPE_H

#include "../storage/DataTypes.h"
#include <map>
#include <memory>

struct FormatTypeVisitor;

struct FormatType{
    virtual ~FormatType() = default;
    virtual bool accept(const char x) {throw std::invalid_argument("Accept not defined");}
    virtual bool isArgument()const { return true;}
    virtual DataType createDataType(){throw std::invalid_argument("CreateDataType not defined");}
    virtual std::shared_ptr<FormatType> create(const std::string & value = "") {throw std::invalid_argument("Create not defined");}
    virtual std::string format(const std::string & value) {throw std::invalid_argument("format not defined");}
    virtual void hanlde(FormatTypeVisitor * vistor) { }
};

struct FormatTypeWord : public FormatType{
    std::string toAcceptedWord;
    int index = 0;
    FormatTypeWord(const std::string & word) : toAcceptedWord(word){  }
    bool accept(const char x) {
        if(index < toAcceptedWord.length()) {
            if (toAcceptedWord[index] == x) {
                index++;
                return true;
            }
        }
        return false;
    }
    bool isArgument() const{
        return false;
    }
    std::shared_ptr<FormatType> create(const std::string & value = "")override{
        return std::make_shared<FormatTypeWord>(value);
    }
    std::string format(const std::string &value){
        return toAcceptedWord;
    }
    void hanlde(FormatTypeVisitor * visitor);


};

struct FormatTypeReal: public FormatType{
    std::string argument;
    bool accept(const char x) {
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
    bool isArgument() const{
        return true;
    }
    DataType createDataType(){
        return DataType(new RealType(std::atof(argument.c_str())));
    }
    std::shared_ptr<FormatType> create(const std::string & value = "")override{
        return std::make_shared<FormatTypeReal>();
    }
};

struct FormatTypeString: public FormatType{
    std::string argument;
    bool accept(const char x) {
        argument += x;
        return true;
    }
    bool isArgument() const{
        return true;
    }
    DataType createDataType(){
        return DataType(new StringType(argument));
    }
    std::shared_ptr<FormatType> create(const std::string & value = "")override{
        return std::make_shared<FormatTypeString>();
    }
};

template<char X>
struct FormatDateTimeType :public FormatType{
    std::shared_ptr<FormatType> create(const std::string & value = "")override{
        return std::make_shared<FormatDateTimeType<X>>();
    }

    void hanlde(FormatTypeVisitor * visitor) override;
};


using FormatTypeYear = FormatDateTimeType<'Y'>;
using FormatTypeShortYear = FormatDateTimeType<'y'>;
using FormatTypeReadableMonth = FormatDateTimeType<'M'>;
using FormatTypeDay = FormatDateTimeType<'d'>;
using FormatTypeTime = FormatDateTimeType<'T'>;
using FormatTypeTimeAll = FormatDateTimeType<'A'>;
using FormatTypeTimeHour = FormatDateTimeType<'h'>;
using FormatTypeTimeSecond = FormatDateTimeType<'s'>;
using FormatTypeTimeMinute = FormatDateTimeType<'i'>;
using FormatTypeTimeMilli = FormatDateTimeType<'l'>;
using FormatTypeTimeMicro = FormatDateTimeType<'c'>;
using FormatTypeTimeNano = FormatDateTimeType<'n'>;
using FormatTypeMonth = FormatDateTimeType<'m'>;

struct FormatTypeVisitor{
    virtual ~FormatTypeVisitor() = default;
    virtual void visit(FormatTypeYear * f){}
    virtual void visit(FormatTypeReadableMonth * f){}
    virtual void visit(FormatTypeWord * f){}
    virtual void visit(FormatTypeShortYear * f){}
    virtual void visit(FormatTypeDay * f){}
    virtual void visit(FormatTypeTime * f){}
    virtual void visit(FormatTypeTimeAll * f){}
    virtual void visit(FormatTypeTimeHour * f){}
    virtual void visit(FormatTypeTimeSecond * f){}
    virtual void visit(FormatTypeTimeMinute * f){}
    virtual void visit(FormatTypeTimeMilli * f){}
    virtual void visit(FormatTypeTimeMicro * f){}
    virtual void visit(FormatTypeTimeNano * f){}
    virtual void visit(FormatTypeMonth * f) { }
};

template<char X>
void FormatDateTimeType<X>::hanlde(FormatTypeVisitor* v){
    v->visit(this);
}


#endif //DATACONTAINER_FORMATTYPE_H
