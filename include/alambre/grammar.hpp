#ifndef ALAMBRE_GRAMMAR_HPP
#define ALAMBRE_GRAMMAR_HPP

#include <alambre/ast.hpp>
#include <boost/spirit/include/qi.hpp>
#include <vector>

namespace qi = boost::spirit::qi;

template<typename Iterator>
class alaGrammar : public boost::spirit::qi::grammar<Iterator> {

  public:

    alaGrammar() : alaGrammar::base_type(root) {

        root = *(statement);

        statement = (
            empty_statement | if_statement
        );

        empty_statement = qi::token(TOK_NEWLINE);

        if_statement = (
            qi::token(TOK_IF) >>
            expression >>
            qi::token(TOK_COLON) >>
            statement_block
        );

        expression = (
            call_expression | symbol_ref_expression
        );

        statement_block = (
            qi::token(TOK_INDENT) >>
            +(statement) >>
            qi::token(TOK_OUTDENT)
        );

        call_expression = (
            expression >> '(' >> ')'
        );

        symbol_ref_expression = (
            qi::token(TOK_IDENT)
        );

    }

    qi::rule<Iterator> root;
    qi::rule<Iterator> if_statement;
    qi::rule<Iterator> empty_statement;
    qi::rule<Iterator> call_expression;
    qi::rule<Iterator> symbol_ref_expression;
    qi::rule<Iterator> statement_block;
    qi::rule<Iterator> statement;
    qi::rule<Iterator> expression;
};

#endif
