#ifndef PRIME_STREAM_H
#define PRIME_STREAM_H
#include "Stream.h"
#include <cmath>

class PrimeStream : public Stream<int> {
public:
    PrimeStream(int start = 7);
    PrimeStream(const PrimeStream & rhs);
    PrimeStream & operator=(const PrimeStream & rhs);
    PrimeStream(PrimeStream && rhs);
    PrimeStream & operator=(PrimeStream && rhs);
public:
    virtual int  next();
private:
    int currentValue;
};





#endif
