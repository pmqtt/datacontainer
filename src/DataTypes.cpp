#include "DataTypes.h"


bool DataType::operator<(const DataType & rhs){
    return this->hash() < rhs.hash();
}

bool DataType::operator==(const DataType &rhs){
    return this->hash() == rhs.hash();
}


