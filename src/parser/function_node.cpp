//
// Created by cefour on 08.06.22.
//
#include "function_node.h"
std::map<std::string,std::function<evaluation_result(const std::vector<arg_result> & arguments)>> built_in_functions{
        {"constante",[](const std::vector<arg_result> & arguments) -> evaluation_result{
            return 3.0;
        }},
        {"identity",[](const std::vector<arg_result> & arguments) -> evaluation_result{
            return std::visit([](auto & x){
                evaluation_result res =x;
                return res;
            },arguments[0]);
        }}
};

evaluation_result function_node::evaluate() {
    auto arguments = args->evaluate();
    return built_in_functions[function_name](std::get<std::vector<arg_result>>(arguments));
}