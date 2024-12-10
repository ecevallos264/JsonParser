#include <iostream>
#include "../json-parser/parser.h"
#include "../json-parser/schema-factory.h"
#include "data-schema.h"

#define JSON_FILE "C:\\Users\\eceva\\CLionProjects\\JsonParse\\examples\\data.json"

int main() {
    SchemaFactory::getInstance().registerType<DataSchema>("RootSchema");
    auto object = Parser::getInstance().parse_file<DataSchema>(JSON_FILE, "RootSchema");

    DataSchema schema;
//    schema.set("innerObject", object["innerObject"].toMap());
    std::cout << object->aaa << std::endl;
    return 0;
}

