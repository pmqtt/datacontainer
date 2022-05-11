//
// Created by cefour on 04.05.22.
//
#define BOOST_TEST_DYN_LINK 1
#define BOOST_TEST_MODULE FormatTest
#include <boost/test/included/unit_test.hpp>
#include <vector>
#include <string>
#include "../src/Format.h"
#include <algorithm>


BOOST_AUTO_TEST_SUITE(FormatTestSuite)

    BOOST_AUTO_TEST_CASE(FormatSimpleParseTest){
        Format f("%r");
        std::vector<std::shared_ptr<FormatType>> list = f.getFormateList();
        BOOST_CHECK_MESSAGE(list.size() == 1, "list.size() should be 1 not " + std::to_string(list.size()));

        std::shared_ptr<FormatType> type = list[0];
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeReal*>(type.get()) != nullptr, "Type should FormatTypeReal");
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeWord*>(type.get()) == nullptr, "Type should not FormatTypeWord");
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeString*>(type.get()) == nullptr, "Type should not FormatTypeWord");

    }

    BOOST_AUTO_TEST_CASE(FormatSimpleParse2RealFormatArgumentsWithDelimiterTest){
        Format f("%r,%r");
        std::vector<std::shared_ptr<FormatType>> list = f.getFormateList();
        BOOST_CHECK_MESSAGE(list.size() == 3, "list.size() should be 3 not " + std::to_string(list.size()));

        std::shared_ptr<FormatType> type = list[0];
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeReal*>(type.get()) != nullptr, "Type should FormatTypeReal");
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeWord*>(type.get()) == nullptr, "Type should not FormatTypeWord");
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeString*>(type.get()) == nullptr, "Type should not FormatTypeWord");

        type = list[1];
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeReal*>(type.get()) == nullptr, "Type should FormatTypeReal");
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeWord*>(type.get()) != nullptr, "Type should not FormatTypeWord");
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeString*>(type.get()) == nullptr, "Type should not FormatTypeWord");


        type = list[2];
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeReal*>(type.get()) != nullptr, "Type should FormatTypeReal");
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeWord*>(type.get()) == nullptr, "Type should not FormatTypeWord");
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeString*>(type.get()) == nullptr, "Type should not FormatTypeWord");
    }

    BOOST_AUTO_TEST_CASE(FormatSimpleComplexTest){
        Format f("t1=%r t2=%r");
        std::vector<std::shared_ptr<FormatType>> list = f.getFormateList();
        BOOST_CHECK_MESSAGE(list.size() == 4, "list.size() should be 4 not " + std::to_string(list.size()));

        std::shared_ptr<FormatType> type = list[0];
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeReal*>(type.get()) == nullptr, "Type should FormatTypeReal");
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeWord*>(type.get()) != nullptr, "Type should not FormatTypeWord");
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeString*>(type.get()) == nullptr, "Type should not FormatTypeWord");

        type = list[1];
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeReal*>(type.get()) != nullptr, "Type should FormatTypeReal");
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeWord*>(type.get()) == nullptr, "Type should not FormatTypeWord");
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeString*>(type.get()) == nullptr, "Type should not FormatTypeWord");


        type = list[2];
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeReal*>(type.get()) == nullptr, "Type should FormatTypeReal");
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeWord*>(type.get()) != nullptr, "Type should not FormatTypeWord");
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeString*>(type.get()) == nullptr, "Type should not FormatTypeWord");

        type = list[3];
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeReal*>(type.get()) != nullptr, "Type should FormatTypeReal");
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeWord*>(type.get()) == nullptr, "Type should not FormatTypeWord");
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeString*>(type.get()) == nullptr, "Type should not FormatTypeWord");

    }

    BOOST_AUTO_TEST_CASE(FormatFailed1Test){
        Format f("t1=%s t2=%s");
        std::vector<std::shared_ptr<FormatType>> list = f.getFormateList();
        BOOST_CHECK_MESSAGE(list.size() == 4, "list.size() should be 4 not " + std::to_string(list.size()));

        std::shared_ptr<FormatType> type = list[0];
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeReal*>(type.get()) == nullptr, "Type should FormatTypeReal");
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeWord*>(type.get()) != nullptr, "Type should not FormatTypeWord");
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeString*>(type.get()) == nullptr, "Type should not FormatTypeWord");

        type = list[1];
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeReal*>(type.get()) == nullptr, "Type should FormatTypeReal");
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeWord*>(type.get()) == nullptr, "Type should not FormatTypeWord");
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeString*>(type.get()) != nullptr, "Type should not FormatTypeWord");


        type = list[2];
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeReal*>(type.get()) == nullptr, "Type should FormatTypeReal");
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeWord*>(type.get()) != nullptr, "Type should not FormatTypeWord");
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeString*>(type.get()) == nullptr, "Type should not FormatTypeWord");

        type = list[3];
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeReal*>(type.get()) == nullptr, "Type should FormatTypeReal");
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeWord*>(type.get()) == nullptr, "Type should not FormatTypeWord");
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeString*>(type.get()) != nullptr, "Type should not FormatTypeWord");

    }


    BOOST_AUTO_TEST_CASE(FormatInterpretTest){
        Format f("t1=%r t2=%r");
        BOOST_CHECK_MESSAGE(f.interpret("t1=1.234 t2=1.02"),"Should be true not false");
        std::vector<std::shared_ptr<FormatType>> list = f.getFormateList();
        std::shared_ptr<FormatType> type = list[0];
        auto word1 = dynamic_cast<FormatTypeWord*>(type.get());
        type = list[1];
        auto real1 = dynamic_cast<FormatTypeReal*>(type.get());
        type = list[3];
        auto real2 = dynamic_cast<FormatTypeReal*>(type.get());
        BOOST_CHECK_MESSAGE(real1->argument == "1.234", "Should 1.234 not " + real1->argument);
        BOOST_CHECK_MESSAGE(real2->argument == "1.02", "Should 1.02 not " + real2->argument);
        list = f.getArgumentList();
        BOOST_CHECK_MESSAGE(list.size() == 2, "list.size() should be 2 not " + std::to_string(list.size()));
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeReal*>(list[0].get())->argument == "1.234", "Should 1.234 not " + dynamic_cast<FormatTypeReal*>(list[0].get())->argument);
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeReal*>(list[1].get())->argument == "1.02", "Should 1.02 not " + dynamic_cast<FormatTypeReal*>(list[1].get())->argument);
        auto argumentsMap = f.getArgumentsMap();
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeReal*>(argumentsMap["$1"].get())->argument == "1.234", "Should 1.234 not " + dynamic_cast<FormatTypeReal*>(argumentsMap["$1"].get())->argument);
        BOOST_CHECK_MESSAGE(dynamic_cast<FormatTypeReal*>(argumentsMap["$2"].get())->argument == "1.02", "Should 1.02 not " + dynamic_cast<FormatTypeReal*>(argumentsMap["$2"].get())->argument);

    }

BOOST_AUTO_TEST_SUITE_END()