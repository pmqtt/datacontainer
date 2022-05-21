#ifndef HASH_STREAM_H
#define HASH_STREAM_H

#include "PrimeStream.h"
class HashStream : public Stream<int>
{
public:
    HashStream();
    ~HashStream();
    HashStream(const HashStream & rhs);
    HashStream & operator=(const HashStream & rhs);
    HashStream(HashStream && rhs);
    HashStream & operator=(HashStream && rhs);
    // Geerbt �ber Stream
    virtual int next();
#if defined(_MSC_VER)
    static constexpr int PRIME_ARRAY[] = {
            1009, 1013, 4289, 4297, 6863, 6869, 7841, 7853, 7867, 7873,
            8009, 8117, 9013, 9029, 9967, 9973, 25801, 25819, 50767, 50773,
            109619, 109621, 175837, 175843, 296771, 296773, 610651, 610661,
            999961, 999979 };
#else
    static int PRIME_ARRAY[];
#endif
private:
    PrimeStream stream;
    int currentIndex;
};

#endif