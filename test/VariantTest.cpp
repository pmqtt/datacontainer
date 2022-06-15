#define BOOST_TEST_DYN_LINK 1
#define BOOST_TEST_MODULE VariantTest.cpp_MODULE

#include <boost/test/included/unit_test.hpp>
#include "../src/api/variant.h"
#include <string>

using test_variant = std::variant<int,bool,float,std::string>;

BOOST_AUTO_TEST_SUITE(VariantTest)

BOOST_AUTO_TEST_CASE(variant_test) {
    test_variant v;
    v = std::string("Hallo");
    bool is_string_or_int = ptl::holds_any_alternative<test_variant,int,std::string>(v);
    bool is_bool_or_int = ptl::holds_any_alternative<test_variant,bool,std::string>(v);

    BOOST_CHECK(is_string_or_int);
    BOOST_CHECK(is_bool_or_int);


}

BOOST_AUTO_TEST_SUITE_END()
