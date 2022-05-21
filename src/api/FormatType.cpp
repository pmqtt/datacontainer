//
// Created by cefour on 17.05.22.
//
#include "FormatType.h"

void FormatTypeWord::hanlde(FormatTypeVisitor * visitor){
    visitor->visit(this);
}
