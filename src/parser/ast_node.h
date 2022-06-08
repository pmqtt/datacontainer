//
// Created by cefour on 08.06.22.
//

#ifndef DATACONTAINER_AST_NODE_H
#define DATACONTAINER_AST_NODE_H

#include <functional>
#include <map>
#include <string>
#include <variant>
#include <vector>


using arg_result = std::variant<bool,double,std::string>;
using evaluation_result = std::variant<bool,double,std::string,std::vector<arg_result>>;


class ast_node
{
public:
    virtual evaluation_result evaluate() = 0;
    virtual ~ast_node() { }
};

using ASTNodePtr = ast_node*;



















#endif //DATACONTAINER_AST_NODE_H
