//
// Created by cefour on 13.05.22.
//
#define BOOST_TEST_DYN_LINK 1
#define BOOST_TEST_MODULE DateTimeTest
#include <boost/test/included/unit_test.hpp>
#include <vector>
#include <string>
#include "../src/api/DateTime.h"
#include <algorithm>
struct MyintParameter {
    constexpr static  const char tag[] ="number";
};
struct MyfloatParameter {
    constexpr static  const char tag[] ="number";
};

struct MytimeParameter {
    constexpr static  const char tag[] ="time";
};

struct MydoubleParameter {
    constexpr static  const char tag[] ="time";
};

CONVERTABLE_BOTH(MyintParameter,MyfloatParameter);




BOOST_AUTO_TEST_SUITE(DateTimeTestSuite)

    BOOST_AUTO_TEST_CASE(DateTimeTest){
/*
        DateTime date;
        using Myfloat = NamedType<float,MyfloatParameter,addable>;
        using Myint = NamedType<int,MyintParameter,addable>;
        using Mytime = NamedType<int,MytimeParameter,addable,stringable>;
        Myfloat f1{1};
        Myint f2{3};
        Myfloat f = f1 + f2;
        Myint g = f2+f1;

        BOOST_CHECK_MESSAGE(f.get() == 4,"SHOULD 4 is " + std::to_string(f.get()));
        BOOST_CHECK_MESSAGE(g.get() == 4,"SHOULD 4 is " + std::to_string(g.get()));
        Mytime t1(12);

        BOOST_CHECK(t1.toString()=="12");
*/
       // Mytime t = t1 + f2;

        DateTime dateTime = DateTime::timestamp();
        dateTime.setFormat("%Y-%m-%d %h:%i:%s.%l");
        std::cout<<dateTime.toString()<<std::endl;
        dateTime.setFormat("Es ist %h Uhr %i und %s Sekunden");
        std::cout<<dateTime.toString()<<std::endl;
    }

BOOST_AUTO_TEST_SUITE_END()