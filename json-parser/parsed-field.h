//
// Created by eceva on 11/29/2024.
//

#ifndef JSONPARSE_PARSED_FIELD_H
#define JSONPARSE_PARSED_FIELD_H

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <optional>
#include "exceptions/NullPointerException.h"
#include "exceptions/IncorrectTypeException.h"
#include "schema.h"

using map = std::unordered_map<std::string, class ParsedField>;
using list = std::vector<class ParsedField>;
using string = std::string;
using null = std::monostate;


class ParsedField {
public:

private:
    std::variant<null, int, unsigned int, double, bool, string, list, map, std::shared_ptr<Schema>> value;

public:
    ParsedField() : value(null {}) {}
    ParsedField(int v) : value(v) {}
    ParsedField(unsigned int v) : value(v) {}
    ParsedField(double v) : value(v) {}
    ParsedField(string v) : value(v) {}
    ParsedField(bool v) : value(v) {}
    ParsedField(map v) : value(v) {}
    ParsedField(list v) : value(v) {}
    ParsedField(std::shared_ptr<Schema> v) : value(v) {}

    template <typename T>
    T getValue() const {
        return std::get<T>(value);
    }

    bool isNull() {
        return std::holds_alternative<std::monostate>(value);
    }

    std::string type() const {
        return std::visit([](const auto& val) {
            using T = std::decay_t<decltype(val)>;
            if (std::is_same_v<T, int>) return "int";
            else if (std::is_same_v<T, double>) return "double";
            else if (std::is_same_v<T, int>) return "int";
            else if (std::is_same_v<T, unsigned int>) return "unsigned int";
            else if (std::is_same_v<T, std::string>) return "string";
            else if (std::is_same_v<T, bool>) return "bool";

            else if (std::is_same_v<T, map>) return "map";
            else if (std::is_same_v<T, list>) return "list";
            else return "unknown";
        }, value);
    }
};

#endif //JSONPARSE_PARSED_FIELD_H