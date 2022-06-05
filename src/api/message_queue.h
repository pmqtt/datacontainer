//
// Created by cefour on 11.05.22.
//

#ifndef DATACONTAINER_MESSAGE_QUEUE_H
#define DATACONTAINER_MESSAGE_QUEUE_H


#include "../exceptions/message_queue_deactivate_exception.h"
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>




template <typename T>
class message_queue
{
public:
    using ulock = std::unique_lock<std::mutex>;
    using mtx = std::mutex;
    using condvar = std::condition_variable;

    message_queue() {
        this->run = true;

    }
    ~message_queue(){
    }

    message_queue(message_queue && rhs){
        queue = std::move(rhs.queue);

    }
    message_queue(const message_queue & rhs){

        queue = rhs.queue;
    }


    message_queue &operator=(message_queue && rhs){
        queue = std::move(rhs.queue);
        return *this;
    }
    message_queue &operator=(const message_queue & rhs){
        queue = rhs.queue;
        return *this;
    }

    T pop(){
        try {
            ulock mlock(mutex);
            while (queue.empty() && run) {
                cond.wait(mlock);
            }
            if(!run){
                throw message_queue_deactivate_exception("Message queue is deactivated by user");
            }
            auto val = queue.front();
            queue.pop();

            return val;
        }
        catch (const std::runtime_error& re){
            std::cerr << "Runtime error: " << re.what() << std::endl;
        }
        catch(message_queue_deactivate_exception & e){
            throw e;
        }
        catch (const std::exception& ex){
            std::cout << "Error occurred: " << ex.what() << std::endl;
        }
        catch (...) {
            std::cout << "Unknown error in Message Queue" << "\n";
        }

        throw std::exception();
    }

    void pop(T& item){
        ulock mlock(mutex);
        while (queue.empty())
        {
            cond.wait(mlock);
        }
        item = queue.front();
        queue.pop();
    }

    void push(const T& item){
        ulock mlock(mutex);
        queue.push(item);
        mlock.unlock();
        cond.notify_one();
    }
    void deactivate(){
        ulock mlock(mutex);
        this->run = false;
        mlock.unlock();
        cond.notify_one();

    }

    void restart(){
        run = true;
    }

private:
    std::queue<T> queue;
    mtx mutex;
    condvar cond;
    std::atomic_bool run;
};






#endif //DATACONTAINER_MESSAGE_QUEUE_H
