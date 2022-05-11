
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestMessageQueu
#include <boost/test/included/unit_test.hpp>
#include <vector>
#include <string>
#include "../src/MessageQueue.h"
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <chrono>




BOOST_AUTO_TEST_SUITE(TEST_MESSAGE_QUEUE)

    BOOST_AUTO_TEST_CASE(TestConstruction){
       MessageQueue<int> queue;
    }

    BOOST_AUTO_TEST_CASE(TestPushPop){
       MessageQueue<int> queue;
        boost::thread t1([&](){
            for(std::size_t i = 0; i< 100; ++i) {
                queue.push(i);
                boost::this_thread::sleep_for(boost::chrono::microseconds(100));
            }
        });
        for(std::size_t i = 0; i < 100; ++i){
            BOOST_CHECK(i == queue.pop());
        }

        t1.join();
    }

    BOOST_AUTO_TEST_CASE(TestPushPopStdThread){
        MessageQueue<int> queue;
        std::thread t1([&](){
            for(std::size_t i = 0; i< 100; ++i) {
                queue.push(i);
                std::this_thread::sleep_for(std::chrono::microseconds(100));
            }
        });
        for(std::size_t i = 0; i < 100; ++i){
            BOOST_CHECK(i == queue.pop());
        }

        t1.join();
    }


    BOOST_AUTO_TEST_CASE(TestDeactivate) {
        try {
           MessageQueue<int> queue;
            boost::thread t1([&]() {
                try {
                    queue.pop();
                }catch(std::exception & e){
                    std::cout<<e.what()<<std::endl;
                    BOOST_CHECK(true);
                }
            });
            BOOST_CHECK(true);
            boost::this_thread::sleep_for(boost::chrono::milliseconds(200));
            queue.deactivate();

            t1.join();

        }catch (std::exception & e){
            std::cout<<e.what()<<std::endl;
        }
    }





BOOST_AUTO_TEST_SUITE_END()//
// Created by cefour on 11.05.22.
//
