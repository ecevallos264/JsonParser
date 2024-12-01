The **JSON Parser Library** is a lightweight C++ library for parsing JSON data from files and strings. This library provides a convenient way to work with JSON objects and arrays, offering robust error handling and flexibility in accessing and manipulating JSON fields.

## Features

- Parse JSON data from files and strings.
- Support for standard JSON data types: objects, arrays, strings, numbers, booleans, and null values.
- Strong type checking and optional value retrieval for safe and error-free operations.
- Extensible design with clean error reporting.

---

## Getting Started

### Importing the Library with CMake

To integrate the JSON Parser Library into your project, add the following to your 'CMakeLists.txt' file:

```cmake
cmake_minimum_required(VERSION 3.29)
project(JsonParse)

set(CMAKE_CXX_STANDARD 20)

add_executable(JsonParse main.cpp)

add_library(json_parser INTERFACE)
target_include_directories(json_parser INTERFACE
    "${CMAKE_SOURCE_DIR}/json-parser"
)

target_link_libraries(JsonParse PRIVATE json_parser)
```

Replace 'json-parser' with the directory where the library's source files are located.

---

### Parsing JSON from a File

Use the 'Parser::parse_file' method to parse JSON from a file. Here's an example:

```cpp
#include <iostream>
#include "json-parser/parser.h"

#define JSON_FILE "path/to/your/data.json"

int main() {
    try {
        std::map<std::string, ParsedField> object = Parser::getInstance().parse_file(JSON_FILE).toMap();
        std::cout << "Parsed value: " << object["aaa"].toFloatOrNull().value_or(0.0f) << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
```

---

### Parsing JSON from a String

Use the 'Parser::parse_contents' method to parse JSON from a string. Example:

```cpp
#include <iostream>
#include "json-parser/parser.h"

int main() {
    std::string jsonString = R"({
        "aaa": 111.1,
        "dd": [12222, 22, 2.2, "example"],
        "cc": {
            "cc1": [
                {"cc1_1": 1, "cc1_2": 2},
                {"cc1_1": 3, "cc1_2": -2}
            ],
            "cc2": 222
        }
    })";

    try {
        std::map<std::string, ParsedField> object = Parser::getInstance().parse_contents(jsonString).toMap();
        std::cout << "Parsed value: " << object["aaa"].toFloat() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
```

---

## Supported Data Types

The library uses custom datatypes for representing JSON fields. Below are the supported types and their equivalents:

| **Type**     | **C++ Equivalent**          |
|--------------|-----------------------------|
| 'null'       | 'std::monostate'            |
| 'int'        | 'int'                       |
| 'unsigned int' | 'unsigned int'            |
| 'float'      | 'float'                     |
| 'bool'       | 'bool'                      |
| 'string'     | 'std::string'               |
| 'list'       | 'std::vector<ParsedField>'  |
| 'map'        | 'std::unordered_map<std::string, ParsedField>' |

---

## Error Handling

The library uses exceptions for error handling. Below are the common exceptions:

- **SyntaxErrorException**: Thrown for invalid JSON syntax.
- **NullPointerException**: Accessing a null value without validation.
- **IncorrectTypeException**: Attempting to retrieve a field with the wrong type.

---

## Contributing

Feel free to fork and contribute to the project! Submit issues or pull requests for any improvements or bug fixes.

---

## License

This project is licensed under the MIT License. See the LICENSE file for details.

---

This README provides an overview of the JSON Parser Library. For further details, refer to the library's source code and in-line comments. Happy coding! 🎉
