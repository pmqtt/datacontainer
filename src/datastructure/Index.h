//
// Created by cefour on 19.05.22.
//

#ifndef DATACONTAINER_INDEX_H
#define DATACONTAINER_INDEX_H
#include "Table.h"

struct Index{
    virtual ~Index(){}
};


template<class KEY,class VALUE>
struct UniqueIndex : public Index{
    ~UniqueIndex(){}
private:
    Table<KEY,VALUE> table;
};

template<class KEY,class VALUE>
struct DateTimeIndex : public Index{
    ~DateTimeIndex(){}
private:
    Table<KEY,VALUE> table;
};

template<class KEY,class VALUE>
struct IntervalIndex : public Index{
    ~IntervalIndex(){}
private:
    Table<KEY,VALUE> table;
};

#endif //DATACONTAINER_INDEX_H
