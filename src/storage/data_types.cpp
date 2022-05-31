#include "data_types.h"


bool data_type::operator<(const data_type & rhs){
    return this->hash() < rhs.hash();
}

bool data_type::operator==(const data_type &rhs){
    return this->hash() == rhs.hash();
}


