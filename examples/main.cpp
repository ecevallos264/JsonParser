#include <iostream>
#include "../json-parser/parser.h"

#define JSON_FILE "\\path\\to\\data.json" // <---- Change to real path

int main() {
    map object = Parser::getInstance().parse_file(JSON_FILE).toMap();

    std::cout << object["aaa"].toFloatOrNull().has_value() << std::endl;
    return 0;
}
