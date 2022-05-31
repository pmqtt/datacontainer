//
// Created by cefour on 04.05.22.
//

#ifndef DATACONTAINER_FORMAT_H
#define DATACONTAINER_FORMAT_H
#include "format_type.h"
#include "../storage/data_types.h"

#include <string>
#include <vector>
#include <memory>
#include <cstdio>
#include <algorithm>
#include <map>




using format_specifier =  std::map<char,std::shared_ptr<format_type>>;

namespace {
    format_specifier TYPE_INPUT_FORMAT = {
            {'r', std::make_shared<format_type_real>()},
            {'s', std::make_shared<format_type_string>()}
    };
}
class format {
private:
    std::string format_def;
    std::vector<std::shared_ptr<format_type>> format_list;
    format_specifier format_specifier_list;

public:
    format(){}
    format(const std::string & format, const format_specifier & spec = TYPE_INPUT_FORMAT ): format_def(format), format_specifier_list(spec) { parse();}

public:
    auto get_formate_list()const { return this->format_list;}
    bool interpret(const std::string & x);
    std::vector<std::shared_ptr<format_type>> get_argument_list(){
        std::vector<std::shared_ptr<format_type>> arguments;
        std::copy_if(this->format_list.begin(), this->format_list.end(),
                     std::back_inserter(arguments), [](const auto & val )->bool { return val->is_argument(); });
        return arguments;
    }

    std::map<std::string,std::shared_ptr<format_type>> get_arguments_map()const{
        std::map<std::string,std::shared_ptr<format_type>> arguments;
        unsigned short index = 1;
        for(auto & iter : format_list){
            if(iter->is_argument()) {
                std::string key ="$";
                key += std::to_string(index);
                arguments[key] = iter;
                index = index +1;
            }
        }
        return arguments;
    }

private:
    void parse();

};

#endif //DATACONTAINER_FORMAT_H
