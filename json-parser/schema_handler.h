//
// Created by eceva on 12/1/2024.
//

#ifndef JSONPARSE_SCHEMA_HANDLER_H
#define JSONPARSE_SCHEMA_HANDLER_H

#include <string>
#include <unordered_map>
#include <functional>
#include <iostream>
#include <memory>
#include "parsed-field.h"

class Schema {
private:
    std::unordered_map<std::string, std::function<void(const ParsedField&)>> bindings;
    std::unordered_map<std::string, std::function<std::shared_ptr<Schema>()>> nestedSchemaFactories;

protected:
    template<typename T, typename U>
    void bind(const std::string& name, T U::*member) {
        bindings[name] = [this, member](const ParsedField& field) {
            if constexpr (std::is_base_of<Schema, T>::value) {
                (this->*member) = *field.get<std::shared_ptr<T>>();
            } else {
                (this->*member) = field.get<T>();
            }
        };
        if constexpr (std::is_base_of<Schema, T>::value) {
            nestedSchemaFactories[name] = []() { return std::make_shared<T>(); };
        }
    }

public:
    void set(const std::string& name, const ParsedField& value) {
        auto it = bindings.find(name);
        if (it != bindings.end()) {
            it->second(value);
        } else {
            std::cerr << "No binding found for: " << name << std::endl;
        }
    }

    std::shared_ptr<Schema> createNestedSchema(const std::string& fieldName) const {
        auto it = nestedSchemaFactories.find(fieldName);
        if (it != nestedSchemaFactories.end()) {
            return it->second();
        }
        return nullptr;
    }
};

#endif //JSONPARSE_SCHEMA_HANDLER_H
