//
// Created by cefour on 04.05.22.
//
#include "format.h"
#include <iostream>
#include <utility>

void format::parse(){
    std::string word_buffer="";
    for(unsigned long i = 0; i < format_def.length(); i++){
        if(format_def[i] == '%'){
            if(i+1 < format_def.length()) {
                if(!word_buffer.empty()){
                    this->format_list.push_back(std::make_shared<format_type_word>(word_buffer));
                    word_buffer = "";
                }
                if(format_specifier_list.count(format_def[i + 1]) > 0){
                    this->format_list.push_back(format_specifier_list[format_def[i + 1]]->create());
                    i++;
                }else{
                    word_buffer += "%";
                    word_buffer += format_def[i + 1];
                    i++;
                }

            }else{
                word_buffer += format_def[i];
                this->format_list.push_back(std::make_shared<format_type_word>(word_buffer));
            }
        }else{
            word_buffer += format_def[i];
        }
    }
    if(!word_buffer.empty()){
        this->format_list.push_back(std::make_shared<format_type_word>(word_buffer));
    }
}
bool format::interpret(const std::string & x){
    unsigned long j = 0;
    auto format_arg = this->format_list[j];
    for(unsigned long i = 0; i < x.length(); i++){
        if(!format_arg->accept(x[i])){
            j = j +1;
            if(j < this->format_list.size()){
                format_arg = this->format_list[j];
                if(!format_arg->accept(x[i])){
                    return false;
                }
            }else{
                return false;
            }
        }else{
        }
    }
    return true;
}
