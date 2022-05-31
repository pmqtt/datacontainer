//
// Created by cefour on 04.05.22.
//

#ifndef DATACONTAINER_FORMAT_H
#define DATACONTAINER_FORMAT_H
#include "FormatType.h"
#include "../storage/DataTypes.h"

#include <string>
#include <vector>
#include <memory>
#include <cstdio>
#include <algorithm>
#include <map>




using FormatSpecifier =  std::map<char,std::shared_ptr<FormatType>>;

namespace {
    FormatSpecifier TYPE_INPUT_FORMAT = {
            {'r', std::make_shared<FormatTypeReal>()},
            {'s', std::make_shared<FormatTypeString>()}
    };
}
class Format {
private:
    std::string format;
    std::vector<std::shared_ptr<FormatType>> formatList;
    FormatSpecifier formatSpecifier;

public:
    Format(){}
    Format(const std::string & format,const FormatSpecifier & spec = TYPE_INPUT_FORMAT ): format(format), formatSpecifier(spec) { parse();}

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
        unsigned short index = 1;
        for(auto & iter : formatList){
            if(iter->isArgument()) {
                std::string key ="$";
                key += std::to_string(index);
                arguments[key] = iter;
                index = index +1;
            }
        }
        return arguments;
    }

private:
    void parse();

};

#endif //DATACONTAINER_FORMAT_H
