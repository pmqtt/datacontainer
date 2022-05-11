//
// Created by cefour on 03.05.22.
//

#ifndef DATACONTAINER_MQTTMESSENGEREXCEPTION_H
#define DATACONTAINER_MQTTMESSENGEREXCEPTION_H

#include <string>
#include <exception>

class MqttMessengerException : public std::exception {
public:
#if defined (__GNUC__) || defined (__GUNG__)
    MqttMessengerException(const std::string & message) : std::exception() {
        this->msg = message;
    }
    const char * what() const noexcept {
        return this->msg.c_str();
    }

#else
    MqttMessengerException(const std::string & message) : std::exception(message.c_str()) {}
    const char * what() const {
        return exception::what();
    }
#endif
private:
    std::string msg;

};


class MqttDisconnectException : public std::exception {
public:
#if defined (__GNUC__) || defined (__GUNG__)
    MqttDisconnectException(const std::string & message) : std::exception() {
        this->msg = message;
    }
    const char * what() const noexcept {
        return this->msg.c_str();
    }

#else
    MqttDisconnectException(const std::string & message) : std::exception(message.c_str()) {}
    const char * what() const {
        return exception::what();
    }
#endif
private:
    std::string msg;

};



#endif //DATACONTAINER_MQTTMESSENGEREXCEPTION_H
