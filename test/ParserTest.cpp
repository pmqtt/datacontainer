#define BOOST_TEST_DYN_LINK 1
#define BOOST_TEST_MODULE ParserTest_MODULE

#include <boost/test/included/unit_test.hpp>
#include "../src/parser/grammar.h"





void test1(std::string input)
{
    std::cout<<"------------------------------------------------------------------------------------------"<<std::endl;
    ASTNode* out_node;
    std::cout<<"Try parse: "<<std::quoted(input)<<std::endl;
    PhraseParseOrDie(input, ArithmeticGrammar1(), qi::space, out_node);
    std::cout<<"Try to evaluate: "<<std::quoted(input)<<std::endl;
    if(out_node != nullptr) {
        auto eval = out_node->evaluate();
        result_visitor res_visitor;
        res_visitor.input = input;
        std::visit(res_visitor,eval);

        delete out_node;
    }else{
        std::cout<<"produce empty node"<<std::endl;
    }
    std::cout<<"------------------------------------------------------------------------------------------"<<std::endl;



}




BOOST_AUTO_TEST_SUITE(ParserTestSuite)

    BOOST_AUTO_TEST_CASE(Test1) {
        test1("\"HALLO\"");
        test1("");
        test1("1+2*3-8");
        test1("1+2*3-8 < 4+5");
        test1("constante()");
        test1("constante(2)");
        test1("identity(2+3) < constante()");
        test1("a");
    }

BOOST_AUTO_TEST_SUITE_END()
