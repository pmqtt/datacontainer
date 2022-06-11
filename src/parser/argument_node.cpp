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

    result.push_back(ptl::cast(first_argument));


    for(auto & argument: args){
        auto iter_argument = argument->evaluate(tbl);
        result.push_back(ptl::cast(iter_argument));
    }

    return result;
}