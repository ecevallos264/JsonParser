//
// Created by eceva on 12/2/2024.
//

#ifndef JSONPARSE_SCHEMA_FACTORY_H
#define JSONPARSE_SCHEMA_FACTORY_H

#include <string>
#include <unordered_map>
#include <memory>
#include <functional>
#include "singleton.h"
#include "schema.h"

class SchemaFactory : public Singleton<SchemaFactory> {
private:
    std::unordered_map<std::string, std::function<std::shared_ptr<Schema>()>> creators;

public:
    SchemaFactory() : Singleton<SchemaFactory>() {}

    template <typename T>
    void registerType(std::string typeName) {
        static_assert(std::is_base_of<Schema, T>::value, "T must derive from Schema");
        creators[typeName] = []() -> std::shared_ptr<Schema> {
            return std::make_shared<T>();
        };
    }

    std::function<std::shared_ptr<Schema>()> getType(const std::string& typeName) const {
        auto it = creators.find(typeName);
        if (it != creators.end()) {
            return it->second;
        }
        throw std::runtime_error("Schema type not registered: " + typeName);
    }

    std::shared_ptr<Schema> createInstance(const std::string& typeName) const {
        auto it = creators.find(SchemaHandler::);
        if (it != creators.end()) {
            return it->second();
        }
        throw std::runtime_error("Schema type not registered: " + typeName);
    }

    bool isTypeRegistered(const std::string& typeName) const {
        return creators.find(typeName) != creators.end();
    }
};

#endif // JSONPARSE_SCHEMA_FACTORY_H