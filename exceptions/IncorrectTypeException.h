//
// Created by eceva on 11/30/2024.
//

#ifndef JSONPARSE_INCORRECTTYPEEXCEPTION_H
#define JSONPARSE_INCORRECTTYPEEXCEPTION_H

#include <exception>
#include <string>
#include <format>

#include <exception>
#include <string>
#include <format> // Requires C++20

class IncorrectTypeException : public std::exception {
    std::string full_message;

public:
    explicit IncorrectTypeException(std::string targetType, std::string originType)
            : full_message(buildMessage(targetType, originType)) {}

    const char *what() const noexcept override {
        return full_message.c_str();
    }

private:
    static std::string buildMessage(std::string targetType, std::string originType) {
        return std::format("[Incorrect Data Type Exception] Cannot convert type {} to {}",
                           originType, targetType);
    }
};


#endif //JSONPARSE_INCORRECTTYPEEXCEPTION_H
