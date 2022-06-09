//
// Created by cefour on 08.06.22.
//
#include "../api/macro_helper.h"
#include "function_node.h"
#include <iostream>
std::map<std::string,std::function<evaluation_result(const std::vector<arg_result> & arguments)>> built_in_functions{
        {"constante",[](const std::vector<arg_result> & arguments) -> evaluation_result{
            UNUSED(arguments);
            return 3.0;
        }},
        {"identity",[](const std::vector<arg_result> & arguments) -> evaluation_result{
            return std::visit([](auto & x){
                evaluation_result res =x;
                return res;
            },arguments[0]);
        }},
        {"mean",[](const std::vector<arg_result> & arguments) -> evaluation_result{
            return std::visit([](auto & x){
                UNUSED(x);
                evaluation_result res = "HALLO WELT";
                return res;
            },arguments[0]);
        }},
};

evaluation_result function_node::evaluate(chakra::storage_table & tbl) {
    UNUSED(tbl);
    try {
        auto arguments = args->evaluate(tbl);
        return built_in_functions[function_name](std::get<std::vector<arg_result>>(arguments));
    }catch(std::bad_function_call & e){
        std::cout<<"really bad function: "<<function_name<<std::endl;
        throw e;
    }
}