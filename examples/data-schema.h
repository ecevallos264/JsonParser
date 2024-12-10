//
// Created by eceva on 12/1/2024.
//

#ifndef JSONPARSE_DATA_SCHEMA_H
#define JSONPARSE_DATA_SCHEMA_H

#include "../json-parser/schema.h"
#include "../json-parser/parsed-field.h"

class InnerObject: public Schema {
public:
    int a1;
    int a2;
    InnerObject() {
        this->bind("a1", &a1);
        this->bind("a2", &a2);
    }
};

class DataSchema: public Schema {
public:
    double aaa;
    int dd;
    std::shared_ptr<InnerObject> nestedObject;
public:
    DataSchema() {
        SchemaFactory::getInstance().registerType<InnerObject>("InnerObject");
        this->bind("aaa", &aaa);
        this->bind("dd", &dd);
        this->bindObject<InnerObject>("object", "InnerObject");

    }
};

#endif //JSONPARSE_DATA_SCHEMA_H
