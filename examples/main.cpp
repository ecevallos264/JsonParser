#include <iostream>
#include "../json-parser/parser.h"
#include "data-schema.h"

#define JSON_FILE "C:\\Users\\eceva\\CLionProjects\\JsonParse\\examples\\data.json"

int main() {
    auto object = Parser::getInstance().parseFile<DataSchema>(JSON_FILE);
    DataSchema schema;
//    schema.set("innerObject", object["innerObject"].toMap());
    std::cout << object.aaa << std::endl;
    return 0;
}

