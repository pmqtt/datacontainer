//
// Created by cefour on 08.06.22.
//

#ifndef DATACONTAINER_CONSTANT_NODE_H
#define DATACONTAINER_CONSTANT_NODE_H
#include "ast_node.h"

class constant_node : public ast_node
{
public:
    constant_node(double value)
            : is_double(true),value(value) { }

    constant_node(const std::string & str)
            : is_double(false),string_value(str) { }

    evaluation_result evaluate() {
        if(is_double)
            return value;
        return string_value;
    }

private:
    bool is_double;
    double value;
    std::string string_value;
};

#endif //DATACONTAINER_CONSTANT_NODE_H
