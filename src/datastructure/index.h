//
// Created by cefour on 19.05.22.
//

#ifndef DATACONTAINER_INDEX_H
#define DATACONTAINER_INDEX_H
#include "table.h"

struct index{
    virtual ~index(){}
};


template<class KEY,class VALUE>
struct unique_index : public index{
    ~unique_index(){}
private:
    table<KEY,VALUE> table;
};

template<class KEY,class VALUE>
struct date_time_index : public index{
    ~date_time_index(){}
private:
    table<KEY,VALUE> table;
};

template<class KEY,class VALUE>
struct interval_index : public index{
    ~interval_index(){}
private:
    table<KEY,VALUE> table;
};

#endif //DATACONTAINER_INDEX_H
