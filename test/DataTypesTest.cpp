//
// Created by Michael Burzan on 18.04.22.
//

#define BOOST_TEST_DYN_LINK 1
#define BOOST_TEST_MODULE DataTypesTest
#include <boost/test/included/unit_test.hpp>
#include <vector>
#include <string>
#include "../src/storage/Table.h"
#include "../src/storage/PrimeStream.h"
#include "../src/storage/TimeMeasure.h"
#include "../src/storage/DataTypes.h"
#include <algorithm>


BOOST_AUTO_TEST_SUITE(DataTypesTestSuite)

BOOST_AUTO_TEST_CASE(DataTypeConstructorTest){
    DataType type(new IntType(123),TYPE_KIND::INT);
    BOOST_CHECK_MESSAGE(type.isInteger(),"Expected Int Type");
    BOOST_CHECK_MESSAGE(!type.isReal(),"Expected Int Type");
    BOOST_CHECK_MESSAGE(!type.isString(),"Expected Int Type");
}

BOOST_AUTO_TEST_SUITE_END()
