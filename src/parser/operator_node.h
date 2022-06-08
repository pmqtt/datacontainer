//
// Created by cefour on 08.06.22.
//

#ifndef DATACONTAINER_OPERATOR_NODE_H
#define DATACONTAINER_OPERATOR_NODE_H
#include "ast_node.h"

class operator_node : public ast_node
{
public:
    operator_node(const std::string & op, const ASTNodePtr& left, const ASTNodePtr& right)
            :op(op), left(left), right(right) {
    }

    evaluation_result evaluate() {
        if(op == "<"){
            return std::get<double>(left->evaluate()) < std::get<double>(right->evaluate());
        }
        if(op == "<="){
            return std::get<double>(left->evaluate()) <= std::get<double>(right->evaluate());
        }
        if(op == ">"){
            return std::get<double>(left->evaluate()) > std::get<double>(right->evaluate());
        }
        if(op == ">="){
            return std::get<double>(left->evaluate()) >= std::get<double>(right->evaluate());
        }
        if(op == "=="){
            return std::get<double>(left->evaluate()) == std::get<double>(right->evaluate());
        }
        if(op == "!="){
            return std::get<double>(left->evaluate()) != std::get<double>(right->evaluate());
        }


        if (op == "+") {
            return std::get<double>(left->evaluate()) + std::get<double>(right->evaluate());
        }
        if (op == "-") {
            return std::get<double>(left->evaluate()) - std::get<double>(right->evaluate());
        }
        if (op == "*") {
            return std::get<double>(left->evaluate()) * std::get<double>(right->evaluate());;
        }
        if (op == "/") {
            return std::get<double>(left->evaluate()) / std::get<double>(right->evaluate());;
        }
        return -1.23456;
    }

    ~operator_node() {
        delete left;
        delete right;
    }

private:
    std::string op;
    ASTNodePtr left, right;
};

#endif //DATACONTAINER_OPERATOR_NODE_H
