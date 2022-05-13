//
// Created by cefour on 13.05.22.
//

#ifndef DATACONTAINER_CRTP_H
#define DATACONTAINER_CRTP_H
#include <string>
#include <exception>


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
    template<class Y>
    T operator+(const Y & other) const{
        std::string_view s1(T::PARAMETER::tag);
        std::string_view s2(Y::PARAMETER::tag);
        if (s1 == s2){
            return T(this->underlying().get() + other.get());
        }
        throw std::exception();
    }
    template<class Y>
    T& operator+=(const Y & other){
        std::string_view s1(T::PARAMETER::tag);
        std::string_view s2(Y::PARAMETER::tag);
        if (s1 == s2) {
            this->underlying().get() += other.get();
            return this->underlying();
        }
        throw std::exception();
    }

};

template <typename T>
concept PARA =
requires(T m) {  // any `m` that is a Machine
    T::tag;
};

template<class T,class Y>
    requires (PARA<T>,PARA<Y>)
Y operator+(const Y & other ,const T & operand) {
    std::string_view s1(T::PARAMETER::tag);
    std::string_view s2(Y::PARAMETER::tag);
    if (s1 == s2) {
        return Y{other.get() + operand.get()};
    }
    throw std::exception();
}



#endif //DATACONTAINER_CRTP_H
