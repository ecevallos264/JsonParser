#ifndef JSONPARSE_PARSER_H
#define JSONPARSE_PARSER_H

#include "exceptions/SyntaxErrorException.h"
#include "parsed-field.h"
#include "singleton.h"
#include "tokenizer.h"
#include "instance_handler.h"
#include "schema-factory.h"
#include "tokens.h"

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

    std::shared_ptr<Schema> parse_object(std::string fieldName) {
        InstanceHandler::getInstance().pushNewInstance(SchemaFactory::getInstance().createInstance(fieldName));
        if (!isValidPos() || tokens[tokenPos].token != TOKEN_OPEN_BRACE) {
            throw SyntaxErrorException(currentToken(), "Invalid Start of Object");
        }
        tokenPos++;
        while (tokenPos < tokens.size()) {
            if (tokens[tokenPos].token == TOKEN_CLOSE_BRACE) {
                tokenPos++;
                return InstanceHandler::getInstance().pop();
            }
            if (tokens[tokenPos].token == TOKEN_COMMA) {
                tokenPos++;
                continue;
            }
            if (tokens[tokenPos].token == TOKEN_STRING) {
                parse_field();
            } else {
                throw SyntaxErrorException(currentToken(), "Invalid Header");
            }
        }
        throw SyntaxErrorException(currentToken(), "Unterminated Object");
    }

    std::vector<ParsedField> parse_array(const std::string& fieldName) {
        if (!isValidPos() || tokens[tokenPos].token != TOKEN_OPEN_BRACKET) {
            throw SyntaxErrorException(currentToken(), "Invalid Start of Array");
        }
        tokenPos++;

        std::vector<ParsedField> array; // Use a vector to store array elements
        while (tokenPos < tokens.size()) {
            if (tokens[tokenPos].token == TOKEN_CLOSE_BRACKET) {
                tokenPos++;
                InstanceHandler::getInstance().getCurrentInstance()->set(fieldName, array);
                return array; // Return the parsed array
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
                case TOKEN_DECIMAL:
                    value = ParsedField(*(double *)tokens[tokenPos].value);
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
                case TOKEN_OPEN_BRACE: // Start of a nested object
                    value = ParsedField(parse_object(fieldName));
                    break;
                case TOKEN_OPEN_BRACKET: // Start of a nested array
                    value = ParsedField(parse_array(fieldName));
                    break;
                default:
                    throw SyntaxErrorException(currentToken(), "Invalid Array Element");
            }
            array.push_back(value); // Add the parsed value to the array
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
            case TOKEN_DECIMAL:
            case TOKEN_INTEGER:
            case TOKEN_BOOLEAN:
                InstanceHandler::getInstance().getCurrentInstance()->set(fieldName, tokens[tokenPos].value);
                tokenPos++;
                break;
            case TOKEN_NULL:
                InstanceHandler::getInstance().getCurrentInstance()->set(fieldName, nullptr);
                tokenPos++;
                break;
            case TOKEN_OPEN_BRACE:
//                fieldValue = ParsedField(parse_object(fieldName));
                InstanceHandler::getInstance().getCurrentInstance()->set(fieldName, parse_object(fieldName));
                break;
            case TOKEN_OPEN_BRACKET:
//                fieldValue = ParsedField(parse_array(fieldName));
                InstanceHandler::getInstance().getCurrentInstance()->set(fieldName, parse_array(fieldName));
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

    template<typename T>
    std::shared_ptr<T> parse_contents(string contents, std::string fieldName) {
        this->tokens = (new Tokenizer())->tokenizeContents(contents);
        std::shared_ptr<Schema> object = parse_object(fieldName);
        if(tokenPos != tokens.size()) {
            throw SyntaxErrorException(tokens[tokenPos], "Invalid End of File");
        }
        return std::dynamic_pointer_cast<T>(object);
    }

    template<typename T>
    std::shared_ptr<T> parse_file(string filepath, std::string fieldName) {
        this->tokens = (new Tokenizer())->tokenizeFile(filepath);
        std::shared_ptr<Schema> object = parse_object(fieldName);
        if(tokenPos != tokens.size()) {
            throw SyntaxErrorException(tokens[tokenPos], "Invalid End of File");
        }
        return std::dynamic_pointer_cast<T>(object);
    }

    Tokenizer *tokenizer;
};

#endif // JSONPARSE_PARSER_H