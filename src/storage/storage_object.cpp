//
// Created by cefour on 31.05.22.
//
#include "../exceptions/assing_storage_object_to_index_value_type_exception.h"
#include "storage_object.h"


#define COMPARATOR(func) \
    return std::visit([&](const auto & x){ \
        return std::visit([&](const auto & y){ \
            return func(x,y); \
        },b); \
    },a);

bool ptl::cmp_less(const base_storage_object &a, const base_storage_object &b){
    COMPARATOR(storage_cmp_less);
}

bool ptl::cmp_less_equal(const base_storage_object &a, const base_storage_object &b) {
    COMPARATOR(storage_cmp_less_equal);
}

bool ptl::cmp_greater(const base_storage_object &a, const base_storage_object &b){
    COMPARATOR(storage_cmp_greater);
}

bool ptl::cmp_greater_equal(const base_storage_object &a, const base_storage_object &b){
    COMPARATOR(storage_cmp_greater_equal);

}

bool ptl::cmp_equal(const base_storage_object &a, const base_storage_object &b){
    COMPARATOR(storage_cmp_equal);
}

bool ptl::cmp_not_equal(const base_storage_object &a, const base_storage_object &b){
    COMPARATOR(storage_cmp_not_equal);
}

base_storage_object ptl::add(const base_storage_object &a, const base_storage_object &b){
    return std::visit([&](const auto & x){
        return std::visit([&](const auto & y){
            return base_storage_object{storage_object_add(x,y)};
        },b);
    },a);

}

bool operator==(const base_storage_object &a, const base_storage_object &b){
    return ptl::cmp_equal(a,b);
}



index_value_type assign_to_index_value(const base_storage_object & rhs){
    /*
     * Why not using visit with Overload pattern? Yeh, it's not possible because visit try to substitute all base_storage_object types!
     */
    index_value_type  key;
    if(std::holds_alternative<storage_string>(rhs)){
        return std::get<storage_string>(rhs);
    }else if(std::holds_alternative<storage_int>(rhs )){
        key = std::get<storage_int>(rhs);
    }else{
        throw assing_storage_object_to_index_value_type_exception("assign base_storage_object to index_value_type not possible");
    }
    return key;
}