//
// Created by cefour on 08.06.22.
//

#ifndef DATACONTAINER_GRAMMER_H
#define DATACONTAINER_GRAMMER_H

#include "ast_node.h"
#include "empty_node.h"
#include "operator_node.h"
#include "constant_node.h"
#include "function_node.h"
#include "identifier_node.h"
#include "argument_node.h"
#include "if_node.h"

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
bool parse_grammar(
        const std::string& input, const Parser& p, const Skipper& s,
        Args&& ... args)
{
    std::string::const_iterator begin = input.begin(), end = input.end();
    boost::spirit::qi::phrase_parse(
            begin, end, p, s, std::forward<Args>(args) ...);
    if (begin != end) {
        std::cout << "Unparseable: "
                  << std::quoted(std::string(begin, end)) << std::endl;
        return false;
    }
    return true;
}

namespace ptl{
    template <typename T>
    struct make_shared_f
    {
        template <typename... A> struct result
        { typedef std::shared_ptr<T> type; };

        template <typename... A>
        typename result<A...>::type operator()(A&&... a) const {
            return std::make_shared<T>(std::forward<A>(a)...);
        }
    };

    template <typename T>
    using make_shared_ = boost::phoenix::function<make_shared_f<T> >;
}



template<class ITERATOR = std::string::const_iterator>
class constraint_grammar
        : public qi::grammar<ITERATOR, ASTNodePtr(), qi::space_type>{
public:
    using Iterator = ITERATOR;
    constraint_grammar() : constraint_grammar::base_type(start) {
        str %= *(qi::char_ - '"');
        identifier %= qi::alpha >> *qi::alnum;
        product_op = qi::string("+") | qi::string("-");
        factor_op = qi::string("*") | qi::string("/");
        cmp_op =  qi::string("<=") | qi::string(">=")|qi::string("<")   |  qi::string(">") | qi::string("==") |
                 qi::string("!=");

        if_clause = ( qi::lit("if") >> '(' >> start >> ')'>> start >> -(qi::lit("else") >> start) ) [ qi::_val = ptl::make_shared_<if_node>()(qi::_1,qi::_2,qi::_3) ];

        start = if_clause [ qi::_val = qi::_1 ]|
                (term >> cmp_op >> start)
                [qi::_val = ptl::make_shared_<operator_node>()(qi::_2, qi::_1, qi::_3)] |
                term[qi::_val = qi::_1] | qi::eps[qi::_val = ptl::make_shared_<empty_node>()()];
        term = (product >> product_op >> term)
               [qi::_val = ptl::make_shared_<operator_node>()(qi::_2, qi::_1, qi::_3)] |
               product[qi::_val = qi::_1];
        product = (factor >> factor_op >> product)
                  [qi::_val = ptl::make_shared_<operator_node>()(qi::_2, qi::_1, qi::_3)] |
                  factor[qi::_val = qi::_1];
        factor = group[qi::_val = qi::_1] |
                 qi::double_[qi::_val = ptl::make_shared_<constant_node>()(qi::_1)] |
                 ('"' >> str >> '"')[qi::_val = ptl::make_shared_<constant_node>()(qi::_1)] |
                 function_call[qi::_val = qi::_1];
        function_call = (identifier >> '(' >> arguments >> ')')[qi::_val = ptl::make_shared_<function_node>()(qi::_1, qi::_2)] |
                        identifier[qi::_val = ptl::make_shared_<identifier_node>()(qi::_1)];

        arguments = (start[qi::_val = qi::_1] >> *(',' >> start)[qi::_val = qi::_1])[qi::_val = ptl::make_shared_<argument_node>()(qi::_1, qi::_2)];

        group %= '(' >> start >> ')';


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
    qi::rule<Iterator, ASTNodePtr(), qi::space_type> if_clause;
    qi::rule<Iterator, ASTNodePtr(), qi::space_type> start, group, term,product, factor,function_call;
};






#endif //DATACONTAINER_GRAMMER_H
