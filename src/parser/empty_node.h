//
// Created by cefour on 08.06.22.
//

#ifndef DATACONTAINER_EMPTY_NODE_H
#define DATACONTAINER_EMPTY_NODE_H

#include "ast_node.h"

class empty_node : public ast_node
{
public:
    empty_node(){

    }
    evaluation_result evaluate() {
        return {std::string("")};
    }

private:

};




#endif //DATACONTAINER_EMPTY_NODE_H
