//
// Created by cefour on 10.06.22.
//

#include "ast_type_helpers.h"

namespace ptl {

    arg_result cast(const evaluation_result &res){
        return std::visit(argument_result_visitor(),res);
    }

    std::string to_string(const evaluation_result & res){
        return std::visit(to_string_visitor(),res);
    }


}