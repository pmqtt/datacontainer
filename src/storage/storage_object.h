//
// Created by cefour on 31.05.22.
//

#ifndef DATACONTAINER_STORAGE_OBJECT_H
#define DATACONTAINER_STORAGE_OBJECT_H

#include <any>
#include <exception>
#include <stdexcept>
#include <type_traits>
#include <variant>

struct bool_comparable{
};
struct numeric_comparable {
};
struct charcter_comparable {
};
struct item_comparable {
};


template<class CHILD, class TYPE>
class storage_object {
public:
    using child_type = CHILD;
    using inner_type = TYPE;
public:
    storage_object() {}

    virtual ~storage_object() { }

    const inner_type &get_value() const {
        const child_type &x = static_cast<const child_type &>(*this);
        return x.get_value();
    }
};


template<class TYPE, class COMPARABLE>
struct storage_type : public storage_object<storage_type<TYPE, COMPARABLE>, TYPE> {
public:
    using comparble = COMPARABLE;
public:
    storage_type() :value{} {}

    storage_type(const TYPE &v) : value(v) {}

    virtual ~storage_type() override { };

    const TYPE &get_value() const {
        return value;
    }

    std::size_t hash() const {
        return std::hash<TYPE>{}(value);
    }

private:
    TYPE value;
};

#include "storage_object_comparator_impl.h"


template<class X, class C1, class Y, class C2>
using storage_object_result_t = typename std::conditional<
        std::is_arithmetic<X>::value && std::is_arithmetic<Y>::value && !std::is_same<X,bool>::value && !std::is_same<Y,bool>::value,
        typename std::conditional<std::is_same<X,int>::value && std::is_same<Y,int>::value,int,double>::type,X>::type;


template<class X, class C1, class Y, class C2>
storage_type<storage_object_result_t<X,C1,Y,C2>,C1>
        storage_object_add(const storage_type<X, C1> &x, const storage_type<Y, C2> &y){
    if constexpr( std::is_arithmetic<X>::value && std::is_arithmetic<Y>::value ) {
        storage_type<storage_object_result_t<X, C1, Y, C2>, C1> result(x.get_value() + y.get_value());
        return result;
    }
    using XX = typename  std::remove_reference<typename std::remove_const<X>::type>::type;
    using YY = typename  std::remove_reference<typename std::remove_const<Y>::type>::type ;

    if constexpr (std::is_same<XX,std::string>::value && std::is_same<YY,std::string>::value){
        storage_type<storage_object_result_t<X, C1, Y, C2>, C1> result(x.get_value() + y.get_value());
        return result;
    }
    throw std::runtime_error("Types not addeable");
}



using storage_bool = storage_type<bool, bool_comparable>;
using storage_int = storage_type<int, numeric_comparable>;
using storage_real = storage_type<double, numeric_comparable>;
using storage_string = storage_type<std::string, charcter_comparable>;


using base_storage_object = std::variant<storage_bool, storage_int, storage_real, storage_string>;
using index_value_type = std::variant<storage_int, storage_string>;


namespace ptl {
    bool cmp_greater(const base_storage_object &a, const base_storage_object &b);

    bool cmp_greater_equal(const base_storage_object &a, const base_storage_object &b);

    bool cmp_equal(const base_storage_object &a, const base_storage_object &b);

    bool cmp_not_equal(const base_storage_object &a, const base_storage_object &b);

    bool cmp_less(const base_storage_object &a, const base_storage_object &b);

    bool cmp_less_equal(const base_storage_object &a, const base_storage_object &b);

    base_storage_object add(const base_storage_object &a, const base_storage_object &b);
}


template<>
struct std::hash<base_storage_object> {
    std::size_t operator()(const base_storage_object &obj) const noexcept {
        return std::visit([](const auto &o) {
            return o.hash();
        }, obj);
    }
};

template<>
struct std::hash<index_value_type> {
    std::size_t operator()(const index_value_type &obj) const noexcept {
        return std::visit([](const auto &o) {
            return o.hash();
        }, obj);
    }
};


bool operator==(const base_storage_object &a, const base_storage_object &b);
index_value_type assign_to_index_value(const base_storage_object & rhs);

#endif //DATACONTAINER_STORAGE_OBJECT_H
