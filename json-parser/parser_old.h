#ifndef JSONPARSE_PARSER_OLD_H
#define JSONPARSE_PARSER_OLD_H

#include <memory>
#include <vector>
#include <string>
#include <stack>
#include "exceptions/SyntaxErrorException.h"
#include "parsed-field.h"
#include "singleton.h"
#include "tokenizer.h"
#include "schema-factory.h"
#include "schema.h"
#include "instance_handler.h"

class ParserOld : public Singleton<ParserOld> {
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
                auto field = parse_field_value("");
            } else {
                throw SyntaxErrorException(currentToken(), "Invalid Header");
            }
        }
        throw SyntaxErrorException(currentToken(), "Unterminated Object");
    }


//    std::shared_ptr<Schema> parse_object(std::string fieldName) {
//        InstanceHandler::getInstance().pushNewInstance(SchemaFactory::getInstance().createInstance(fieldName));
//        if (!isValidPos() || tokens[tokenPos].token != TOKEN_OPEN_BRACE) {
//            throw SyntaxErrorException(currentToken(), "Invalid Start of Object");
//        }
//        tokenPos++;
//
//        while (tokenPos < tokens.size()) {
//            if (tokens[tokenPos].token == TOKEN_CLOSE_BRACE) {
//                tokenPos++;
//                return InstanceHandler::getInstance().pop();
//            }
//            if (tokens[tokenPos].token == TOKEN_COMMA) {
//                tokenPos++;
//                continue;
//            }
//            if (tokens[tokenPos].token == TOKEN_STRING) {
//                std::string fieldName;
//                if (tokens[tokenPos].value != nullptr) {
//                    std::string fieldName = *static_cast<std::string*>(tokens[tokenPos].value);
//                } else {
//                    throw std::runtime_error("Error: Null pointer at tokens");
//                }
//
//                tokenPos++;
//
//                if (!isValidPos() || tokens[tokenPos].token != TOKEN_COLON) {
//                    throw SyntaxErrorException(currentToken(), "Invalid Field Separator");
//                }
//                tokenPos++;
//                ParsedField value = parse_field_value(fieldName);
//                InstanceHandler::getInstance().getCurrentInstance()->set(fieldName, &value);
//            } else {
//                throw SyntaxErrorException(currentToken(), "Invalid Field Name");
//            }
//        }
//
//        throw SyntaxErrorException(currentToken(), "Unterminated Object");
//    }

    ParsedField parse_field_value(std::string fieldName) {
        ParsedField value;
        switch (tokens[tokenPos].token) {
            case TOKEN_STRING:
                value = ParsedField(*static_cast<std::string*>(tokens[tokenPos].value));
                tokenPos++;
                break;
            case TOKEN_DECIMAL:
                value = ParsedField(*(double *)tokens[tokenPos].value);
                tokenPos++;
                break;
            case TOKEN_INTEGER:
                value = ParsedField(*(int*)tokens[tokenPos].value);
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
            case TOKEN_OPEN_BRACE: {
                value = ParsedField(parse_object(fieldName));
                break;
            }
            case TOKEN_OPEN_BRACKET:
                value = ParsedField(parse_array(fieldName));
                break;
            default:
                throw SyntaxErrorException(currentToken(), "Invalid Field Value");
        }
        return value;
    }

    std::vector<ParsedField> parse_array(std::string fieldName) {
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
            list.push_back(parse_field_value(fieldName));
        }
        throw SyntaxErrorException(currentToken(), "Unterminated Array");
    }

public:
    Parser() : Singleton<Parser>() {}

    ParsedField parse_contents(const std::string& contents, std::string rootSchema) {
        this->tokens = Tokenizer().tokenizeContents(contents);
//        auto rootSchemaCreator = SchemaFactory::getInstance().getType("RootSchema");
        InstanceHandler::getInstance().pushNewInstance(SchemaFactory::getInstance().createInstance(rootSchema));
        ParsedField object = ParsedField(parse_object(rootSchema));
        if (tokenPos != tokens.size()) {
            throw SyntaxErrorException(tokens[tokenPos], "Unexpected content after parsing");
        }
        return object;
    }

    ParsedField parse_file(const std::string& filepath, std::string rootSchema) {
        this->tokens = Tokenizer().tokenizeFile(filepath);
        InstanceHandler::getInstance().pushNewInstance(SchemaFactory::getInstance().createInstance(rootSchema));
        ParsedField object = ParsedField(parse_object(rootSchema));
        if (tokenPos != tokens.size()) {
            throw SyntaxErrorException(tokens[tokenPos], "Unexpected content after parsing");
        }
        return object;
    }
};

#endif // JSONPARSE_PARSER_OLD_H
