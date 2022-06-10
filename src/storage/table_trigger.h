//
// Created by cefour on 07.06.22.
//

#ifndef DATACONTAINER_TABLE_OBSERVER_H
#define DATACONTAINER_TABLE_OBSERVER_H

#include "storage.h"
#include "../parser/ast_node.h"
#include <functional>


struct table_trigger{
    ASTNodePtr condition;
    std::vector<ASTNodePtr> preparation;
    std::map<std::string,evaluation_result> value_map;

    table_trigger(const ASTNodePtr & cond, const std::vector<ASTNodePtr> & prep );

    virtual ~table_trigger();
    virtual bool trigger(chakra::storage_table & tbl);
    virtual void execute(chakra::storage_table & tbl);
    virtual std::string get_message(const std::string & message_template);
};





#endif //DATACONTAINER_TABLE_OBSERVER_H
