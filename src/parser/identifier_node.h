//
// Created by cefour on 08.06.22.
//

#ifndef DATACONTAINER_IDENTIFIER_NODE_H
#define DATACONTAINER_IDENTIFIER_NODE_H
#include "ast_node.h"

class identifier_node : public ast_node
{
public:
    identifier_node(const std::string & name) : name(name){
    }

    evaluation_result evaluate(chakra::storage_table & tbl) {
        UNUSED(tbl);
        return name;
    }

private:
    std::string name;
};

#endif //DATACONTAINER_IDENTIFIER_NODE_H
