#ifndef JSONPARSE_PARSER_H
#define JSONPARSE_PARSER_H

#include <vector>
#include <string>
#include <memory>
#include "exceptions/SyntaxErrorException.h"
#include "parsed-field.h"
#include "singleton.h"
#include "tokenizer.h"
#include "schema_handler.h"

class Parser : public Singleton<Parser> {
    std::vector<Token_Data> tokens;
    size_t tokenPos = 0;

private:
    bool isValidPos() const {
        return tokenPos < tokens.size();
    }

    Token_Data& currentToken() {
        if (!isValidPos()) {
            throw SyntaxErrorException(Token_Data(), "Unexpected end of input");
        }
        return tokens[tokenPos];
    }

    template<typename T>
    void parse_object(T* instance) {
        while (tokenPos < tokens.size()) {
            if (tokens[tokenPos].token == TOKEN_CLOSE_BRACE) {
                tokenPos++;
                return;
            }
            if (tokens[tokenPos].token == TOKEN_COMMA) {
                tokenPos++;
                continue;
            }
            if (tokens[tokenPos].token == TOKEN_STRING) {
                // Parse field name
                std::string fieldName = *static_cast<std::string*>(tokens[tokenPos].value);
                tokenPos++;
                if (!isValidPos() || tokens[tokenPos].token != TOKEN_COLON) {
                    throw SyntaxErrorException(currentToken(), "Expected ':' after field name");
                }
                tokenPos++;
                if (!isValidPos()) {
                    throw SyntaxErrorException(currentToken(), "Expected value after ':'");
                }
                // Parse field value with knowledge of parent schema and field name
                ParsedField fieldValue = parse_value(instance, fieldName);
                // Set the field in the instance
                instance->set(fieldName, fieldValue);
            } else {
                throw SyntaxErrorException(currentToken(), "Invalid Field Name");
            }
        }
        throw SyntaxErrorException(currentToken(), "Unterminated Object");
    }

    template<typename T>
    void parse_field(T& instance) {
        // Parse field name
        std::string fieldName = *static_cast<std::string*>(tokens[tokenPos].value);
        tokenPos++;
        if (!isValidPos() || tokens[tokenPos].token != TOKEN_COLON) {
            throw SyntaxErrorException(currentToken(), "Expected ':' after field name");
        }
        tokenPos++;
        if (!isValidPos()) {
            throw SyntaxErrorException(currentToken(), "Expected value after ':'");
        }
        // Parse field value
        ParsedField fieldValue = parse_value();
        // Set the field in the instance
        instance.set(fieldName, fieldValue);
    }

    ParsedField parse_value(Schema* parentSchema = nullptr, const std::string& fieldName = "") {
        if (!isValidPos()) {
            throw SyntaxErrorException(currentToken(), "Unexpected end of input");
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
            case TOKEN_OPEN_BRACE: {
                if (parentSchema) {
                    auto nestedSchema = parentSchema->createNestedSchema(fieldName);
                    if (nestedSchema) {
                        tokenPos++;
                        parse_object(nestedSchema.get());
                        fieldValue = ParsedField(nestedSchema);
                        break;
                    }
                }
                // If no specific nested schema, parse as generic Schema
                auto nestedSchema = std::make_shared<Schema>();
                tokenPos++;
                parse_object(nestedSchema.get());
                fieldValue = ParsedField(nestedSchema);
                break;
            }
            case TOKEN_OPEN_BRACKET: {
                fieldValue = ParsedField(parse_array(parentSchema));
                break;
            }
            default:
                throw SyntaxErrorException(currentToken(), "Invalid value");
        }
        return fieldValue;
    }

    std::vector<ParsedField> parse_array() {
        if (!isValidPos() || tokens[tokenPos].token != TOKEN_OPEN_BRACKET) {
            throw SyntaxErrorException(currentToken(), "Invalid Start of Array");
        }
        tokenPos++;
        std::vector<ParsedField> list;
        while (tokenPos < tokens.size()) {
            if (tokens[tokenPos].token == TOKEN_CLOSE_BRACKET) {
                tokenPos++;
                return list;
            }
            if (tokens[tokenPos].token == TOKEN_COMMA) {
                tokenPos++;
                continue;
            }
            ParsedField value = parse_value();
            list.push_back(value);
        }
        throw SyntaxErrorException(currentToken(), "Unterminated Array");
    }

public:
    Parser() : Singleton<Parser>() {}

    template<typename T>
    T parseContents(const std::string& contents) {
        Tokenizer tokenizer;
        this->tokens = tokenizer.tokenizeContents(contents);
        tokenPos = 0;
        auto instance = parse_object<T>();
        if (tokenPos != tokens.size()) {
            throw SyntaxErrorException(tokens[tokenPos], "Invalid End of File");
        }
        return *instance;
    }

    template<typename T>
    T parseFile(const std::string& filepath) {
        Tokenizer tokenizer;
        this->tokens = tokenizer.tokenizeFile(filepath);
        tokenPos = 0;
        auto instance = parse_object<T>();
        if (tokenPos != tokens.size()) {
            throw SyntaxErrorException(tokens[tokenPos], "Invalid End of File");
        }
        return *instance;
    }
};

#endif // JSONPARSE_PARSER_H
