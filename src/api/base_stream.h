#pragma once
#include "macro_helper.h"

template<class T>
class base_stream {
public:
    base_stream() =default;
    virtual T  next() = 0;

};



