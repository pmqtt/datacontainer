//
// Created by cefour on 08.06.22.
//

#ifndef DATACONTAINER_AST_NODE_H
#define DATACONTAINER_AST_NODE_H


#include "../storage/storage.h"

#include <boost/optional.hpp>

#include <functional>
#include <map>
#include <string>
#include <variant>
#include <vector>

struct empty_result{};

using arg_result = std::variant<bool,double,std::string,empty_result>;
using evaluation_result = std::variant<bool,double,empty_result,std::string,std::vector<arg_result>>;



class ast_node
{
public:
    virtual evaluation_result evaluate(chakra::storage_table & tbl) = 0;
    virtual ~ast_node() { }
};

using ASTNodePtr = std::shared_ptr<ast_node>;


struct if_node : public ast_node{
    if_node(const ASTNodePtr & clause, const ASTNodePtr & then_block, const boost::optional< ASTNodePtr> & else_block)
        : clause(clause), then_block(then_block), else_block(else_block){
    }
    evaluation_result evaluate(chakra::storage_table & tbl){
        auto bool_expression = clause->evaluate(tbl);
        if(std::get<bool>(bool_expression)){
            return then_block->evaluate(tbl);
        }else{
            if(else_block) {
                return (*else_block)->evaluate(tbl);
            }
            return -1.0;
        }
    }
    ASTNodePtr clause;
    ASTNodePtr then_block;
    boost::optional< ASTNodePtr > else_block;

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














#endif //DATACONTAINER_AST_NODE_H
