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
    Regex *whitespace = Regex_new("^[ \r\n\t]");
    Regex *endofline = Regex_new("[\n");
    Regex *letter = Regex_new("^[a-z]"); // this isnt actually being used anywhere from what i can tell
    // Regex *symbol = Regex_new("\\(|\\)|\\+|\\*|-|!|%|<|<=|>=|>|==|!=|&&|(\\|\\|;)"); // I think we're missing a few
    Regex *symbol = Regex_new("\\(|\\)|\\{|\\}|\\[|\\]|\\+|\\*|-|!|%|<|<=|>=|>|=|==|!=|&&|(\\|\\|)|;");
    Regex *integer = Regex_new("^0|(^[1-9][0-9]*)");
    Regex *identifiers = Regex_new("^[a-zA-Z][a-zA-Z0-9_]*");
    Regex *hexidecimal = Regex_new("^0x[a-f0-9][a-f0-9]*");
    Regex *comments = Regex_new("^[//][^\n]*");
    Regex *strings = Regex_new("^\"[a-zA-Z]*\""); // placeholder, not sure this is correct

    /* read and handle input */
    char match[MAX_TOKEN_LEN];
    int line = 1;

    if (text == NULL)
    {
        Error_throw_printf("Invalid token! 1\n");
        return tokens;
    }

    while (*text != '\0')
    {

        /* match regular expressions */
        if (Regex_match(whitespace, text, match))
        {
            // ignore whitespace
            // Check for end of line
            char eol [1] = "\n";
            if (strstr(match, eol) != NULL) // increment line number when finding \n
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
            if (isKey(match)) // check to see if id is a keyword
            {
                TokenQueue_add(tokens, Token_new(KEY, match, line));
            }
            else if (isReserved(match)) // check to see if id is a reserved word
            {
                Error_throw_printf("Invalid token! 2\n");
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
        else if (Regex_match(comments, text, match)) // comments
        {
            // Nothing. End of line character is handled in whitespace
        }
        else // invalid token
        {
            Error_throw_printf("Invalid token! 3\n");
        }

        /* skip matched text to look for next token */
        text += strlen(match);
        // memset(match, '\0', MAX_TOKEN_LEN);
    }

    /* clean up */
    Regex_free(whitespace);
    Regex_free(letter);
    Regex_free(endofline);
    Regex_free(symbol);
    Regex_free(integer);
    Regex_free(identifiers);
    Regex_free(hexidecimal);
    Regex_free(comments);
    Regex_free(strings);

    return tokens;
}

/* use this method to check if the found token is a keyword*/
bool isKey(char *key)
{

    //Regex *keys[] = {Regex_new("^def\\s*"), Regex_new("if\\s*"), Regex_new("else\\s*"), Regex_new("while\\s*"), Regex_new("return\\s*"), Regex_new("^break\\s*"),
    // Regex_new("continue\\s*"), Regex_new("^i^n^t\\s*"), Regex_new("bool\\s*"), Regex_new("void\\s*"), Regex_new("true\\s*"), Regex_new("^false\\s*")};
    bool isKey = false;
    int i = 0;
    // char match[MAX_TOKEN_LEN];

    // while (i < keysSize && !isKey)
    // {
    //     if (Regex_match(keys[i], key, match))
    //     {
    //         isKey = true;
    //     }
    //     i += 1;
    // }

    while (i < keysSize && !isKey)
    {
        if (!strcmp(key, keys[i]))
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
    // Regex *reserved[] = {Regex_new("for\\s*"), Regex_new("callout\\s*"), Regex_new("class\\s*"), Regex_new("interface\\s*"), Regex_new("extends\\s*"), Regex_new("implements\\s*"),
    //Regex_new("new\\s*"), Regex_new("this\\s*"), Regex_new("string\\s*"), Regex_new("float\\s*"), Regex_new("double\\s*"), Regex_new("null\\s*")};

    bool isReserved = false;
    int i = 0;
    //char match[MAX_TOKEN_LEN];

    // while (i < reservedSize && !isReserved)
    // {
    //     if (Regex_match(reserved[i], key, match))
    //     {
    //         isReserved = true;
    //     }
    //     i += 1;
    // }

    while (i < reservedSize && !isReserved)
    {
        if (!strcmp(key, reserved[i]))
        {
            isReserved = true;
        }
        i += 1;
    }

    return isReserved;
}
