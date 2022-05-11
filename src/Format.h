//
// Created by cefour on 04.05.22.
//

#ifndef DATACONTAINER_FORMAT_H
#define DATACONTAINER_FORMAT_H
#include "storage/DataTypes.h"
#include <string>
#include <vector>
#include <memory>
#include <cstdio>
#include <algorithm>
#include <map>

struct FormatType{
    virtual ~FormatType() = default;
    virtual bool accept(const char x)  = 0;
    virtual bool isArgument() const = 0;
    virtual DataType createDataType() = 0;
};

struct FormatTypeWord : public FormatType{
    std::string toAcceptedWord;
    int index = 0;
    FormatTypeWord(const std::string & word) : toAcceptedWord(word){  }
    bool accept(const char x) {
        if(index < toAcceptedWord.length()) {
            if (toAcceptedWord[index] == x) {
                index++;
                return true;
            }
        }
        return false;
    }
    bool isArgument() const{
        return false;
    }
    DataType createDataType(){ throw std::invalid_argument("Not valid parameter");}

};

struct FormatTypeReal: public FormatType{
    std::string argument;
    bool accept(const char x) {
        if(x == '.'){
            argument += x;
            return true;
        }
        if(::isdigit(x)){
            argument += x;
            return true;
        }
        return false;
    }
    bool isArgument() const{
        return true;
    }
    DataType createDataType(){
        return DataType(new RealType(std::atof(argument.c_str())));
    }
};

struct FormatTypeString: public FormatType{
    std::string argument;
    bool accept(const char x) {
        argument += x;
        return true;
    }
    bool isArgument() const{
        return true;
    }
    DataType createDataType(){
        return DataType(new StringType(argument));
    }
};

class Format {
private:
    std::string format;
    std::vector<std::shared_ptr<FormatType>> formatList;

public:
    Format(const std::string & format ): format(format){ parse();}

public:
    auto getFormateList()const { return this->formatList;}
    bool interpret(const std::string & x);
    std::vector<std::shared_ptr<FormatType>> getArgumentList(){
        std::vector<std::shared_ptr<FormatType>> arguments;
        std::copy_if(this->formatList.begin(), this->formatList.end(),
                  std::back_inserter(arguments), [](const auto & val )->bool { return val->isArgument(); });
        return arguments;
    }

    std::map<std::string,std::shared_ptr<FormatType>> getArgumentsMap()const{
        std::map<std::string,std::shared_ptr<FormatType>> arguments;
        int index = 1;
        for(auto & iter : formatList){
            if(iter->isArgument()) {
                arguments["$" + std::to_string(index)] = iter;
                index = index +1;
            }
        }
        return arguments;
    }

private:
    void parse();

};

#endif //DATACONTAINER_FORMAT_H
