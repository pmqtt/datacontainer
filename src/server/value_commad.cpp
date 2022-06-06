//
// Created by cefour on 05.06.22.
//

#include "../api/date_time.h"
#include "../exceptions/value_command_exception.h"
#include "value_command.h"


#include <cstring>


#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <regex>
#include <string>



static const std::map<std::string,std::function<std::shared_ptr<value_command>(const std::string & )>> commands{
        {"ink_index",[](const std::string & str)->std::shared_ptr<value_command>{
            const std::string_view command_text = "ink_index";
            std::size_t len = command_text.length();
            std::string digit;
            bool expected_clamps_up = true;
            bool clamps_close = false;
            for(std::size_t i = len; i < str.length(); ++i){
                if(::isblank(str[i])) continue;
                if(::isdigit(str[i])){
                    if(expected_clamps_up){
                        throw value_command_exception("expected ( after ink_index");
                    }
                    if(clamps_close){
                        value_command_exception("expected ink_index( number )");
                    }
                    digit += str[i];
                }
                if(::isalpha(str[i])){
                    throw value_command_exception("expected ink_index( number )");
                }
                if(str[i] == '('){
                    expected_clamps_up = false;
                    continue;
                }
                if(str[i] == ')'){
                    clamps_close = true;
                }
            }
            if(digit.empty()){
                throw value_command_exception("expected ink_index( number ); number is not set ");
            }
            return std::make_shared<value_increment_index_command>(digit);

        }},
        {"timestamp",[](const std::string & str)->std::shared_ptr<value_command>{
            UNUSED(str);
            date_time dt = date_time::timestamp();
            dt.set_format("%Y-%m-%d %h:%i:%s.%l");
            return std::make_shared<value_timestamp_command>(dt.to_string());
        }}
};

static const std::vector<std::string> command_pallete ={"ink_index","timestamp"};

std::optional<std::shared_ptr<value_command>> make_value_command(const std::string & str,const std::string &format_def) {
    for (const auto &x: command_pallete) {
        if (str.starts_with(x)) {
            if(commands.count(x) > 0) {
                return commands.at(x)(str);
            }
        }
    }

    format fmt(format_def);
    return std::make_shared<value_argument_command>(fmt,str);

    return {};
}