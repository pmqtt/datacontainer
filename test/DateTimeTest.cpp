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
BOOST_AUTO_TEST_SUITE(DateTimeTestSuite)

    BOOST_AUTO_TEST_CASE(DateTime){

        DateTime date;
        using Myfloat = NamedType<float,MyfloatParameter,Addable>;
        using Myint = NamedType<int,MyintParameter,Addable>;
        using Mytime = NamedType<int,MytimeParameter,Addable>;
        Myfloat f1{1};
        Myint f2{3};
        Myfloat f = f1 + f2;
        Myint g = f2+f1;

        BOOST_CHECK_MESSAGE(f.get() == 4,"SHOULD 4 is " + std::to_string(f.get()));
        BOOST_CHECK_MESSAGE(g.get() == 4,"SHOULD 4 is " + std::to_string(g.get()));
        Mytime t1(12);
      //  Mytime t = t1 + f2;
    }

BOOST_AUTO_TEST_SUITE_END()