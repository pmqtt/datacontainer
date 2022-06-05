//
// Created by cefour on 31.05.22.
//
#include "../api/crtp.h"
#include "../api/macro_helper.h"
#include "storage_object.h"
#include <map>
#include <stdexcept>

#define VISITOR_CREATE_INNER_VISIT(TYPE,FUNC) \
[&](const TYPE & y) {\
    return FUNC(x,y);                                              \
}

#define VISITOR_CREATE_INNER_THROW_VISIT(TYPE,THROW_TEXT) \
[&](const TYPE & y) {\
   UNUSED(y); \
   throw std::runtime_error(THROW_TEXT);\
   return false; \
}


#define VISITOR_COMPERATOR(FUNC) \
Overload{ \
    [&](const storage_int &x) { \
        auto chk = Overload{     \
            VISITOR_CREATE_INNER_VISIT(storage_int,FUNC),  \
            VISITOR_CREATE_INNER_VISIT(storage_real,FUNC), \
            VISITOR_CREATE_INNER_THROW_VISIT(storage_string,"type storage_int and storage_string are not compatible to compare"), \
            VISITOR_CREATE_INNER_THROW_VISIT(storage_bool,"type storage_int and storage_bool are not compatible to compare")\
        };\
        return std::visit(chk, b);\
    },\
    [&](const storage_real &x) {\
        auto chk = Overload{\
            VISITOR_CREATE_INNER_VISIT(storage_int,FUNC),  \
            VISITOR_CREATE_INNER_VISIT(storage_real,FUNC),                                                                  \
            VISITOR_CREATE_INNER_THROW_VISIT(storage_string,"type storage_real and storage_string are not compatible to compare"), \
            VISITOR_CREATE_INNER_THROW_VISIT(storage_bool,"type storage_real and storage_bool are not compatible to compare")\
        };\
        return std::visit(chk, b);\
    },\
    [&](const storage_string &x) {\
        auto chk = Overload{     \
            VISITOR_CREATE_INNER_THROW_VISIT(storage_int,"type storage_int and storage_string are not compatible to compare"),  \
            VISITOR_CREATE_INNER_THROW_VISIT(storage_real,"type storage_real and storage_string are not compatible to compare"),\
            VISITOR_CREATE_INNER_THROW_VISIT(storage_bool,"type storage_string and storage_bool are not compatible to compare"),\
            VISITOR_CREATE_INNER_VISIT(storage_string,FUNC) \
        };\
        return std::visit(chk, b);\
    },                           \
    [&](const storage_bool &x) {\
        auto chk = Overload{     \
            VISITOR_CREATE_INNER_THROW_VISIT(storage_int   ,"type storage_int and storage_bool are not compatible to compare"),  \
            VISITOR_CREATE_INNER_THROW_VISIT(storage_real  ,"type storage_real and storage_bool are not compatible to compare"),    \
            VISITOR_CREATE_INNER_THROW_VISIT(storage_string,"type storage_string and storage_bool are not compatible to compare"),\
            VISITOR_CREATE_INNER_VISIT(storage_bool,FUNC) \
        };\
        return std::visit(chk, b);   \
    }                            \
}


bool ptl::cmp_less(const base_storage_object &a, const base_storage_object &b) {
    auto check_less_overload = VISITOR_COMPERATOR(storage_cmp_less);
    return std::visit(check_less_overload, a);
}

bool ptl::cmp_less_equal(const base_storage_object &a, const base_storage_object &b) {
    auto check_less_overload = VISITOR_COMPERATOR(storage_cmp_less_equal);
    return std::visit(check_less_overload, a);
}

bool ptl::cmp_greater(const base_storage_object &a, const base_storage_object &b){
    auto check_less_overload = VISITOR_COMPERATOR(storage_cmp_greater);
    return std::visit(check_less_overload, a);
}

bool ptl::cmp_greater_equal(const base_storage_object &a, const base_storage_object &b){
    auto check_less_overload = VISITOR_COMPERATOR(storage_cmp_greater_equal);
    return std::visit(check_less_overload, a);
}

bool ptl::cmp_equal(const base_storage_object &a, const base_storage_object &b){
    auto check_less_overload = VISITOR_COMPERATOR(storage_cmp_equal);
    return std::visit(check_less_overload, a);
}

bool ptl::cmp_not_equal(const base_storage_object &a, const base_storage_object &b){
    auto check_less_overload = VISITOR_COMPERATOR(storage_cmp_not_equal);
    return std::visit(check_less_overload, a);
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
