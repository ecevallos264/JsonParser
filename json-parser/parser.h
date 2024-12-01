#ifndef JSONPARSE_PARSER_H
#define JSONPARSE_PARSER_H

#include "exceptions/SyntaxErrorException.h"
#include "parsed-field.h"
#include "singleton.h"
#include "tokenizer.h"

class Parser : public Singleton<Parser> {
    std::vector<Token_Data> tokens;
    int tokenPos = 0;

private:
    bool isValidPos() {
        return tokenPos < tokens.size();
    }

    Token_Data& currentToken() {
        if (!isValidPos()) {
            throw SyntaxErrorException(Token_Data(), "Unexpected end of input");
        }
        return tokens[tokenPos];
    }

    map parse_object() {
        if (!isValidPos() || tokens[tokenPos].token != TOKEN_OPEN_BRACE) {
            throw SyntaxErrorException(currentToken(), "Invalid Start of Object");
        }
        tokenPos++;
        map fieldMap;
        while (tokenPos < tokens.size()) {
            if (tokens[tokenPos].token == TOKEN_CLOSE_BRACE) {
                tokenPos++;
                return fieldMap;
            }
            if (tokens[tokenPos].token == TOKEN_COMMA) {
                tokenPos++;
                continue;
            }
            if (tokens[tokenPos].token == TOKEN_STRING) {
                auto field = parse_field();
                fieldMap.insert(field);
            } else {
                throw SyntaxErrorException(currentToken(), "Invalid Header");
            }
        }
        throw SyntaxErrorException(currentToken(), "Unterminated Object");
    }

    list parse_array() {
        if (!isValidPos() || tokens[tokenPos].token != TOKEN_OPEN_BRACKET) {
            throw SyntaxErrorException(currentToken(), "Invalid Start of Array");
        }
        tokenPos++;
        list list;
        while (tokenPos < tokens.size()) {
            if (tokens[tokenPos].token == TOKEN_CLOSE_BRACKET) {
                tokenPos++;
                return list;
            }
            if (tokens[tokenPos].token == TOKEN_COMMA) {
                tokenPos++;
                continue;
            }
            ParsedField value;
            switch (tokens[tokenPos].token) {
                case TOKEN_STRING:
                    value = ParsedField(*static_cast<std::string*>(tokens[tokenPos].value));
                    tokenPos++;
                    break;
                case TOKEN_INTEGER:
                    value = ParsedField(*(int*)tokens[tokenPos].value);
                    tokenPos++;
                    break;
                case TOKEN_FLOAT:
                    value = ParsedField(*(float*)tokens[tokenPos].value);
                    tokenPos++;
                    break;
                case TOKEN_BOOLEAN:
                    value = ParsedField(*(bool*)tokens[tokenPos].value);
                    tokenPos++;
                    break;
                case TOKEN_NULL:
                    value = ParsedField();
                    tokenPos++;
                    break;
                case TOKEN_OPEN_BRACE:
                    value = ParsedField(parse_object());
                    break;
                case TOKEN_OPEN_BRACKET:
                    value = ParsedField(parse_array());
                    break;
                default:
                    throw SyntaxErrorException(currentToken(), "Invalid Array Element");
            }
            list.push_back(value);
        }
        throw SyntaxErrorException(currentToken(), "Unterminated Array");
    }

    std::pair<std::string, ParsedField> parse_field() {
        if (!isValidPos() || tokens[tokenPos].token != TOKEN_STRING) {
            throw SyntaxErrorException(currentToken(), "Invalid Field Initializer");
        }
        std::string fieldName = *static_cast<std::string*>(tokens[tokenPos].value);
        tokenPos++;
        if (!isValidPos() || tokens[tokenPos].token != TOKEN_COLON) {
            throw SyntaxErrorException(currentToken(), "Invalid Field");
        }
        tokenPos++;
        if (!isValidPos()) {
            throw SyntaxErrorException(currentToken(), "Invalid Field Value");
        }
        ParsedField fieldValue;
        switch (tokens[tokenPos].token) {
            case TOKEN_STRING:
                fieldValue = ParsedField(*static_cast<std::string*>(tokens[tokenPos].value));
                tokenPos++;
                break;
            case TOKEN_FLOAT:
                fieldValue = ParsedField(*(float*)tokens[tokenPos].value);
                tokenPos++;
                break;
            case TOKEN_INTEGER:
                fieldValue = ParsedField(*(int*)tokens[tokenPos].value);
                tokenPos++;
                break;
            case TOKEN_BOOLEAN:
                fieldValue = ParsedField(*(bool*)tokens[tokenPos].value);
                tokenPos++;
                break;
            case TOKEN_NULL:
                fieldValue = ParsedField();
                tokenPos++;
                break;
            case TOKEN_OPEN_BRACE:
                fieldValue = ParsedField(parse_object());
                break;
            case TOKEN_OPEN_BRACKET:
                fieldValue = ParsedField(parse_array());
                break;
            default:
                throw SyntaxErrorException(currentToken(), "Invalid Field Value");
        }
        if (isValidPos()) {
            if (tokens[tokenPos].token == TOKEN_COMMA) {
                tokenPos++;
            } else if (tokens[tokenPos].token != TOKEN_CLOSE_BRACE) {
                throw SyntaxErrorException(currentToken(), "Invalid Field Separator");
            }
        }
        return std::make_pair(fieldName, fieldValue);
    }

public:
    Parser() : Singleton<Parser>() {}

    ParsedField parse_contents(string contents) {
        this->tokens = (new Tokenizer())->tokenizeContents(contents);
        ParsedField object = ParsedField(parse_object());
        if(tokenPos != tokens.size()) {
            throw SyntaxErrorException(tokens[tokenPos], "Invalid End of File");
        }
        return object;
    }

    ParsedField parse_file(string filepath) {
        this->tokens = (new Tokenizer())->tokenizeFile(filepath);
        ParsedField object = ParsedField(parse_object());
        if(tokenPos != tokens.size()) {
            throw SyntaxErrorException(tokens[tokenPos], "Invalid End of File");
        }
        return object;
    }

    Tokenizer *tokenizer;
};

#endif // JSONPARSE_PARSER_H