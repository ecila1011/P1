/**
 * @file p1-lexer.c
 * @brief Compiler phase 1: lexer
 */
#include "p1-lexer.h"

/* declare keyword and reserved array */
const char *keys[] = {"def", "if", "else", "while", "return", "break", "continue", "int", "bool", "void", "true", "false"};
const char *reserved[] = {"for", "callout", "class", "interface", "extends", "implements", "new", "this", "string", "float", "double", "null"};
const int keysSize = 12;
const int reservedSize = 12;
/*Initialize function*/
bool isKey(char *key);
bool isReserved(char *key);

TokenQueue *lex(char *text)
{
    TokenQueue *tokens = TokenQueue_new();

    /* compile regular expressions */
    Regex *whitespace = Regex_new("^[ \n]");
    Regex *endofline = Regex_new("[ ]*\n");
    Regex *letter = Regex_new("^[a-z]");                                            // this isnt actually being used anywhere from what i can tell
    Regex *symbol = Regex_new("\\(|\\)|\\+|\\*|-|!|%|<|<=|>=|>|==|!=|&&|(\\|\\|;)"); // took the \ from the lab3 doc
    Regex *integer = Regex_new("0|([1-9][0-9]*)");
    Regex *identifiers = Regex_new("^[a-zA-Z][a-zA-Z0-9_]*");
    Regex *hexidecimal = Regex_new("^0x[a-f0-9][a-f0-9]*");
    Regex *comments = Regex_new("^[//][^\n]*");
    Regex *strings = Regex_new("^\"[a-zA-Z]*\""); // placeholder, not sure this is correct

    /* read and handle input */
    char match[MAX_TOKEN_LEN];
    int line = 1;

    if (text == NULL)
    {
        return tokens;
    }

    while (*text != '\0')
    {

        /* match regular expressions */
        if (Regex_match(whitespace, text, match)) // ignore whitespace
        {
            // add to line count
            // I don't know if this actually should go here
            if (Regex_match(endofline, text, match)) // ignore whitespace
            {
                line = line + 1;
            }
        }
        else if (Regex_match(hexidecimal, text, match)) // hex
        {
            TokenQueue_add(tokens, Token_new(HEXLIT, match, line));
        }
        else if (Regex_match(strings, text, match)) // strings
        {
            TokenQueue_add(tokens, Token_new(STRLIT, match, line));
        }
        else if (Regex_match(identifiers, text, match)) // starts off as identifier
        {
            if (isKey(text)) // check to see if id is a keyword
            {
                TokenQueue_add(tokens, Token_new(KEY, match, line));
            }
            else if (isReserved(text)) // check to see if id is a reserved word
            {
                Error_throw_printf("Invalid token!\n");
            }
            else
            {
                TokenQueue_add(tokens, Token_new(ID, match, line));
            }
        }
        else if (Regex_match(integer, text, match)) // integer
        {
            TokenQueue_add(tokens, Token_new(DECLIT, match, line));
        }
        else if (Regex_match(symbol, text, match)) // symbol
        {
            TokenQueue_add(tokens, Token_new(SYM, match, line));
        }
        else if (Regex_match(comments, text, match)) // symbol
        {
            // Nothing. End of line character is handled in whitespace
        }
        else // invalid token
        {
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

/* use this method to check if the found token is a keyword*/
bool isKey(char *key)
{
    bool isKey = false;
    int i = 0;

    while (i < keysSize && !isKey)
    {
        if (strcmp(key, keys[i]) == 0)
        {
            isKey = true;
        }
        i += 1;
    }

    return isKey;
}

/* use this method to check if the found token is a reserved word*/
bool isReserved(char *key)
{
    bool isReserved = false;
    int i = 0;

    while (i < reservedSize && !isReserved)
    {
        if (strcmp(key, reserved[i]) == 0)
        {
            isReserved = true;
        }
        i += 1;
    }

    return isReserved;
}
