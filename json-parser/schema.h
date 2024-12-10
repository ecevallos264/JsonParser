#ifndef JSONPARSE_SCHEMA_H
#define JSONPARSE_SCHEMA_H

#include <string>
#include <unordered_map>
#include <functional>
#include <iostream>
#include <memory>
#include <type_traits>
#include "singleton.h"

class Schema {
private:
    // Bindings for both primitive fields and nested schema objects
    std::unordered_map<std::string, std::function<void(void*)>> bindings;\
    std::unordered_map<std::string, std::string> classMappings;

protected:
    template <typename T>
    void bind(const std::string& name, T* member) {
        bindings[name] = [member](void* value) {
            if constexpr (std::is_base_of<Schema, T>::value) {
                *member = *static_cast<T*>(value);
            } else {
                *member = *static_cast<T*>(value);
            }
        };
    }

    template <typename T>
    void bindObject(const std::string& fieldName, const std::string& className) {
        static_assert(std::is_base_of<Schema, T>::value, "T must derive from Schema");
        classMappings[fieldName] = className;
    }


    template <typename T>
    void bindArray(const std::string& name, std::vector<T>* member) {
        bindings[name] = [member](void* value) {
            *member = *static_cast<std::vector<T>*>(value);
        };
    }

public:
    virtual ~Schema() = default;
    void set(const std::string& name, void* value) {
        auto it = bindings.find(name);
        if (it != bindings.end()) {
            it->second(value);
        } else {
            std::cerr << "No binding found for field: " << name << std::endl;
        }
    }

    void set(const std::string& name, std::shared_ptr<Schema> instance) {
        auto it = bindings.find(name);
        if (it != bindings.end()) {
            it->second(instance.get());
        } else {
            std::cerr << "No binding found for field: " << name << std::endl;
        }
    }

    template <typename T>
    void set(const std::string& name, const std::vector<T>& array) {
        auto it = bindings.find(name);
        if (it != bindings.end()) {
            std::vector<T>* value = new std::vector<T>(array);
            it->second(static_cast<void*>(value));
            delete value;
        } else {
            std::cerr << "No binding found for array field: " << name << std::endl;
        }
    }

    std::string getClassName(const std::string& fieldName) {
        auto it = classMappings.find(fieldName);
        if (it != classMappings.end()) {
            return it->second;
        } else {
            throw std::runtime_error("Class has not been binded");
        }
    }
};

#endif // JSONPARSE_SCHEMA_H
