//
// Created by eceva on 12/1/2024.
//

#ifndef JSONPARSE_OPTIONAL_H
#define JSONPARSE_OPTIONAL_H

#include <stdexcept>
#include <variant>
#include <optional>

template <typename T>
class Optional {
private:
    T value;
    bool isInitialized;
public:
    Optional() : isInitialized(false) {}
    Optional(const T& val) : value(val), isInitialized(true) {}

    bool isEmpty() {
        return !this->isInitialized;
    }

    void setValue(T val) {
        this->isInitialized = true;
        this->value = val;
    }

    T getValue() {
        if(!this->isInitialized) {
            throw std::runtime_error("Value is NULL");
        }
        return this->value;
    }

    T getValue(T optional) {
        if(!this->isInitialized) {
            return optional;
        }
        return this->value;
    }

    std::variant<T, std::monostate> getValueOrNull() {
        if(!this->isInitialized) {
            return std::nullopt;
        }
        return this->value;
    }
};

#endif //JSONPARSE_OPTIONAL_H