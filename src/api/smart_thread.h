//
// Created by cefour on 10.05.22.
//

#ifndef DATACONTAINER_SMART_THREAD_H
#define DATACONTAINER_SMART_THREAD_H



#include <thread>
#include <type_traits>
#include <boost/thread.hpp>

template<class THREAD>
class smart_thread {
public:
    template<class FUNCTION, class... ARGS>
    smart_thread(const std::function<void()> & releaseFunc, FUNCTION && threadFunc, ARGS&&... args)
            :release_function(releaseFunc),
             thread(threadFunc,args...){

    }
    ~smart_thread(){
        if constexpr ( std::is_same<THREAD,boost::thread>::value ){
            thread.interrupt();
        }
        release_function();
        if(thread.joinable()) {
            thread.join();
        }
    }
    smart_thread(smart_thread && rhs) = default;
    smart_thread &operator=(smart_thread && rhs) = default;
    THREAD & get() {
        return thread;
    }
private:
    std::function<void()> release_function;
    THREAD thread;
};



#endif //DATACONTAINER_SMART_THREAD_H
