//
// Created by cefour on 11.06.22.
//

#ifndef DATACONTAINER_IF_NODE_H
#define DATACONTAINER_IF_NODE_H
#include "ast_node.h"

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
#endif //DATACONTAINER_IF_NODE_H
