#include <iostream>
#include "parser.h"

#define JSON_FILE "C:\\Users\\eceva\\CLionProjects\\JsonParse\\data.json"

int main() {
    map object = Parser::getInstance().parse_file(JSON_FILE).toMap();

    std::cout << object["aaa"].toFloatOrNull().has_value() << std::endl;
    return 0;
}
