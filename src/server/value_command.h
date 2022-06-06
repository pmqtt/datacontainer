//
// Created by cefour on 05.06.22.
//

#ifndef DATACONTAINER_VALUE_COMMAND_H
#define DATACONTAINER_VALUE_COMMAND_H

#include "../api/format.h"
#include "../api/macro_helper.h"
#include "../storage/storage.h"

#include <cstdlib>
#include <memory>
#include <optional>
#include <regex>
#include <string>



struct value_command{
    virtual ~value_command() = default;

    virtual base_storage_object execute(chakra::storage_table & tbl,const std::string & value)= 0;

};

struct value_increment_index_command : public value_command{

    value_increment_index_command(const std::string & command) : cmd(command){}
    ~value_increment_index_command() override = default;

    base_storage_object execute(chakra::storage_table & tbl,const std::string & value) override{
        UNUSED(value);
        std::string index = tbl.create_index(::atoi(cmd.c_str()));
        return storage_string(index);
    }
private:
    std::string cmd;
};

struct value_timestamp_command : public value_command{
    value_timestamp_command(const std::string & command) : cmd(command){}
    ~value_timestamp_command() override = default;
    base_storage_object execute(chakra::storage_table & tbl,const std::string & value) override{
        UNUSED(tbl);
        UNUSED(value);
        return storage_string(cmd);
    }
private:
    std::string cmd;
};

struct value_argument_command : public value_command{
    value_argument_command(const format & f,const std::string & str) : fmt(f),cmd(str){}
    ~value_argument_command() override = default;
    base_storage_object execute(chakra::storage_table & tbl,const std::string & value) override{
        UNUSED(tbl);
        UNUSED(value);
        std::smatch sm;
        std::regex x("\\$[0-9]+");
        std::cout<<"value:"<<value<<std::endl;
        if(fmt.interpret(value)) {
            auto format_map = fmt.get_arguments_map();
            if (std::regex_match(cmd, sm, x)) {
                return format_map[sm[0]]->create_base_storage_object();
            }
        }
        throw std::runtime_error("error");
    }
private:
    format fmt;
    std::string cmd;
};


std::optional<std::shared_ptr<value_command>> make_value_command(const std::string & str,const std::string &format_def);

#endif //DATACONTAINER_VALUE_COMMAND_H
