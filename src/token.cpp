// THIS FILE IS PART OF MJava-Compiler PROJECT
// token.cpp - token
 
// Created by Li Taiji 2020-03-18
// Copyright (c) 2020 Li Taiji All rights reserved

#include "token.h"


namespace MJava
{

    TokenLocation::TokenLocation(const std::string& fileName, int line, int column)
        : fileName_(fileName), line_(line), column_(column)
    {}

    TokenLocation::TokenLocation() : fileName_(""), line_(1), column_(0)
    {}


    std::string TokenLocation::toString() const
    {
        return fileName_ + ":" + std::to_string(line_) + ":" + std::to_string(column_) + ":";
    }

    // End TokenLocation


    Token::Token() : type_(TokenType::UNKNOWN), value_(TokenValue::UNRESERVED),
        location_(std::string(""), 0, 0), name_(""), symbolPrecedence_(-1)
    {}

    Token::Token(TokenType type, TokenValue value, const TokenLocation& location,
                 std::string name, int symbolPrecedence)
        : type_(type), value_(value), location_(location), name_(name),
          symbolPrecedence_(symbolPrecedence)
    {}

    Token::Token(TokenType type, TokenValue value, const TokenLocation& location,
                 const std::string& strValue, std::string name)
        : type_(type), value_(value), location_(location),
          name_(name), symbolPrecedence_(-1), strValue_(strValue)
    {}

    Token::Token(TokenType type, TokenValue value, const TokenLocation& location,
                 long intValue, std::string name)
        : type_(type), value_(value), location_(location),
          name_(name), symbolPrecedence_(-1), intValue_(intValue)
    {}

    Token::Token(TokenType type, TokenValue value, const TokenLocation& location,
                 double realValue, std::string name)
        : type_(type), value_(value), location_(location),
          name_(name), symbolPrecedence_(-1), realValue_(realValue)
    {}

    std::string Token::tokenTypeDescription() const
    {
        std::string buffer;

        switch (type_)
        {
            case MJava::TokenType::INTEGER:
                buffer = "integer";
                break;

            case MJava::TokenType::BOOLEAN:
                buffer = "boolean";
                break;
            
            case MJava::TokenType::REAL:
                buffer = "real";
                break;

            case MJava::TokenType::CHAR_LITERAL:
                buffer = "char literal";
                break;

            case MJava::TokenType::STRING_LITERAL:
                buffer = "string literal";
                break;

            case MJava::TokenType::IDENTIFIER:
                buffer = "identifier";
                break;

            case MJava::TokenType::KEYWORD:
                buffer = "keyword";
                break;

            case MJava::TokenType::TYPE:
                buffer = "data type";
                break;

            case MJava::TokenType::OPERATOR:
                buffer = "operator";
                break;

            case MJava::TokenType::DELIMITER:
                buffer = "delimiter";
                break;

            case MJava::TokenType::END_OF_FILE:
                buffer = "eof";
                break;

            case MJava::TokenType::UNKNOWN:
                buffer = "unknown";
                break;

            default:
                break;
        }

        return buffer;
    }

    std::string Token::toString() const
    {
        return std::string("Token Type:\t " + tokenTypeDescription() + "\t" + "Token Name:\t" + name_);
    }

    void Token::dump(std::ostream& out /* = std::cout */) const
    {
        out << location_.toString() << "\t" << tokenTypeDescription()
            << "\t" << name_ << "\t\t" << getSymbolPrecedence() << std::endl;
    }

    // End Token
}