#ifndef JSONPARSE_TOKENS_H
#define JSONPARSE_TOKENS_H

enum Token {
    TOKEN_STRING,
    TOKEN_DECIMAL,
    TOKEN_INTEGER,
    TOKEN_UNSIGNED_INTEGER,
    TOKEN_ARRAY,
    TOKEN_BOOLEAN,
    TOKEN_NULL,
    TOKEN_COLON,
    TOKEN_COMMA,
    TOKEN_OPEN_BRACKET,
    TOKEN_CLOSE_BRACKET,
    TOKEN_OPEN_BRACE,
    TOKEN_CLOSE_BRACE,
    TOKEN_NONE,
};

std::string getTokenName(Token token) {
    switch (token) {
        case TOKEN_STRING: return "TOKEN_STRING";
//        case TOKEN_NUMBER: return "TOKEN_NUMBER";
        case TOKEN_DECIMAL: return "TOKEN_DECIMAL";
        case TOKEN_INTEGER: return "TOKEN_INTEGER";
        case TOKEN_UNSIGNED_INTEGER: return "TOKEN_UNSIGNED_INTEGER";
        case TOKEN_ARRAY: return "TOKEN_ARRAY";
        case TOKEN_BOOLEAN: return "TOKEN_BOOLEAN";
        case TOKEN_NULL: return "TOKEN_NULL";
        case TOKEN_COLON: return "TOKEN_COLON";
        case TOKEN_COMMA: return "TOKEN_COMMA";
        case TOKEN_OPEN_BRACKET: return "TOKEN_OPEN_BRACKET";
        case TOKEN_CLOSE_BRACKET: return "TOKEN_CLOSE_BRACKET";
        case TOKEN_OPEN_BRACE: return "TOKEN_OPEN_BRACE";
        case TOKEN_CLOSE_BRACE: return "TOKEN_CLOSE_BRACE";
        case TOKEN_NONE: return "TOKEN_NONE";
        default: return "UNKNOWN_TOKEN";
    }
}

struct Token_Data {
    int line, col;
    Token token;
    void *value;
};

#endif //JSONPARSE_TOKENS_H
