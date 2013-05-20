#ifndef ALAMBRE_AST_HPP
#define ALAMBRE_AST_HPP

#include <string>
#include <vector>

#include <boost/fusion/adapted.hpp>
#include <boost/fusion/include/adapted.hpp>

namespace ala {

struct expression {};

struct statement {};

struct symbol_ref_expression : public expression {
    std::string ident;
};

struct call_expression : public expression {
    expression operand;
};

struct statement_block {
    std::vector<statement> statements;
};

struct module {
    std::vector<statement> statements;
};

struct if_statement : public statement {
    expression expr;
    statement_block true_stmts;
};

}

BOOST_FUSION_ADAPT_STRUCT(
    ala::symbol_ref_expression,
    (std::string, ident)
)

#endif
