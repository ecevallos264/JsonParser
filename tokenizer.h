#ifndef JSONPARSE_TOKENIZER_H
#define JSONPARSE_TOKENIZER_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "exceptions/SyntaxErrorException.h"
#include "tokens.h"

class Tokenizer {
private:
    int lineNum = 1;
    int pos = 0;
    std::string line;
    std::vector<Token_Data> tokens;

    void addToken(Token token, void* value = nullptr) {
        tokens.push_back({lineNum, pos, token, value});
    }

    void parseString() {
        std::string currentString;
        while (pos < line.size() && line[pos] != '"') {
            currentString += line[pos++];
        }

        if (pos >= line.size() || line[pos] != '"') {
            throw SyntaxErrorException(lineNum, pos, "Invalid field value");
            return;
        }

//        pos++;
        addToken(TOKEN_STRING, new std::string(currentString));
    }

    void parseBoolean() {
        std::string value = (line[pos] == 't') ? "true" : "false";
        std::string expected = value;

        for (char c : expected) {
            if (pos >= line.size() || line[pos++] != c) {
                throw SyntaxErrorException(lineNum, pos, "Invalid field value");
                return;
            }
        }

        addToken(TOKEN_BOOLEAN, new bool(value == "true"));
    }

    void parseNull() {
        std::string nullStr = "null";
        for (char c : nullStr) {
            if (pos >= line.size() || line[pos++] != c) {
                throw SyntaxErrorException(lineNum, pos, "Invalid field value");
                return;
            }
        }

        addToken(TOKEN_NULL, nullptr);
    }

    void parseNumber() {
        unsigned int integerPart = 0;
        float decimalPart = 0.0f;
        bool isNegative = false;
        bool hasPeriod = false;
        float decimalMultiplier = 0.1;

        if (line[pos] == '-') {
            isNegative = true;
            pos++;
        }

        if (line[pos] == '.' || !isdigit(line[pos])) {
            throw SyntaxErrorException(lineNum, pos, "Invalid field value");
            return;
        }

        while (pos < line.size() && (isdigit(line[pos]) || line[pos] == '.')) {
            if (line[pos] == '.') {
                if (hasPeriod) {
                    throw SyntaxErrorException(lineNum, pos, "Invalid field value");
                    return;
                }
                hasPeriod = true;
            } else if (isdigit(line[pos])) {
                if (hasPeriod) {
                    decimalPart += (line[pos] - '0') * decimalMultiplier;
                    decimalMultiplier *= 0.1f;
                } else {
                    integerPart = (integerPart * 10) + (line[pos] - '0');
                }
            }
            pos++;
        }

        float finalNum = (isNegative ? -1.0f : 1.0f) * (integerPart + decimalPart);

        if (pos < line.size() && line[pos] != ',' && line[pos] != '}' && line[pos] != ']' && !isspace(line[pos])) {
            addToken(TOKEN_FLOAT, nullptr);
            return;
        }

        if (!hasPeriod) {
            addToken(TOKEN_INTEGER, new int (finalNum));
        } else {
            addToken(TOKEN_FLOAT, new float (finalNum));
        }
    }


    void skipWhitespace() {
        while (pos < line.size() && isspace(line[pos])) {
            pos++;
        }
    }

public:
    explicit Tokenizer() {}

    ~Tokenizer() {
        for (auto &token : tokens) {
            if (token.value) {
                switch (token.token) {
                    case TOKEN_STRING:
                        delete static_cast<std::string *>(token.value);
                        break;
                    case TOKEN_BOOLEAN:
                        delete static_cast<bool *>(token.value);
                        break;
                    case TOKEN_FLOAT:
                        delete static_cast<float *>(token.value);
                        break;
                    case TOKEN_INTEGER:
                        delete static_cast<int *>(token.value);
                        break;
                    default:
                        break;
                }
            }
        }
    }

    void parseCharacter() {
        char c = line[pos];
        switch (c) {
            case '[': addToken(TOKEN_OPEN_BRACKET); pos++; break;
            case ']': addToken(TOKEN_CLOSE_BRACKET); pos++; break;
            case '{': addToken(TOKEN_OPEN_BRACE); pos++; break;
            case '}': addToken(TOKEN_CLOSE_BRACE); pos++; break;
            case ',': addToken(TOKEN_COMMA); pos++; break;
            case ':': addToken(TOKEN_COLON); pos++; break;
            case '"':
                pos++;
                parseString();
                pos++;
                break;
            case 't':
            case 'f':
                parseBoolean();
                break;
            case 'n':
                parseNull();
                break;
            default:
                parseNumber();
                break;
        }
    }

    std::vector<Token_Data> getTokens() const {
        return tokens;
    }

    std::vector<Token_Data> tokenizeFile(std::string filepath) {
        std::fstream file(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("File stream is not open.");
        }

        this->lineNum = 1;
        while (getline(file, line)) {
            pos = 0;
            while (pos < line.size()) {
                skipWhitespace();
                if (pos >= line.size()) break;
                parseCharacter();
            }
            lineNum++;
        }
        return this->tokens;
    }

    std::vector<Token_Data> tokenizeContents(std::string contents) {
        pos = 0;
        this->line = contents;
        while (pos < contents.size()) {
            skipWhitespace();
            if (pos >= line.size()) break;
            parseCharacter();
        }
        return this->tokens;
    }

    void printTokens() {
        int index = 0;
        for (auto token : tokens) {
            std::cout << "Token[" << index++ << "] ";
            switch (token.token) {
                case TOKEN_STRING:
                    std::cout << "TOKEN STRING["<< *static_cast<std::string*>(token.value) <<"]" << std::endl;
                    break;
                case TOKEN_BOOLEAN:
                    std::cout << "TOKEN BOOLEAN["<< *static_cast<bool *>(token.value) <<"]" << std::endl;
                    break;
                case TOKEN_FLOAT:
                    std::cout << "TOKEN NUMBER["<< *static_cast<float *>(token.value) <<"]" << std::endl;
                    break;
                case TOKEN_INTEGER:
                    std::cout << "TOKEN NUMBER["<< *static_cast<float *>(token.value) <<"]" << std::endl;
                    break;
                case TOKEN_ARRAY:
                    std::cout << "TOKEN ARRAY" << std::endl;
                    break;
                case TOKEN_NULL:
                    std::cout << "TOKEN NULL" << std::endl;
                    break;
                case TOKEN_COLON:
                    std::cout << "TOKEN COLON" << std::endl;
                    break;
                case TOKEN_COMMA:
                    std::cout << "TOKEN COMMA" << std::endl;
                    break;
                case TOKEN_OPEN_BRACKET:
                    std::cout << "TOKEN OPEN BRACKET" << std::endl;
                    break;
                case TOKEN_CLOSE_BRACKET:
                    std::cout << "TOKEN CLOSE BRACKET" << std::endl;
                    break;
                case TOKEN_OPEN_BRACE:
                    std::cout << "TOKEN OPEN BRACE" << std::endl;
                    break;
                case TOKEN_CLOSE_BRACE:
                    std::cout << "TOKEN CLOSE BRACE" << std::endl;
                    break;
                case TOKEN_NONE:
                    std::cout << "TOKEN NONE" << std::endl;
                    break;

                default:
                    std::cerr << "Unknown token[" << token.token << "]" << std::endl;
                    break;
            }
        }
    }
};

#endif // JSONPARSE_TOKENIZER_H
