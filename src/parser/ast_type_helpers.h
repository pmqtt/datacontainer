//
// Created by cefour on 10.06.22.
//

#ifndef DATACONTAINER_AST_TYPE_HELPERS_H
#define DATACONTAINER_AST_TYPE_HELPERS_H
#include "ast_node.h"

struct argument_result_visitor{
    arg_result operator()(bool x){
        return x;
    }
    arg_result operator()(double x){
        return x;
    }
    arg_result operator()(std::string x){
        return x;
    }
    arg_result operator()(empty_result x){
        return x;
    }
    arg_result operator()(std::vector<arg_result> & x){
        UNUSED(x);
        throw std::runtime_error("not valid cast");
    }
};


struct to_string_visitor{
    std::string operator()(bool x){
        if(x) return "true;";
        return "false";
    }
    std::string  operator()(double x){
        return std::to_string(x);
    }
    std::string  operator()(std::string x){
        return x;
    }
    std::string  operator()(empty_result & x){
        UNUSED(x);
        return "";
    }

    std::string  operator()(std::vector<arg_result> & x){
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



#endif //DATACONTAINER_AST_TYPE_HELPERS_H
