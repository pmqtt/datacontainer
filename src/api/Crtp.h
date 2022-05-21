//
// Created by cefour on 13.05.22.
//

#ifndef DATACONTAINER_CRTP_H
#define DATACONTAINER_CRTP_H
#include <string>
#include <exception>
#include "MacroHelper.h"

template<class X,class Y>
struct isConvertable {
    static constexpr bool value = std::is_same<X, Y>::value;
    constexpr bool test(){
        if constexpr (std::is_same<X,Y>::value) return true;
        return false;
    }
    static X convert( Y && rhs ){
        return static_cast<X>(rhs);
    }
};

#define CONVERTABLE_BOTH(X,Y) \
    template<> struct isConvertable<X,Y> { static constexpr bool value = true; }; \
    template<> struct isConvertable<Y,X> { static constexpr bool value = true;};





template <typename T, template <typename> class crtpType>
struct Crtp{
    constexpr T& underlying(){
        return static_cast<T&>(*this);
    }
    constexpr T const& underlying() const{
        return static_cast<T const&>(*this);
    }
};


template <typename T>
struct PreIncrementable: Crtp<T, PreIncrementable>{
    T& operator++(){
        ++this->underlying().get();
        return this->underlying();
    }

};

template<class T>
struct Addable: Crtp<T, Addable >{
    Addable() = default;
    Addable(const T &x){
        this->underlying().get() = x.underlying().get();
    }
    template<class Y> requires (isConvertable<typename T::PARAMETER,typename Y::PARAMETER>::value)
    T operator+(const Y & other) const{
            return T(this->underlying().get() + other.get());
    }
    template<class Y> requires (isConvertable<typename T::PARAMETER,typename Y::PARAMETER>::value)
    T& operator+=(const Y & other){
            this->underlying().get() += other.get();
            return this->underlying();
    }
};

template<class T>
struct Stringable : Crtp<T,Stringable>{
    Stringable() = default;
    Stringable(const T &x){
        this->underlying().get() = x.underlying().get();
    }
    std::string toString()const{
        return std::to_string(this->underlying().get());
    }
};



template<class T,class Y> requires (isConvertable<typename T::PARAMETER,typename Y::PARAMETER>::value)
Y operator+(const Y & other ,const T & operand) {
    return Y{isConvertable<typename Y::TYPE,typename T::TYPE>::convert( other.get() + operand.get() )};
}

template<class T,class Y> requires (isConvertable<typename T::PARAMETER,typename Y::PARAMETER>::value)
Y operator-(const Y & other ,const T & operand) {
    return Y{isConvertable<typename Y::TYPE,typename T::TYPE>::convert( other.get() - operand.get() )};
}


#define CREATE_STRONG_TYPE(NAME,TYPE) \
    struct NAME ## _PARAMETER { }; \
    using NAME = NamedType<TYPE,NAME ## _PARAMETER>

#define CREATE_STRONG_FIELD_TYPE(NAME,TYPE) \
    struct NAME ## _PARAMETER { }; \
    using NAME = NamedType<TYPE,NAME ## _PARAMETER,Addable,Stringable>










#endif //DATACONTAINER_CRTP_H
