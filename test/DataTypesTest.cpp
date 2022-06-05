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
#include "../src/storage/storage_object.h"
#include <algorithm>
#include <variant>
#include <typeinfo>




BOOST_AUTO_TEST_SUITE(DataTypesTestSuite)

    BOOST_AUTO_TEST_CASE(DataTypeConstructorTest) {
        data_type type(new int_type(123), TYPE_KIND::INT);
        BOOST_CHECK_MESSAGE(type.is_integer(), "Expected Int Type");
        BOOST_CHECK_MESSAGE(!type.is_real(), "Expected Int Type");
        BOOST_CHECK_MESSAGE(!type.is_string(), "Expected Int Type");
    }

    BOOST_AUTO_TEST_CASE(STORAGE_OBJECT_CMP_LESS_CONCRET_TEST) {
        storage_int i(10);
        storage_real r(12.3);
        BOOST_CHECK(storage_cmp_less(i, r));
        BOOST_CHECK(storage_cmp_less(r, i) == false);
    }

    BOOST_AUTO_TEST_CASE(STORAGE_OBJECT_CMP_LESS_TEST) {
        base_storage_object i = storage_int(10);
        base_storage_object r = storage_real(12.3);
        BOOST_CHECK(ptl::cmp_less(i, r));
        BOOST_CHECK(ptl::cmp_less(r, i) == false);
    }

    BOOST_AUTO_TEST_CASE(STORAGE_OBJECT_CMP_LESS_STRING_WITH_TYPE_TEST) {
        base_storage_object i = storage_string("10");
        base_storage_object r = storage_real(12.3);
        try {
            ptl::cmp_less(i, r);
        } catch (std::runtime_error &e) {
            BOOST_CHECK(true);
        }

    }

    BOOST_AUTO_TEST_CASE(STORAGE_OBJECT_CMP_LESS_BOOL_TEST) {
        base_storage_object i = storage_bool(true);
        base_storage_object r = storage_bool(false);
        try {
            ptl::cmp_less(i, r);
        } catch (std::runtime_error &e) {
            BOOST_CHECK(true);
        }

    }

    BOOST_AUTO_TEST_CASE(STORAGE_OBJECT_ADD_TEST) {
        storage_int i(10);
        storage_int k(20);
        storage_real r(12.3);
        storage_real f(12.3);
        storage_string str("HALLO");
        storage_string str2(" Welt");
        using R1 = decltype(storage_object_add(i, k).get_value());
        using R2 = decltype(storage_object_add(i, r).get_value());
        bool isInt = std::is_same<R1, const int &>::value;
        bool isDouble = std::is_same<R2, const double &>::value;

        BOOST_CHECK(isDouble);
        BOOST_CHECK(isInt);
        BOOST_CHECK(storage_object_add(i, r).get_value() == 22.3);
        BOOST_CHECK(storage_object_add(i, k).get_value() == 30);
        BOOST_CHECK(storage_object_add(r, f).get_value() == 24.6);
        BOOST_CHECK(storage_object_add(str, str2).get_value() == "HALLO Welt");
        try {
            storage_object_add(r, str);
        } catch (std::runtime_error &e) {
            BOOST_CHECK(true);
        }
        BOOST_CHECK(true);
        base_storage_object b1 = i;
        base_storage_object r1 = r;
        base_storage_object res = ptl::add(b1, r1);
        std::visit(
                [&](const auto &v1) {
                    using T = std::decay_t<decltype(v1)>;
                    std::cout << typeid(T).name() << std::endl;
                    bool chk = std::is_same<T, storage_real>::value;
                    BOOST_CHECK(chk);
                }, res);
    }

    BOOST_AUTO_TEST_CASE(STORAGE_OBJECT_ADD_TO_VECTOR) {
        ptl::table<base_storage_object,std::vector<base_storage_object>> foo;
        foo[storage_string("HALLO")].emplace_back(storage_int(2));
    }

    BOOST_AUTO_TEST_SUITE_END()
