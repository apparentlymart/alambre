
#include <alambre/scanner.hpp>
#include <alambre/ast.hpp>
#include <alambre/grammar.hpp>

int main(int argc, char **argv) {

    typedef lex::lexertl::token<char const*> token_type;
    typedef lex::lexertl::actor_lexer<token_type> lexer_type;

    alaLexer<lexer_type> lexer;

    std::string str("if foo:\n    baz()\n");
    char const* first = str.c_str();
    char const* last = &first[str.size()];

    lexer_type::iterator_type iter = lexer.begin(first, last);
    lexer_type::iterator_type end = lexer.end();

    /*namespace qi = boost::spirit::qi;

    qi::rule<lexer_type::iterator_type> if_stmt = (
        qi::token(TOK_IF) >> qi::token(TOK_IDENT) >> qi::token(TOK_COLON) >>
        qi::token(TOK_INDENT) >> qi::token(TOK_IDENT) >>
        qi::char_('(') >> qi::char_(')')
        );

    bool parse_result = lex::tokenize_and_parse(first, last, lexer, if_stmt);*/

    alaGrammar<lexer_type::iterator_type> grammar;

    bool parse_result = lex::tokenize_and_parse(first, last, lexer, grammar.root);


    if (parse_result) {
        cout << "Looks like it worked\n";
    }
    else {
        cout << "Looks like it didn't work\n";
    }

}
