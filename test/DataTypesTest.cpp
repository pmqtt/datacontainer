//
// Created by Michael Burzan on 18.04.22.
//

#define BOOST_TEST_DYN_LINK 1
#define BOOST_TEST_MODULE DataTypesTest
#include <boost/test/included/unit_test.hpp>
#include <vector>
#include <string>
#include "../src/datastructure/table.h"
#include "../src/api/prime_stream.h"
#include "../src/api/time_measure.h"
#include "../src/storage/data_types.h"
#include <algorithm>


BOOST_AUTO_TEST_SUITE(DataTypesTestSuite)

BOOST_AUTO_TEST_CASE(DataTypeConstructorTest){
    data_type type(new int_type(123), TYPE_KIND::INT);
    BOOST_CHECK_MESSAGE(type.is_integer(), "Expected Int Type");
    BOOST_CHECK_MESSAGE(!type.is_real(), "Expected Int Type");
    BOOST_CHECK_MESSAGE(!type.is_string(), "Expected Int Type");
}

BOOST_AUTO_TEST_SUITE_END()
