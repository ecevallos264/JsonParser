//
// Created by eceva on 12/10/2024.
//

#ifndef JSONPARSE_INSTANCE_HANDLER_H
#define JSONPARSE_INSTANCE_HANDLER_H

#include <memory>
#include <stack>
#include "singleton.h"
#include "schema.h"

class InstanceHandler : public Singleton<InstanceHandler> {
private:
    std::stack<std::shared_ptr<Schema>> instances;
public:
    InstanceHandler(): Singleton<InstanceHandler>() {};
public:
    void pushNewInstance(std::shared_ptr<Schema> newInstance) {
        instances.push(newInstance);
    }

    std::shared_ptr<Schema> getCurrentInstance() {
        return instances.top();
    }

    std::shared_ptr<Schema> pop() {
        if(instances.size() <= 0) {
            return nullptr;
        }
        std::shared_ptr<Schema> instance = instances.top();
        instances.pop();
        return instance;
    }

    std::shared_ptr<Schema> peekThenPop() {
        std::shared_ptr<Schema> instance = instances.top();
        instances.pop();
        return instance;
    }
};

#endif //JSONPARSE_INSTANCE_HANDLER_H
