//
// Created by cefour on 12.04.22.
//

#ifndef DATACONTAINER_DATATYPES_H
#define DATACONTAINER_DATATYPES_H

#include <any>
#include <string>
#include <algorithm>
#include <iostream>

enum TYPE_KIND{
    INT,
    REAL,
    STRING
};

class DataType{
private:
    TYPE_KIND kind;
    DataType * innerType;
public:
    DataType() = default;
    DataType(DataType * type,TYPE_KIND kind){
        this->innerType = type;
        this->kind = kind;
    }
    DataType(DataType * type){
        this->innerType = type;
        this->kind = type->kind;
    }

    virtual ~DataType() = default;
    bool isInteger() { return isType(TYPE_KIND::INT);     }
    bool isReal()    { return isType(TYPE_KIND::REAL);    }
    bool isString()  { return isType(TYPE_KIND::STRING);  }

    template<class TYPE>
    TYPE as()const{
        return std::any_cast<TYPE>(innerType->getValue());
    }

    bool operator<(const DataType & rhs);

    bool operator==(const DataType & rhs);

    std::size_t hash()const{
        return innerType->getHash();
    }
    void print()const{
        innerType->printToConsole();
    }

    virtual std::size_t getHash() const { return -1; }



private:
    bool isType(TYPE_KIND type){
        return kind == type;
    }
protected:
    virtual std::any getValue() const { return nullptr; }
    DataType(TYPE_KIND kind){
        this->kind = kind;
    }
    virtual void printToConsole() const {}

};


template<class TYPE,TYPE_KIND KIND>
class TemplateType : public DataType{
private:
    TYPE value;
public:
    TemplateType(TYPE value) :
            DataType(KIND),
            value(value){
    }

    ~TemplateType() override { }


    operator TYPE (){
        return value;
    }
    std::size_t getHash() const override;

protected:
    std::any getValue() const override{
        return this->value;
    }

    void printToConsole() const override{
        std::cout<<"VALUE:"<<value<<std::endl;
    }


};

using IntType =  TemplateType<int,TYPE_KIND::INT>;
using RealType =  TemplateType<double,TYPE_KIND::REAL>;
using StringType =  TemplateType<std::string,TYPE_KIND::STRING>;


template<>
struct std::hash<IntType>
{
    std::size_t operator()(const IntType & data) const noexcept
    {
        int v = data.as<int>();
        return std::hash<int>{}(v);
    }
};


template<>
struct std::hash<RealType>
{
    std::size_t operator()(const RealType & data) const noexcept
    {
        double v = data.as<double>();
        return std::hash<double>{}(v);
    }
};

template<>
struct std::hash<StringType>
{
    std::size_t operator()(const StringType & data) const noexcept
    {
        std::string v = data.as<std::string>();
        return std::hash<std::string>{}(v);
    }
};


template<>
struct std::hash<DataType>
{
    std::size_t operator()(const DataType &data)const noexcept{
        return data.hash();
    }
};


template<class TYPE,TYPE_KIND KIND>
std::size_t TemplateType<TYPE,KIND>::getHash()const {
    return std::hash<TYPE>{}(this->value);
}


struct DataTypeComparator{
    bool operator()(const DataType& a, const DataType& b) const {
        return a.hash() < b.hash();
    }
};

#endif //DATACONTAINER_DATATYPES_H
