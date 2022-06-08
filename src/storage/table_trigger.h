//
// Created by cefour on 07.06.22.
//

#ifndef DATACONTAINER_TABLE_OBSERVER_H
#define DATACONTAINER_TABLE_OBSERVER_H

#include <functional>

struct table_trigger{
    virtual void trigger() = 0;
    virtual void execute(const std::function<void()> &func) = 0;
};

struct insert_count_trigger : public table_trigger{
    insert_count_trigger() : should_execute(false){}
    void trigger(){
        should_execute = true;
    }
    void execute(const std::function<void()> &func){
        if(should_execute){
            func();
        }
    };



private:
    bool should_execute;
};



#endif //DATACONTAINER_TABLE_OBSERVER_H
