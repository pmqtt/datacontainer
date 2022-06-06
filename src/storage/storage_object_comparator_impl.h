//
// Created by cefour on 01.06.22.
//

#ifndef DATACONTAINER_STORAGE_OBJECT_COMPARATOR_IMPL_H
#define DATACONTAINER_STORAGE_OBJECT_COMPARATOR_IMPL_H


template<class X, class C1, class Y, class C2>
bool storage_cmp_greater(const storage_type<X, C1> &x, const storage_type<Y, C2> &y){
    if constexpr( std::is_same<C1,numeric_comparable>::value && std::is_same<C2,numeric_comparable>::value ){
        return x.get_value() > y.get_value();
    }
    if constexpr(std::is_same<C1,bool_comparable>::value || std::is_same<C2,bool_comparable>::value){
        throw std::runtime_error("comparbale greater with type boolean is not allowes");
    }
    if constexpr(std::is_same<C1,C2>::value){
        return x.get_value() > y.get_value();
    }
    throw std::runtime_error("comparbale greater with different types not allowes");
}

template<class X, class C1, class Y, class C2>
bool storage_cmp_greater_equal(const storage_type<X, C1> &x, const storage_type<Y, C2> &y){
    if constexpr( std::is_same<C1,numeric_comparable>::value && std::is_same<C2,numeric_comparable>::value ){
        return x.get_value() >= y.get_value();
    }
    if constexpr(std::is_same<C1,bool_comparable>::value || std::is_same<C2,bool_comparable>::value){
        throw std::runtime_error("comparbale greater equal with type boolean is not allowes");
    }
    if constexpr(std::is_same<C1,C2>::value){
        return x.get_value() >= y.get_value();
    }
    throw std::runtime_error("comparbale greater equal with different types not allowes");
}


template<class X, class C1, class Y, class C2>
bool storage_cmp_less(const storage_type<X, C1> &x, const storage_type<Y, C2> &y){
    if constexpr( std::is_same<C1,numeric_comparable>::value && std::is_same<C2,numeric_comparable>::value ){
        return x.get_value() < y.get_value();
    }
    if constexpr(std::is_same<C1,bool_comparable>::value || std::is_same<C2,bool_comparable>::value){
        throw std::runtime_error("comparbale less with type boolean is not allowes");
    }
    if constexpr(std::is_same<C1,C2>::value){
        return x.get_value() < y.get_value();
    }
    throw std::runtime_error("comparbale less with different types not allowes");
}

template<class X, class C1, class Y, class C2>
bool storage_cmp_less_equal(const storage_type<X, C1> &x, const storage_type<Y, C2> &y){
    if constexpr( std::is_same<C1,numeric_comparable>::value && std::is_same<C2,numeric_comparable>::value ){
        return x.get_value() <= y.get_value();
    }
    if constexpr(std::is_same<C1,bool_comparable>::value || std::is_same<C2,bool_comparable>::value){
        throw std::runtime_error("comparbale less equal with type boolean is not allowes");
    }
    if constexpr(std::is_same<C1,C2>::value){
        return x.get_value() <= y.get_value();
    }
    throw std::runtime_error("comparbale less equal with different types not allowes");
}

template<class X, class C1, class Y, class C2>
bool storage_cmp_equal(const storage_type<X, C1> &x, const storage_type<Y, C2> &y){
    if constexpr( std::is_same<C1,numeric_comparable>::value && std::is_same<C2,numeric_comparable>::value ){
        return x.get_value() == y.get_value();
    }
    if constexpr(std::is_same<C1,C2>::value){
        return x.get_value() == y.get_value();
    }
    throw std::runtime_error("comparbale equal with different types not allowes");
}

template<class X, class C1, class Y, class C2>
bool storage_cmp_not_equal(const storage_type<X, C1> &x, const storage_type<Y, C2> &y){
    if constexpr( std::is_same<C1,numeric_comparable>::value && std::is_same<C2,numeric_comparable>::value ){
        return x.get_value() != y.get_value();
    }
    if constexpr(std::is_same<C1,C2>::value){
        return x.get_value() != y.get_value();
    }
    throw std::runtime_error("comparbale not equal with different types not allowes");
}





#endif //DATACONTAINER_STORAGE_OBJECT_COMPARATOR_IMPL_H
