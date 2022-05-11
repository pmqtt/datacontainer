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
                switch (format[i+1]){
                    case 'r':{
                        this->formatList.push_back(std::make_shared<FormatTypeReal>());
                        i++;
                        break;
                    }
                    case 's':{
                        this->formatList.push_back(std::make_shared<FormatTypeString>());
                        i++;
                        break;
                    }
                }

            }else{
                //IGNORE
            }
        }else{
            wordBuffer += format[i];
        }
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
