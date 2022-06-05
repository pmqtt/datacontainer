//
// Created by cefour on 05.06.22.
//

#ifndef DATACONTAINER_VALUE_COMMAND_H
#define DATACONTAINER_VALUE_COMMAND_H

#include "../api/macro_helper.h"
#include "../storage/storage.h"

#include <cstdlib>
#include <memory>
#include <optional>
#include <string>



struct value_command{
    virtual ~value_command() = default;

    virtual base_storage_object execute(chakra::storage_table & tbl) = 0;
};

struct value_increment_index_command : public value_command{

    value_increment_index_command(const std::string & command) : cmd(command){}
    ~value_increment_index_command() override = default;

    base_storage_object execute(chakra::storage_table & tbl) override{
        std::string index = tbl.create_index(::atoi(cmd.c_str()));
        return storage_string(index);
    }
private:
    std::string cmd;
};

struct value_timestamp_command : public value_command{
    value_timestamp_command(const std::string & command) : cmd(command){}
    ~value_timestamp_command() override = default;
    base_storage_object execute(chakra::storage_table & tbl) override{
        UNUSED(tbl);
        return storage_string(cmd);
    }
private:
    std::string cmd;
};


std::optional<std::shared_ptr<value_command>> make_value_commad(const std::string & str);

#endif //DATACONTAINER_VALUE_COMMAND_H
