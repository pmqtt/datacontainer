//
// Created by cefour on 13.05.22.
//

#ifndef DATACONTAINER_CRTP_H
#define DATACONTAINER_CRTP_H

#include <string>
#include <exception>
#include "macro_helper.h"

template<typename ... Ts>
struct Overload : Ts ... {
    using Ts::operator() ...;
};
template<class... Ts> Overload(Ts...) -> Overload<Ts...>;




template<class X, class Y>
struct is_convertable {
    static constexpr bool value = std::is_same<X, Y>::value;

    constexpr bool test() {
        if constexpr (std::is_same<X, Y>::value) return true;
        return false;
    }

    static X convert(Y &&rhs) {
        return static_cast<X>(rhs);
    }
};

#define CONVERTABLE_BOTH(X, Y) \
    template<> struct is_convertable<X,Y> { static constexpr bool value = true; }; \
    template<> struct is_convertable<Y,X> { static constexpr bool value = true;};


template<typename T,typename X>
concept Is_Convertable = is_convertable<typename T::PARAMETER,typename X::PARAMETER>::value;

template<typename T, template<typename> class crtpType>
struct crtp {
    constexpr T &underlying() {
        return static_cast<T &>(*this);
    }

    constexpr T const &underlying() const {
        return static_cast<T const &>(*this);
    }
};


template<typename T>
struct pre_incrementable : crtp<T, pre_incrementable> {
    T &operator++() {
        ++this->underlying().get();
        return this->underlying();
    }

};

template<class T>
struct addable : crtp<T, addable> {
    addable() = default;

    explicit addable(const T &x) {
        this->underlying().get() = x.underlying().get();
    }

    template<Is_Convertable<T> Y>
    T operator+(const Y &other) const {
        return T(this->underlying().get() + other.get());
    }

    template<Is_Convertable<T> Y>
    T & operator+=(const Y &other){
        this->underlying().get() += other.get();
        return this->underlying();
    }
};

template<class T>
struct stringable : crtp<T, stringable> {
    stringable() = default;

    explicit stringable(const T &x) {
        this->underlying().get() = x.underlying().get();
    }

    [[nodiscard]] std::string to_string() const {
        return std::to_string(this->underlying().get());
    }
};


template<class T, class Y>
requires(is_convertable<typename T::PARAMETER, typename Y::PARAMETER>::value)
Y operator+(const Y &other, const T &operand) {
    return Y{is_convertable<typename Y::TYPE, typename T::TYPE>::convert(other.get() + operand.get())};
}

template<class T, class Y>
requires(is_convertable<typename T::PARAMETER, typename Y::PARAMETER>::value)
Y operator-(const Y &other, const T &operand) {
    return Y{is_convertable<typename Y::TYPE, typename T::TYPE>::convert(other.get() - operand.get())};
}


#define CREATE_STRONG_TYPE(NAME, TYPE) \
    struct NAME ## _PARAMETER { }; \
    using NAME = NamedType<TYPE,NAME ## _PARAMETER>

#define CREATE_STRONG_FIELD_TYPE(NAME, TYPE) \
    struct NAME ## _PARAMETER { }; \
    using NAME = named_type<TYPE,NAME ## _PARAMETER,addable,stringable>


#endif //DATACONTAINER_CRTP_H
