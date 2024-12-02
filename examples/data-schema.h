//
// Created by eceva on 12/1/2024.
//

#ifndef JSONPARSE_DATA_SCHEMA_H
#define JSONPARSE_DATA_SCHEMA_H

#include "../json-parser/schema_handler.h"
#include "../json-parser/parsed-field.h"

class InnerObject: public Schema {
public:
    int a;
    InnerObject() {
        this->bind("a", &InnerObject::a);
    }
};

class DataSchema: public Schema {
public:
    float aaa;
    int dd;
    InnerObject innerObject;
public:
    DataSchema() {
        this->bind("aaa", &DataSchema::aaa);
        this->bind("dd", &DataSchema::dd);
        this->bind("innerObject", &DataSchema::innerObject);
    }
};

#endif //JSONPARSE_DATA_SCHEMA_H
