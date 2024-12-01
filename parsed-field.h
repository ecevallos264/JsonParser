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

using map = std::unordered_map<std::string, class ParsedField>;
using list = std::vector<class ParsedField>;
using string = std::string;
using null = std::monostate;


class ParsedField {
public:

private:
    std::variant<null, int, unsigned int, float, bool, string, list, map> value;

public:
    ParsedField() : value(null {}) {}
    ParsedField(int v) : value(v) {}
    ParsedField(unsigned int v) : value(v) {}
    ParsedField(float v) : value(v) {}
    ParsedField(string v) : value(v) {}
    ParsedField(bool v) : value(v) {}
    ParsedField(map v) : value(v) {}
    ParsedField(list v) : value(v) {}

    template <typename T>
    T getValue() const {
        return std::get<T>(value);
    }

    std::optional<string> toStringOrNull() {
        if (std::holds_alternative<std::monostate>(value)) {
            return std::nullopt;
        }
        if (!std::holds_alternative<string>(value)) {
            throw IncorrectTypeException("string", type());
        }
        return std::get<string>(value);
    }

    std::optional<int> toIntOrNull() {
        if (std::holds_alternative<std::monostate>(value)) {
            return std::nullopt;
        }
        if (!std::holds_alternative<int>(value)) {
            throw IncorrectTypeException("int", type());
        }
        return std::get<int>(value);
    }

    std::optional<unsigned int> toUIntOrNull() {
        if (std::holds_alternative<std::monostate>(value)) {
            return std::nullopt;
        }
        if (!std::holds_alternative<unsigned int>(value)) {
            throw IncorrectTypeException("unsigned int", type());
        }
        return std::get<unsigned int>(value);
    }

    std::optional<float> toFloatOrNull() {
        if (std::holds_alternative<int>(value)) {
            return static_cast<float>(std::get<int>(value));
        }
        if (std::holds_alternative<unsigned int>(value)) {
            return static_cast<float>(std::get<unsigned int>(value));
        }
        if (std::holds_alternative<std::monostate>(value)) {
            return std::nullopt;
        }
        if (!std::holds_alternative<float>(value)) {
            throw IncorrectTypeException("float", type());
        }
        return std::get<float>(value);
    }

    std::optional<bool> toBooleanOrNull() {
        if (std::holds_alternative<std::monostate>(value)) {
            return std::nullopt;
        }
        if (!std::holds_alternative<bool>(value)) {
            throw IncorrectTypeException("bool", type());
        }
        return std::get<bool>(value);
    }

    std::optional<map> toMapOrNull() {
        if (std::holds_alternative<std::monostate>(value)) {
            return std::nullopt;
        }
        if (!std::holds_alternative<map>(value)) {
            throw IncorrectTypeException("map", type());
        }
        return std::get<map>(value);
    }
    std::optional<list> toListOrNull() {
        if (std::holds_alternative<std::monostate>(value)) {
            return std::nullopt;
        }
        if (!std::holds_alternative<list>(value)) {
            throw IncorrectTypeException("list", type());
        }
        return std::get<list>(value);
    }

    string toString() {
        if (!std::holds_alternative<string>(value)) {
            throw IncorrectTypeException("string", type());
        }
        if (isNull()) {
            throw NullPointerException("Value is Null");
        }
        return std::get<string>(value);
    }

    float toFloat() {
        if (std::holds_alternative<int>(value)) {
            return static_cast<float>(std::get<int>(value));
        }
        if (std::holds_alternative<unsigned int>(value)) {
            return static_cast<float>(std::get<unsigned int>(value));
        }
        if (isNull()) {
            throw NullPointerException("Value is Null");
        }
        return std::get<float>(value);
    }

    int toInt() {
        if (!std::holds_alternative<int>(value)) {
            throw IncorrectTypeException("int", type());
        }
        if (isNull()) {
            throw NullPointerException("Value is Null");
        }
        return std::get<int>(value);
    }

    unsigned int toUInt() {
        if (!std::holds_alternative<unsigned int>(value)) {
            throw IncorrectTypeException("unsigned int", type());
        }
        if (isNull()) {
            throw NullPointerException("Value is Null");
        }
        return std::get<unsigned int>(value);
    }

    bool toBoolean() {
        if (!std::holds_alternative<bool>(value)) {
            throw IncorrectTypeException("bool", type());
        }
        if (isNull()) {
            throw NullPointerException("Value is Null");
        }
        return std::get<bool>(value);
    }

    map toMap() {
        if (!std::holds_alternative<map>(value)) {
            throw IncorrectTypeException("map", type());
        }
        if (isNull()) {
            throw NullPointerException("Value is Null");
        }
        return std::get<map>(value);
    }

    list toList() {
        if (!std::holds_alternative<list>(value)) {
            throw IncorrectTypeException("array", type());
        }
        if (isNull()) {
            throw NullPointerException("Value is Null");
        }
        return std::get<list>(value);
    }

    bool isNull() {
        return std::holds_alternative<std::monostate>(value);
    }

    std::string type() const {
        return std::visit([](const auto& val) {
            using T = std::decay_t<decltype(val)>;
            if (std::is_same_v<T, int>) return "int";
            else if (std::is_same_v<T, float>) return "float";
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
