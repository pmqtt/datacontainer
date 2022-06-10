//
// Created by cefour on 08.06.22.
//
#include "argument_node.h"
#include <stdexcept>
#include "ast_type_helpers.h"

evaluation_result argument_node::evaluate(chakra::storage_table & tbl) {
    auto first_argument = arg->evaluate(tbl);
    bool is_empty = std::holds_alternative<empty_result>(first_argument);
    std::vector<arg_result> result;
    if( is_empty ){
        empty_result tmp;
        result.push_back(tmp);
        return result;
    }
    argument_result_visitor vis;
    arg_result casted_argument = std::visit(vis,first_argument);
    result.push_back(casted_argument);


    for(auto & argument: args){
        auto iter_argument = argument->evaluate(tbl);
        casted_argument = std::visit(vis,iter_argument);
        result.push_back(casted_argument);
    }

    return result;
}