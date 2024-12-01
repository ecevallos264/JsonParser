#ifndef JSONPARSE_SYNTAXERROREXCEPTION_H
#define JSONPARSE_SYNTAXERROREXCEPTION_H

#include <exception>
#include <string>
#include "../../../JsonParserLib/tokens.h"

class SyntaxErrorException : public std::exception {
    std::string full_message;
    int line;
    int col;

public:
    SyntaxErrorException(int line, int col, const std::string &message)
            : full_message(buildMessage(message, {line, col, TOKEN_NONE})), line(line), col(col) {}

    SyntaxErrorException(const Token_Data &data, const std::string &message)
            : full_message(buildMessage(message, data)), line(data.line), col(data.col) {}

    const char *what() const noexcept override {
        return full_message.c_str();
    }

private:
    static std::string buildMessage(const std::string &message, Token_Data data) {
        return "[Syntax Error][" + getTokenName(data.token) + "] " + message +
               " at line " + std::to_string(data.line) +
               " column " + std::to_string(data.col);
    }
};

#endif //JSONPARSE_SYNTAXERROREXCEPTION_H
