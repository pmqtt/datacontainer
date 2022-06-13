//
// Created by cefour on 04.05.22.
//
#define BOOST_TEST_DYN_LINK 1
#define BOOST_TEST_MODULE FormatTest
#include <boost/test/included/unit_test.hpp>
#include <vector>
#include <string>
#include "../src/api/format.h"
#include <algorithm>


BOOST_AUTO_TEST_SUITE(FormatTestSuite)

    BOOST_AUTO_TEST_CASE(FormatSimpleParseTest){
        format f("%r");
        std::vector<std::shared_ptr<format_type>> list = f.get_formate_list();
        BOOST_CHECK_MESSAGE(list.size() == 1, "list.size() should be 1 not " + std::to_string(list.size()));

        std::shared_ptr<format_type> type = list[0];
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_real*>(type.get()) != nullptr, "Type should format_type_real");
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_word*>(type.get()) == nullptr, "Type should not format_type_word");
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_string*>(type.get()) == nullptr, "Type should not format_type_word");

    }

    BOOST_AUTO_TEST_CASE(FormatSimpleParse2RealFormatArgumentsWithDelimiterTest){
        format f("%r,%r");
        std::vector<std::shared_ptr<format_type>> list = f.get_formate_list();
        BOOST_CHECK_MESSAGE(list.size() == 3, "list.size() should be 3 not " + std::to_string(list.size()));

        std::shared_ptr<format_type> type = list[0];
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_real*>(type.get()) != nullptr, "Type should format_type_real");
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_word*>(type.get()) == nullptr, "Type should not format_type_word");
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_string*>(type.get()) == nullptr, "Type should not format_type_word");

        type = list[1];
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_real*>(type.get()) == nullptr, "Type should format_type_real");
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_word*>(type.get()) != nullptr, "Type should not format_type_word");
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_string*>(type.get()) == nullptr, "Type should not format_type_word");


        type = list[2];
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_real*>(type.get()) != nullptr, "Type should format_type_real");
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_word*>(type.get()) == nullptr, "Type should not format_type_word");
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_string*>(type.get()) == nullptr, "Type should not format_type_word");
    }

    BOOST_AUTO_TEST_CASE(FormatSimpleComplexTest){
        format f("t1=%r t2=%r");
        std::vector<std::shared_ptr<format_type>> list = f.get_formate_list();
        BOOST_CHECK_MESSAGE(list.size() == 4, "list.size() should be 4 not " + std::to_string(list.size()));

        std::shared_ptr<format_type> type = list[0];
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_real*>(type.get()) == nullptr, "Type should format_type_real");
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_word*>(type.get()) != nullptr, "Type should not format_type_word");
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_string*>(type.get()) == nullptr, "Type should not format_type_word");

        type = list[1];
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_real*>(type.get()) != nullptr, "Type should format_type_real");
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_word*>(type.get()) == nullptr, "Type should not format_type_word");
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_string*>(type.get()) == nullptr, "Type should not format_type_word");


        type = list[2];
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_real*>(type.get()) == nullptr, "Type should format_type_real");
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_word*>(type.get()) != nullptr, "Type should not format_type_word");
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_string*>(type.get()) == nullptr, "Type should not format_type_word");

        type = list[3];
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_real*>(type.get()) != nullptr, "Type should format_type_real");
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_word*>(type.get()) == nullptr, "Type should not format_type_word");
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_string*>(type.get()) == nullptr, "Type should not format_type_word");

    }

    BOOST_AUTO_TEST_CASE(FormatFailed1Test){
        format f("t1=%s t2=%s");
        std::vector<std::shared_ptr<format_type>> list = f.get_formate_list();
        BOOST_CHECK_MESSAGE(list.size() == 4, "list.size() should be 4 not " + std::to_string(list.size()));

        std::shared_ptr<format_type> type = list[0];
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_real*>(type.get()) == nullptr, "Type should format_type_real");
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_word*>(type.get()) != nullptr, "Type should not format_type_word");
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_string*>(type.get()) == nullptr, "Type should not format_type_word");

        type = list[1];
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_real*>(type.get()) == nullptr, "Type should format_type_real");
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_word*>(type.get()) == nullptr, "Type should not format_type_word");
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_string*>(type.get()) != nullptr, "Type should not format_type_word");


        type = list[2];
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_real*>(type.get()) == nullptr, "Type should format_type_real");
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_word*>(type.get()) != nullptr, "Type should not format_type_word");
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_string*>(type.get()) == nullptr, "Type should not format_type_word");

        type = list[3];
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_real*>(type.get()) == nullptr, "Type should format_type_real");
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_word*>(type.get()) == nullptr, "Type should not format_type_word");
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_string*>(type.get()) != nullptr, "Type should not format_type_word");

    }


    BOOST_AUTO_TEST_CASE(FormatInterpretTest){
        format f("t1=%r t2=%r");
        BOOST_CHECK_MESSAGE(f.interpret("t1=1.234 t2=1.02"),"Should be true not false");
        std::vector<std::shared_ptr<format_type>> list = f.get_formate_list();
        std::shared_ptr<format_type> type = list[0];
        auto word1 = dynamic_cast<format_type_word*>(type.get());
        BOOST_CHECK_MESSAGE(word1->toAcceptedWord == "t1=", "t1= " + word1->toAcceptedWord);
        type = list[1];
        auto real1 = dynamic_cast<format_type_real*>(type.get());
        type = list[3];
        auto real2 = dynamic_cast<format_type_real*>(type.get());
        BOOST_CHECK_MESSAGE(real1->argument == "1.234", "Should 1.234 not " + real1->argument);
        BOOST_CHECK_MESSAGE(real2->argument == "1.02", "Should 1.02 not " + real2->argument);
        list = f.get_argument_list();
        BOOST_CHECK_MESSAGE(list.size() == 2, "list.size() should be 2 not " + std::to_string(list.size()));
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_real*>(list[0].get())->argument == "1.234", "Should 1.234 not " + dynamic_cast<format_type_real*>(list[0].get())->argument);
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_real*>(list[1].get())->argument == "1.02", "Should 1.02 not " + dynamic_cast<format_type_real*>(list[1].get())->argument);
        auto argumentsMap = f.get_arguments_map();
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_real*>(argumentsMap["$1"].get())->argument == "1.234", "Should 1.234 not " + dynamic_cast<format_type_real*>(argumentsMap["$1"].get())->argument);
        BOOST_CHECK_MESSAGE(dynamic_cast<format_type_real*>(argumentsMap["$2"].get())->argument == "1.02", "Should 1.02 not " + dynamic_cast<format_type_real*>(argumentsMap["$2"].get())->argument);

    }

    BOOST_AUTO_TEST_CASE(FORMAT_INTERPRET_TEST_WITH_FIRST_STRING_ARGUMENT){
        format f("%s %r %s");
        BOOST_CHECK_MESSAGE(f.interpret("Wohnzimmer 19.7 HALLOWELT"),"Should be true not false");
        std::vector<std::shared_ptr<format_type>> list = f.get_formate_list();
        BOOST_CHECK_MESSAGE(list.size() == 5, "List size should 5 not " + std::to_string(list.size()));
        std::shared_ptr<format_type> type = list[0];
        auto word1 = dynamic_cast<format_type_string*>(type.get());
        BOOST_CHECK(word1 != nullptr);
        BOOST_CHECK(std::get<storage_string>(word1->create_base_storage_object()).get_value()=="Wohnzimmer");
    }

BOOST_AUTO_TEST_SUITE_END()