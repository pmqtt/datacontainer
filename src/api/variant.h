//
// Created by cefour on 15.06.22.
//

#ifndef DATACONTAINER_VARIANT_H
#define DATACONTAINER_VARIANT_H
#include <variant>
namespace ptl{

    template<class T,class U>
    bool holds_any_alternative(const T & var){
        return std::holds_alternative<U>(var);
    }

    template<class T,class ...ARGS>
    bool holds_any_alternative(const T & var){
        return true || holds_any_alternative<T,ARGS...>(var);
    }
}

#endif //DATACONTAINER_VARIANT_H
