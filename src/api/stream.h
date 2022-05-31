#pragma once
template<class T>
class stream {
public:
    stream() {}
    virtual ~stream(){}
    stream(const stream & rhs){}
    stream(stream && stream){}
    stream& operator=(stream & rhs){
        return *this;
    }
    stream& operator=(stream && rhs){
        return *this;
    }
    virtual T  next() = 0;

};



