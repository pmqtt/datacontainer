#pragma once
template<class T>
class Stream {
public:
    Stream() {}
    virtual ~Stream(){}
    Stream(const Stream & rhs){}
    Stream(Stream && stream){}
    Stream& operator=(Stream & rhs){
        return *this;
    }
    Stream& operator=(Stream && rhs){
        return *this;
    }
    virtual T  next() = 0;

};



