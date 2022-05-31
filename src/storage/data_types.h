//
// Created by cefour on 12.04.22.
//

#ifndef DATACONTAINER_DATA_TYPES_H
#define DATACONTAINER_DATA_TYPES_H

#include <any>
#include <string>
#include <algorithm>
#include <iostream>

enum TYPE_KIND{
    INT,
    REAL,
    STRING
};

class data_type{
private:
    TYPE_KIND kind;
    data_type * inner_type;
public:
    data_type() = default;
    data_type(data_type * type, TYPE_KIND kind){
        this->inner_type = type;
        this->kind = kind;
    }
    data_type(data_type * type){
        this->inner_type = type;
        this->kind = type->kind;
    }

    virtual ~data_type() = default;
    bool is_integer() { return is_type(TYPE_KIND::INT);     }
    bool is_real()    { return is_type(TYPE_KIND::REAL);    }
    bool is_string()  { return is_type(TYPE_KIND::STRING);  }

    template<class TYPE>
    TYPE as()const{
        return std::any_cast<TYPE>(inner_type->get_value());
    }

    bool operator<(const data_type & rhs);

    bool operator==(const data_type & rhs);

    std::size_t hash()const{
        return inner_type->get_hash();
    }
    void print()const{
        inner_type->print_to_console();
    }

    virtual std::size_t get_hash() const { return -1; }



private:
    bool is_type(TYPE_KIND type){
        return kind == type;
    }
protected:
    virtual std::any get_value() const { return nullptr; }
    data_type(TYPE_KIND kind){
        this->kind = kind;
    }
    virtual void print_to_console() const {}

};


template<class TYPE,TYPE_KIND KIND>
class template_type : public data_type{
private:
    TYPE value;
public:
    template_type(TYPE value) :
            data_type(KIND),
            value(value){
    }

    ~template_type() override { }


    operator TYPE (){
        return value;
    }
    std::size_t get_hash() const override;

protected:
    std::any get_value() const override{
        return this->value;
    }

    void print_to_console() const override{
        std::cout<<"VALUE:"<<value<<std::endl;
    }


};

using int_type =  template_type<int,TYPE_KIND::INT>;
using real_type =  template_type<double,TYPE_KIND::REAL>;
using string_type =  template_type<std::string,TYPE_KIND::STRING>;


template<>
struct std::hash<int_type>
{
    std::size_t operator()(const int_type & data) const noexcept
    {
        int v = data.as<int>();
        return std::hash<int>{}(v);
    }
};


template<>
struct std::hash<real_type>
{
    std::size_t operator()(const real_type & data) const noexcept
    {
        double v = data.as<double>();
        return std::hash<double>{}(v);
    }
};

template<>
struct std::hash<string_type>
{
    std::size_t operator()(const string_type & data) const noexcept
    {
        std::string v = data.as<std::string>();
        return std::hash<std::string>{}(v);
    }
};


template<>
struct std::hash<data_type>
{
    std::size_t operator()(const data_type &data)const noexcept{
        return data.hash();
    }
};


template<class TYPE,TYPE_KIND KIND>
std::size_t template_type<TYPE,KIND>::get_hash()const {
    return std::hash<TYPE>{}(this->value);
}


struct DataTypeComparator{
    bool operator()(const data_type& a, const data_type& b) const {
        return a.hash() < b.hash();
    }
};

#endif //DATACONTAINER_DATA_TYPES_H
