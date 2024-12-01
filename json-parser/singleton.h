//
// Created by eceva on 11/30/2024.
//

#ifndef JSONPARSE_SINGLETON_H
#define JSONPARSE_SINGLETON_H

template <typename T>
class Singleton {
public:
    static T& getInstance() {
        static T instance;
        return instance;
    }

protected:
    Singleton() {}
    ~Singleton() {}

private:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

#endif //JSONPARSE_SINGLETON_H
