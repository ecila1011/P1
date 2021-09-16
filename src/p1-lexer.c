/**
 * @file p1-lexer.c
 * @brief Compiler phase 1: lexer
 */
#include "p1-lexer.h"

// TokenQueue* lex (char* text)
// {
//     TokenQueue* tokens = TokenQueue_new();

//     return tokens;
// }

TokenQueue* lex (char* text)
{
    TokenQueue* tokens = TokenQueue_new();

    /* compile regular expressions */
    Regex* whitespace = Regex_new("^[ \n]");
    Regex* letter = Regex_new("^[a-z]");
    Regex* symbol = Regex_new("\\(|\\)|\\+|\\*|-|!|%|<|<=|>=|>|==|!=|&&|(\\|\\|)"); // took the \ from the lab3 doc
    Regex* integer = Regex_new("0|([1-9][0-9]*)");
    Regex* identifiers = Regex_new("^[a-zA-Z][a-zA-Z0-9_]*");
    Regex* hexidecimal = Regex_new("^0x[a-f0-9][a-f0-9]*");
    Regex* strings = Regex_new();

    /* read and handle input */
    char match[MAX_TOKEN_LEN];
    while (*text != '\0') {

        /* match regular expressions */
        if (Regex_match(whitespace, text, match)) {
            /* ignore whitespace */
        } else if (Regex_match(hexidecimal, text, match)) {
            /* TODO: implement line count and replace placeholder (-1) */
            TokenQueue_add(tokens, Token_new(HEXLIT, match, -1));
        } else if (Regex_match(strings, text, match)) {
            /* TODO: implement line count and replace placeholder (-1) */
            TokenQueue_add(tokens, Token_new(ID, match, -1));
        } else if (Regex_match(identifiers, text, match)) {
            /* TODO: implement line count and replace placeholder (-1) */
            TokenQueue_add(tokens, Token_new(ID, match, -1));
        } else if (Regex_match(integer, text, match)) {
            /* TODO: implement line count and replace placeholder (-1) */
            TokenQueue_add(tokens, Token_new(DECLIT, match, -1));
        } else if (Regex_match(symbol, text, match)) {
            /* TODO: implement line count and replace placeholder (-1) */
            TokenQueue_add(tokens, Token_new(SYM, match, -1));
        }  else {
            Error_throw_printf("Invalid token!\n");
        }

        /* skip matched text to look for next token */
        text += strlen(match);
    }

    /* clean up */
    Regex_free(whitespace);
    Regex_free(letter);

    return tokens;
}


