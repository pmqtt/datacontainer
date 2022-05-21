//
// Created by cefour on 10.05.22.
//

#ifndef DATACONTAINER_SMARTTHREAD_H
#define DATACONTAINER_SMARTTHREAD_H



#include <thread>
#include <type_traits>
#include <boost/thread.hpp>

template<class THREAD>
class SmartThread {
public:
    template<class FUNCTION, class... ARGS>
    SmartThread(const std::function<void()> & releaseFunc,FUNCTION && threadFunc,ARGS&&... args)
            :releaseFunction(releaseFunc),
             thread(threadFunc,args...){

    }
    ~SmartThread(){
        if constexpr ( std::is_same<THREAD,boost::thread>::value ){
            thread.interrupt();
        }
        releaseFunction();
        if(thread.joinable()) {
            thread.join();
        }
    }
    SmartThread(SmartThread && rhs) = default;
    SmartThread &operator=(SmartThread && rhs) = default;
    THREAD & get() {
        return thread;
    }
private:
    std::function<void()> releaseFunction;
    THREAD thread;
};



#endif //DATACONTAINER_SMARTTHREAD_H
