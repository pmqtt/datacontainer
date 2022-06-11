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
#include <iomanip>

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















#endif //DATACONTAINER_AST_NODE_H
