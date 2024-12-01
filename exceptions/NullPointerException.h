//
// Created by eceva on 11/30/2024.
//

#ifndef JSONPARSE_NULLPOINTEREXCEPTION_H
#define JSONPARSE_NULLPOINTEREXCEPTION_H

#include <exception>
#include <string>
#include "../../JsonParserLib/tokens.h"

class NullPointerException : public std::exception {
    std::string full_message;

public:
    explicit NullPointerException(const std::string &message)
            : full_message(buildMessage(message)) {}

    const char *what() const noexcept override {
        return full_message.c_str();
    }

private:
    static std::string buildMessage(const std::string &message) {
        return "[Null Pointer Exception] " + message;
    }
};

#endif //JSONPARSE_NULLPOINTEREXCEPTION_H