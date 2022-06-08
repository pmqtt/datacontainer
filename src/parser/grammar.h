//
// Created by cefour on 08.06.22.
//

#ifndef DATACONTAINER_GRAMMER_H
#define DATACONTAINER_GRAMMER_H

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <memory>
#include <variant>

#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix.hpp>

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;

/******************************************************************************/

// Utility to run a parser, check for errors, and capture the results.
template <typename Parser, typename Skipper, typename ... Args>
void PhraseParseOrDie(
        const std::string& input, const Parser& p, const Skipper& s,
        Args&& ... args)
{
    std::string::const_iterator begin = input.begin(), end = input.end();
    boost::spirit::qi::phrase_parse(
            begin, end, p, s, std::forward<Args>(args) ...);
    if (begin != end) {
        std::cout << "Unparseable: "
                  << std::quoted(std::string(begin, end)) << std::endl;
    }
}

/******************************************************************************/


using arg_result = std::variant<bool,double,std::string>;

using evaluation_result = std::variant<bool,double,std::string,std::vector<arg_result>>;


class ASTNode
{
public:
    virtual evaluation_result evaluate() = 0;
    virtual ~ASTNode() { }
};

using ASTNodePtr = ASTNode*;

class EmptyNode : public ASTNode
{
public:
    EmptyNode(){

    }

    evaluation_result evaluate() {
        return {std::string("")};
    }

private:

};


class OperatorNode : public ASTNode
{
public:
    OperatorNode(const std::string & op,const ASTNodePtr& left, const ASTNodePtr& right)
            :op(op), left(left), right(right) {
    }

    evaluation_result evaluate() {
        if(op == "<"){
            return std::get<double>(left->evaluate()) < std::get<double>(right->evaluate());
        }
        if(op == "<="){
            return std::get<double>(left->evaluate()) <= std::get<double>(right->evaluate());
        }
        if(op == ">"){
            return std::get<double>(left->evaluate()) > std::get<double>(right->evaluate());
        }
        if(op == ">="){
            return std::get<double>(left->evaluate()) >= std::get<double>(right->evaluate());
        }
        if(op == "=="){
            return std::get<double>(left->evaluate()) == std::get<double>(right->evaluate());
        }
        if(op == "!="){
            return std::get<double>(left->evaluate()) != std::get<double>(right->evaluate());
        }


        if (op == "+") {
            return std::get<double>(left->evaluate()) + std::get<double>(right->evaluate());
        }
        if (op == "-") {
            return std::get<double>(left->evaluate()) - std::get<double>(right->evaluate());
        }
        if (op == "*") {
            return std::get<double>(left->evaluate()) * std::get<double>(right->evaluate());;
        }
        if (op == "/") {
            return std::get<double>(left->evaluate()) / std::get<double>(right->evaluate());;
        }
        return -1.23456;
    }

    ~OperatorNode() {
        delete left;
        delete right;
    }

private:
    std::string op;
    ASTNodePtr left, right;
};

class ConstantNode : public ASTNode
{
public:
    ConstantNode(double value)
            : is_double(true),value(value) { }

    ConstantNode(const std::string & str)
            : is_double(false),string_value(str) { }

    evaluation_result evaluate() {
        if(is_double)
            return value;
        return string_value;
    }

private:
    bool is_double;
    double value;
    std::string string_value;
};


std::map<std::string,std::function<evaluation_result(const std::vector<arg_result> & arguments)>> built_in_functions{
        {"constante",[](const std::vector<arg_result> & arguments) -> evaluation_result{
            return 3.0;
        }},
        {"identity",[](const std::vector<arg_result> & arguments) -> evaluation_result{
            return std::visit([](auto & x){
                evaluation_result res =x;
                return res;
            },arguments[0]);
        }}
};


class FunctionNode : public ASTNode
{
public:
    FunctionNode(const std::string & func_name,const ASTNodePtr & args) : function_name(func_name),args(args){
    }

    evaluation_result evaluate() {
        auto arguments = args->evaluate();
        return built_in_functions[function_name](std::get<std::vector<arg_result>>(arguments));
    }

private:
    std::string function_name;
    ASTNodePtr args;
};




class IdentifierNode : public ASTNode
{
public:
    IdentifierNode(const std::string & name) : name(name){
    }

    evaluation_result evaluate() {
        return -1.0;
    }

private:
    std::string name;
};

template<class T>
void is_class(ASTNodePtr & ptr,const std::string & name){
    if(dynamic_cast<T*>(ptr) != nullptr){
        std::cout<<name;
    }
}


struct argument_result_visitor{
    arg_result operator()(bool x){
        return x;
    }
    arg_result operator()(double x){
        return x;
    }
    arg_result operator()(std::string x){
        return x;
    }
    arg_result operator()(std::vector<arg_result> & x){
        throw std::runtime_error("not valid cast");
    }
};




class ArgumentNode : public ASTNode
{
public:
    ArgumentNode(const ASTNodePtr & a,const std::vector<ASTNodePtr> &args) : arg(a),args(args){
    }

    evaluation_result evaluate() {

        auto first_argument = arg->evaluate();
        bool is_empty = std::holds_alternative<std::string>(first_argument);
        std::vector<arg_result> result;
        if( is_empty ){
            arg_result tmp = "";
            result.push_back(tmp);
            return result;
        }
        argument_result_visitor vis;
        arg_result casted_argument = std::visit(vis,first_argument);
        result.push_back(casted_argument);


        for(auto & argument: args){
            auto iter_argument = argument->evaluate();
            casted_argument = std::visit(vis,iter_argument);
            result.push_back(casted_argument);
        }

        return result;
    }

private:
    ASTNodePtr  arg;
    std::vector<ASTNodePtr> args;
};




/******************************************************************************/

class ArithmeticGrammar1
        : public qi::grammar<std::string::const_iterator, ASTNodePtr(), qi::space_type>
{
public:
    using Iterator = std::string::const_iterator;

    ArithmeticGrammar1() : ArithmeticGrammar1::base_type(start)
    {
        str %= *(qi::char_ - '"');
        identifier %= qi::alpha >> *qi::alnum;
        product_op = qi::string("+") | qi::string("-");
        factor_op = qi::string("*") | qi::string("/");
        cmp_op = qi::string("<") | qi::string("<=") | qi::string(">") | qi::string(">=") | qi::string("==") | qi::string("!=");
        start = ( term >> cmp_op >> start)
                [qi::_val = phx::new_<OperatorNode>(qi::_2, qi::_1, qi::_3 ) ] |
                term [qi::_val = qi::_1] | qi::eps [ qi::_val = phx::new_<EmptyNode>()];
        term = (product >> product_op >> term)
               [qi::_val = phx::new_<OperatorNode>(qi::_2, qi::_1, qi::_3 ) ] |
               product [qi::_val = qi::_1];
        product = (factor >> factor_op >> product)
                  [qi::_val = phx::new_<OperatorNode>(qi::_2,qi::_1, qi::_3 ) ] |
                  factor [qi::_val = qi::_1];
        factor  = group [qi::_val = qi::_1]  |
                  qi::double_ [qi::_val = phx::new_<ConstantNode>(qi::_1) ] |
                  ( '"'>> str >> '"') [qi::_val = phx::new_<ConstantNode>(qi::_1) ] |
                  function_call [qi::_val = qi::_1] ;
        function_call = (identifier >> '(' >> arguments >>')') [qi::_val = phx::new_<FunctionNode>(qi::_1, qi::_2 ) ] | identifier  [qi::_val = phx::new_<IdentifierNode>(qi::_1)];

        arguments = (start [qi::_val = qi::_1] >> *(',' >>start)[qi::_val = qi::_1]) [qi::_val = phx::new_<ArgumentNode>(qi::_1,qi::_2)] ;

        group   %= '(' >> start >> ')';


#ifdef EXTRA_DEBUG
        identifier.name("identifier");
        product_op.name("product_op");
        factor_op.name("factor_op");
        cmp_op.name("cmp_op");
        start.name("start");
        term.name("term");
        product.name("product");
        factor.name("factor");
        function_call.name("function_call");
        arguments.name("arguments");
        group.name("group");
        qi::debug(identifier);
        qi::debug(product_op);
        qi::debug(factor_op);
        qi::debug(cmp_op);
        qi::debug(start);
        qi::debug(term);
        qi::debug(product);
        qi::debug(factor);
        qi::debug(function_call);
        qi::debug(arguments);
        qi::debug(group);
#endif

    }


    qi::rule<Iterator, std::string(), qi::space_type> identifier,str;
    qi::rule<Iterator, std::string(), qi::space_type> cmp_op;
    qi::rule<Iterator, std::string(), qi::space_type> product_op;
    qi::rule<Iterator, std::string(), qi::space_type> factor_op;
    qi::rule<Iterator, ASTNodePtr(), qi::space_type> arguments;
    qi::rule<Iterator, ASTNodePtr(), qi::space_type> start, group, term,product, factor,function_call;
};


struct result_visitor{
    std::string input;
    void operator()(bool x){
        std::cout << std::boolalpha<<"evaluate( "<<std::quoted(input)<< " ) = " << x << std::endl;
    }
    void operator()(double x){
        std::cout << std::boolalpha<<"evaluate( "<<std::quoted(input)<< " ) = " << x << std::endl;
    }
    void operator()(std::string x){
        std::cout << std::boolalpha<<"evaluate( "<<std::quoted(input)<< " ) = " << std::quoted(x) << std::endl;
    }
    void operator()(std::vector<arg_result> & x){
        std::cout << std::boolalpha<<"evaluate( "<<std::quoted(input)<< " ) = " << "std::vector<arg_result>" << std::endl;
    }
};
#endif //DATACONTAINER_GRAMMER_H
