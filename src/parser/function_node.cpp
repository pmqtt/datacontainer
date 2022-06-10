//
// Created by cefour on 08.06.22.
//
#include "../api/macro_helper.h"
#include "function_node.h"
#include <iostream>
std::map<std::string,std::function<evaluation_result(chakra::storage_table & ,const std::vector<arg_result> & arguments)>> built_in_functions{
        {"constante",[](chakra::storage_table & tbl,const std::vector<arg_result> & arguments) -> evaluation_result{
            UNUSED(arguments);
            UNUSED(tbl);
            return 3.0;
        }},
        {"identity",[](chakra::storage_table & tbl,const std::vector<arg_result> & arguments) -> evaluation_result{
            UNUSED(tbl);
            return std::visit([](auto & x){
                evaluation_result res =x;
                return res;
            },arguments[0]);
        }},
        {"mean",[](chakra::storage_table & tbl,const std::vector<arg_result> & arguments) -> evaluation_result{
            return std::visit([&](auto & x){
                UNUSED(x);
                double result = 0;
                int count = 0;
                std::cout<<"mean parameter:"<<std::get<std::string>(arguments[0])<<std::endl;
                tbl.aggregate_table(std::get<std::string>(arguments[0]), [&](base_storage_object &type) {
                    if (std::holds_alternative<storage_real>(type)) {
                        result += std::get<storage_real>(type).get_value();
                        count++;
                    }
                });
                double mean = result / count;

                evaluation_result res = mean;
                return res;
            },arguments[0]);
        }},
        {"count",[](chakra::storage_table & tbl,const std::vector<arg_result> & arguments) -> evaluation_result{
            return std::visit([&](auto & x){
                UNUSED(x);
                int count = 0;
                std::cout<<"count parameter:"<<std::get<std::string>(arguments[0])<<std::endl;
                tbl.aggregate_table(std::get<std::string>(arguments[0]), [&](base_storage_object &type) {
                    UNUSED(type);
                    count++;
                });

                evaluation_result res = (double)count;
                return res;
            },arguments[0]);
        }}
};

evaluation_result function_node::evaluate(chakra::storage_table & tbl) {
    UNUSED(tbl);
    try {
        std::cout<<"evaluate:";
        auto arguments = args->evaluate(tbl);
        return built_in_functions[function_name](tbl,std::get<std::vector<arg_result>>(arguments));
    }catch(std::bad_function_call & e){
        std::cout<<"really bad function: "<<function_name<<std::endl;
        throw e;
    }catch(std::bad_variant_access &e){
        std::cout<<"really bad variant access";
        throw e;
    }
}