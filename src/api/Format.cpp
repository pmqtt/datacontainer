//
// Created by cefour on 04.05.22.
//
#include "Format.h"
#include <iostream>

void Format::parse(){
    std::string wordBuffer="";
    for(int i = 0; i < format.length(); i++){
        if(format[i] == '%'){
            if(i+1 < format.length()) {
                if(!wordBuffer.empty()){
                    this->formatList.push_back(std::make_shared<FormatTypeWord>(wordBuffer));
                    wordBuffer = "";
                }
                if( formatSpecifier.count(format[i+1]) > 0){
                    this->formatList.push_back(formatSpecifier[format[i+1]]->create());
                    i++;
                }else{
                    wordBuffer += "%"+format[i+1];
                    i++;
                }

            }else{
                wordBuffer += format[i];
                this->formatList.push_back(std::make_shared<FormatTypeWord>(wordBuffer));
            }
        }else{
            wordBuffer += format[i];
        }
    }
    if(!wordBuffer.empty()){
        this->formatList.push_back(std::make_shared<FormatTypeWord>(wordBuffer));
    }
}
bool Format::interpret(const std::string & x){
    int j = 0;
    auto formatType = this->formatList[j];
    for(int i = 0; i < x.length(); i++){
        if(!formatType->accept(x[i])){
            j = j +1;
            if(j < this->formatList.size()){
                formatType = this->formatList[j];
                if(!formatType->accept(x[i])){
                    return false;
                }
            }else{
                return false;
            }
        }
    }
    return true;
}
