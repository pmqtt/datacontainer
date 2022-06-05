//
// Created by cefour on 27.05.22.
//
#define BOOST_TEST_DYN_LINK 1
#define BOOST_TEST_MODULE STRING_TEST_MODULE
#include <boost/test/included/unit_test.hpp>
#include "../src/datastructure/string.h"

#include <variant>



using PTL_VAR = std::variant<ptl::string>;

void CONTROLL_ERROR_VARIANT(){
    PTL_VAR x = ptl::string("xx");
}


BOOST_AUTO_TEST_SUITE(StringTestSuite)


    BOOST_AUTO_TEST_CASE(String_Constructor){
        BOOST_CHECK(true);
    }





BOOST_AUTO_TEST_SUITE_END()
