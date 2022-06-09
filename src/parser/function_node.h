//
// Created by cefour on 08.06.22.
//

#ifndef DATACONTAINER_FUNCTION_NODE_H
#define DATACONTAINER_FUNCTION_NODE_H
#include "ast_node.h"



class function_node : public ast_node
{
public:
    function_node(const std::string & func_name, const ASTNodePtr & args) : function_name(func_name), args(args){
    }

    evaluation_result evaluate(chakra::storage_table & tbl);

private:
    std::string function_name;
    ASTNodePtr args;
};
#endif //DATACONTAINER_FUNCTION_NODE_H
