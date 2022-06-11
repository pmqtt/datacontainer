//
// Created by cefour on 10.06.22.
//

#ifndef DATACONTAINER_AST_TYPE_HELPERS_H
#define DATACONTAINER_AST_TYPE_HELPERS_H
#include "ast_node.h"

struct argument_result_visitor{
    arg_result operator()(const bool x){
        return x;
    }
    arg_result operator()(const double x){
        return x;
    }
    arg_result operator()(const std::string& x){
        return x;
    }
    arg_result operator()(const empty_result & x){
        return x;
    }
    arg_result operator()(const std::vector<arg_result> & x){
        UNUSED(x);
        throw std::runtime_error("not valid cast");
    }
};

struct to_string_visitor{
    std::string operator()(const bool &x){
        if(x) return "true;";
        return "false";
    }
    std::string  operator()(const double &x){
        return std::to_string(x);
    }
    std::string  operator()(const std::string &x){
        return x;
    }
    std::string  operator()(const empty_result & x){
        UNUSED(x);
        return "";
    }

    std::string  operator()(const std::vector<arg_result> & x){
        UNUSED(x);
        throw std::runtime_error("std::vector<arg_result> not possible to cast into string");
    }
};

struct result_visitor{
    std::string input;
    void operator()(bool x){
        std::cout << std::boolalpha<<"evaluate( "<<std::quoted(input)<< " ) = " << x << std::endl;
    }
    void operator()(double x){
        std::cout << std::boolalpha<<"evaluate( "<<std::quoted(input)<< " ) = " << x << std::endl;
    }
    void operator()(std::string x){
        std::cout << std::boolalpha<<"evaluate( "<<std::quoted(input)<< " ) = " << std::quoted(x) << std::endl;
    }
    void operator()(empty_result x){
        UNUSED(x);
        std::cout << std::boolalpha<<"evaluate( "<<std::quoted(input)<< " ) = "<< std::endl;
    }
    void operator()(std::vector<arg_result> & x){
        UNUSED(x);
        std::cout << std::boolalpha<<"evaluate( "<<std::quoted(input)<< " ) = " << "std::vector<arg_result>" << std::endl;
    }
};

namespace ptl{
    arg_result cast(const evaluation_result & res);
    std::string to_string(const evaluation_result & res);
}


#endif //DATACONTAINER_AST_TYPE_HELPERS_H
