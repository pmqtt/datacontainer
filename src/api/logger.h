//
// Created by cefour on 18.05.22.
//

#ifndef DATACONTAINER_LOGGER_H
#define DATACONTAINER_LOGGER_H
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <ctime>   // localtime
#include <sstream> // stringstream
#include <iomanip> // put_time
#include <thread>
#include <algorithm>
#include <source_location>
#include <cstdlib>
#include <sstream>
#include <utility>
#include "date_time.h"
#include "macro_helper.h"



namespace logger {

    CREATE_STRINGABLE_ENUM(LEVEL,TRACE,INFO,DEBUG,WARNING,ERROR,FATAL)

    namespace configuration {
        static std::string timestamp_format = "%Y-%m-%d %h:%i:%s.%l";
        static LEVEL log_level = LEVEL::TRACE;
    }


    class logger {
    public:
        logger(){
            max_level = configuration::log_level;
        }
        void set_log_level(LEVEL l){
            max_level = l;
        }
        void log(LEVEL l, std::string &&msg, const std::source_location &location = std::source_location::current()) {
            if(l >= max_level) {
                if(l >= max_level) {
                    color_shell(l);
                    std::cout << create_output_stream(l, std::forward<std::string>(msg), location).str();
                }
            }
        }

        void log(LEVEL l, const std::string &msg, const std::source_location &location = std::source_location::current()) {
            if(l >= max_level) {
                color_shell(l);
                std::cout << create_output_stream(l, msg, location).str();
            }
        }

    private:
        std::stringstream  create_output_stream(LEVEL l, const std::string & msg, const std::source_location &location){
            date_time date = date_time::timestamp();
            date.set_format(configuration::timestamp_format);
            std::stringstream stream;
            std::string user = std::getenv("USER");
            stream << "[ " << LEVELtoString(l) << " ] "
                   << "[ " << date.to_string() << " ] "
                   << "[ " << std::hex << std::this_thread::get_id() << ", " << user << " ] "
                   << "[ " << location.function_name() << " ] "
                   << msg
                   << "\033[0m\n";
            return stream;
        }


        void color_shell(LEVEL l) {
            std::vector<std::string> vec = {
                    "\033[0m",
                    "\033[0m",
                    "\033[0;32m",
                    "\033[0;36m",
                    "\033[0;33m",
                    "\033[1;31m",
            };
            std::cout << vec[l];

        }

        std::hash<std::thread::id> hasher;
        LEVEL max_level;




    };


}

#endif //DATACONTAINER_LOGGER_H
