//
// Created by cefour on 11.05.22.
//

#ifndef DATACONTAINER_TYPEDEFEXCEPTION_H
#define DATACONTAINER_TYPEDEFEXCEPTION_H

#include <string>
#include <exception>

class TypedefException : public std::exception {
public:
#if defined (__GNUC__) || defined (__GUNG__)
    TypedefException(const std::string & message) : std::exception() {
        this->msg = message;
    }
    const char * what() const noexcept {
        return this->msg.c_str();
    }

#else
    TypedefException(const std::string & message) : std::exception(message.c_str()) {}
    const char * what() const {
        return exception::what();
    }
#endif
private:
    std::string msg;

};

#endif //DATACONTAINER_TYPEDEFEXCEPTION_H
