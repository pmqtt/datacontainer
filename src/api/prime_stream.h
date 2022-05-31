#ifndef PRIME_STREAM_H
#define PRIME_STREAM_H
#include "stream.h"
#include <cmath>

class prime_stream : public stream<int> {
public:
    prime_stream(int start = 7);
    prime_stream(const prime_stream & rhs);
    prime_stream & operator=(const prime_stream & rhs);
    prime_stream(prime_stream && rhs);
    prime_stream & operator=(prime_stream && rhs);
public:
    virtual int  next();
private:
    int current_value;
};





#endif
