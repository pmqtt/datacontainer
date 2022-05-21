//
// Created by cefour on 18.05.22.
//

#ifndef DATACONTAINER_EXCEPTIONMACRO_H
#define DATACONTAINER_EXCEPTIONMACRO_H
#include <exception>
#include <string>

#if defined (__GNUC__) || defined (__GUNG__)
    #define CREATE_EXCEPTION_CLASS(CLNAME) \
        class CLNAME : public std::exception { \
        public:\
        CLNAME(const std::string & message) : std::exception() { \
                this->msg = message;\
            }\
            const char * what() const noexcept {\
                return this->msg.c_str();\
            }\
        private:\
            std::string msg;\
        };
#define CREATE_EXCEPTION_CLASS_DERIVATED_FROM(CLNAME,DERIVATED) \
        class CLNAME : public DERIVATED { \
        public:\
        CLNAME(const std::string & message) : DERIVATED(message) { \
                this->msg = message;\
        }\
        const char * what() const noexcept {\
            return this->msg.c_str();\
        }\
        private:\
            std::string msg;\
        };
#else
    #define CREATE_EXCEPTION_CLASS(CLNAME) \
        class CLNAME : public std::exception { \
        public: \
            CLNAME(const std::string & message) : std::exception(message.c_str()) {} \
            const char * what() const noexcept { \
                return this->msg.c_str(); \
            }\
            const char * what() const { \
                return exception::what(); \
            }
        private: \
            std::string msg;\
        };

    #define CREATE_EXCEPTION_CLASS_DERIVATED_FROM(CLNAME,DERIVATED) \
        class CLNAME : public DERIVATED { \
        public:\
        CLNAME(const std::string & message) : std::DERIVATED(message.c_str()) { \
                this->msg = message;\
        }\
        const char * what() const noexcept {\
            return this->msg.c_str();\
        }\
        private:\
            std::string msg;\
        };

#endif

#endif //DATACONTAINER_EXCEPTIONMACRO_H
