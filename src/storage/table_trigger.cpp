//
// Created by cefour on 09.06.22.
//
#include "table_trigger.h"
#include "../parser/ast_type_helpers.h"

#include <regex>


table_trigger::table_trigger(const ASTNodePtr & cond, const std::vector<ASTNodePtr> & prep )
        : condition(cond),preparation(prep){}


table_trigger::~table_trigger(){

}

bool table_trigger::trigger(chakra::storage_table & tbl){
    auto result = this->condition->evaluate(tbl);
    if(!std::holds_alternative<bool>(result)){
        throw std::runtime_error("evaluation error: condition is not bool ");
    }
    bool value = std::get<bool>(result);
    if(value){
        execute(tbl);
    }
    return value;
}
void table_trigger::execute(chakra::storage_table & tbl){
    for(std::size_t i = 0; i < preparation.size(); i++){
        std::string index = "$";
        index += std::to_string(i+1);
        index = std::string("\\") +index;
        auto result = preparation[i]->evaluate(tbl);
        this->value_map[index] = result;
    }
}
std::string table_trigger::get_message(const std::string & message_template){
    std::string result = message_template;
    for(auto [key, value]: value_map) {
        result = std::regex_replace(result, std::regex(key), ptl::to_string(value));
    }
    return result;
}