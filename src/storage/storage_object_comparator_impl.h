//
// Created by cefour on 01.06.22.
//

#ifndef DATACONTAINER_STORAGE_OBJECT_COMPARATOR_IMPL_H
#define DATACONTAINER_STORAGE_OBJECT_COMPARATOR_IMPL_H


template<class X, class C1, class Y, class C2>
bool storage_cmp_greater(const storage_object<X, C1> &x, const storage_object<Y, C2> &y) {
    using x_comp = typename X::comparble;
    using y_comp = typename Y::comparble;
    if constexpr (!(std::is_same<x_comp, bool_comparable>::value || std::is_same<y_comp, bool_comparable>::value)) {
        return x.get_value() > y.get_value();
    }
    throw std::runtime_error("bool type can only check for equality");
}

template<class X, class C1, class Y, class C2>
bool storage_cmp_greater_equal(const storage_object<X, C1> &x, const storage_object<Y, C2> &y) {
    using x_comp = typename X::comparble;
    using y_comp = typename Y::comparble;
    if constexpr (!(std::is_same<x_comp, bool_comparable>::value || std::is_same<y_comp, bool_comparable>::value)) {
        return x.get_value() >= y.get_value();
    }
    throw std::runtime_error("bool type can only check for equality");
}

template<class X, class C1, class Y, class C2>
bool storage_cmp_less(const storage_object<X, C1> &x, const storage_object<Y, C2> &y) {
    using x_comp = typename X::comparble;
    using y_comp = typename Y::comparble;
    if constexpr (!(std::is_same<x_comp, bool_comparable>::value || std::is_same<y_comp, bool_comparable>::value)) {
        return x.get_value() < y.get_value();
    }
    throw std::runtime_error("bool type can only check for equality");
}

template<class X, class C1, class Y, class C2>
bool storage_cmp_less_equal(const storage_object<X, C1> &x, const storage_object<Y, C2> &y) {
    using x_comp = typename X::comparble;
    using y_comp = typename Y::comparble;
    if constexpr (!(std::is_same<x_comp, bool_comparable>::value || std::is_same<y_comp, bool_comparable>::value)) {
        return x.get_value() <= y.get_value();
    }
    throw std::runtime_error("bool type can only check for equality");
}

template<class X, class C1, class Y, class C2>
bool storage_cmp_equal(const storage_object<X, C1> &x, const storage_object<Y, C2> &y) {
    return x.get_value() == y.get_value();
}

template<class X, class C1, class Y, class C2>
bool storage_cmp_not_equal(const storage_object<X, C1> &x, const storage_object<Y, C2> &y) {
    return x.get_value() != y.get_value();
}



#endif //DATACONTAINER_STORAGE_OBJECT_COMPARATOR_IMPL_H
