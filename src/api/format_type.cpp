//
// Created by cefour on 17.05.22.
//
#include "format_type.h"

void format_type_word::handle(format_type_visitor * visitor){
    visitor->visit(this);
}
