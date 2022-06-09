//
// Created by cefour on 08.06.22.
//

#ifndef DATACONTAINER_ARGUMENT_NODE_H
#define DATACONTAINER_ARGUMENT_NODE_H
#include "ast_node.h"


class argument_node : public ast_node
{
public:
    argument_node(const ASTNodePtr & a, const std::vector<ASTNodePtr> &args) : arg(a), args(args){
    }

    evaluation_result evaluate(chakra::storage_table & tbl);

private:
    ASTNodePtr  arg;
    std::vector<ASTNodePtr> args;
};


#endif //DATACONTAINER_ARGUMENT_NODE_H
