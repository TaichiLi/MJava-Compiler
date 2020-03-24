// THIS FILE IS PART OF MJava-Compiler PROJECT
// dicitonary.cpp - dicitonary
 
// Created by Li Taiji 2020-03-18
// Copyright (c) 2020 Li Taiji All rights reserved

#include "dictionary.h"

namespace MJava
{
    Dictionary::Dictionary()
    {
        addToken("=",                   std::make_tuple(TokenValue::ASSIGN,            TokenType::OPERATOR, 0));
        addToken("<",                   std::make_tuple(TokenValue::LT,                TokenType::OPERATOR, 2));
        addToken("+",                   std::make_tuple(TokenValue::ADD,               TokenType::OPERATOR, 10));
        addToken("-",                   std::make_tuple(TokenValue::SUB,               TokenType::OPERATOR, 10));
        addToken("*",                   std::make_tuple(TokenValue::MULTI,             TokenType::OPERATOR, 20));
        addToken("&&",                  std::make_tuple(TokenValue::AND,               TokenType::OPERATOR, 20));
        addToken("!",                   std::make_tuple(TokenValue::NOT,               TokenType::OPERATOR,  40));
        addToken("(",                   std::make_tuple(TokenValue::LPAREN,            TokenType::DELIMITER, -1));
        addToken(")",                   std::make_tuple(TokenValue::RPAREN,            TokenType::DELIMITER, -1));
        addToken("[",                   std::make_tuple(TokenValue::LBRACK,            TokenType::DELIMITER, -1));
        addToken("]",                   std::make_tuple(TokenValue::RBRACK,            TokenType::DELIMITER, -1));
        addToken("{",                   std::make_tuple(TokenValue::LBRACE,            TokenType::DELIMITER, -1));
        addToken("}",                   std::make_tuple(TokenValue::RBRACE,            TokenType::DELIMITER, -1));
        addToken(",",                   std::make_tuple(TokenValue::COMMA,             TokenType::DELIMITER, -1));
        addToken(";",                   std::make_tuple(TokenValue::SEMICOLON,         TokenType::DELIMITER, -1));
        addToken(".",                   std::make_tuple(TokenValue::DOT,               TokenType::DELIMITER, -1));
        addToken("class",               std::make_tuple(TokenValue::CLASS,             TokenType::KEYWORD,  -1));
        addToken("public",              std::make_tuple(TokenValue::PUBLIC,            TokenType::KEYWORD,  -1));
        addToken("static",              std::make_tuple(TokenValue::STATIC,            TokenType::KEYWORD,  -1));
        addToken("void",                std::make_tuple(TokenValue::VOID,              TokenType::KEYWORD,  -1));
        addToken("main",                std::make_tuple(TokenValue::MAIN,              TokenType::KEYWORD,  -1));
        addToken("extends",             std::make_tuple(TokenValue::EXTENDS,           TokenType::KEYWORD,  -1));
        addToken("return",              std::make_tuple(TokenValue::RETURN,            TokenType::KEYWORD,  -1));
        addToken("if",                  std::make_tuple(TokenValue::IF,                TokenType::KEYWORD,  -1));
        addToken("else",                std::make_tuple(TokenValue::ELSE,              TokenType::KEYWORD,  -1));
        addToken("while",               std::make_tuple(TokenValue::WHILE,             TokenType::KEYWORD,  -1));
        addToken("System.out.println",  std::make_tuple(TokenValue::PRINT,             TokenType::KEYWORD,  -1));
        addToken("length",              std::make_tuple(TokenValue::LENGTH,            TokenType::KEYWORD,  -1));
        addToken("this",                std::make_tuple(TokenValue::THIS,              TokenType::KEYWORD,  -1));
        addToken("new",                 std::make_tuple(TokenValue::NEW,               TokenType::KEYWORD,  -1));
        addToken("true",                std::make_tuple(TokenValue::TRUE,              TokenType::BOOLEAN,  -1));
        addToken("false",               std::make_tuple(TokenValue::FALSE,             TokenType::BOOLEAN,  -1));
        addToken("int",                 std::make_tuple(TokenValue::INT,               TokenType::TYPE,  -1));
        addToken("boolean",             std::make_tuple(TokenValue::BOOL,              TokenType::TYPE,  -1));
        addToken("char",                std::make_tuple(TokenValue::CHAR,              TokenType::TYPE,  -1));
        addToken("String",              std::make_tuple(TokenValue::STRING,            TokenType::TYPE,  -1));
    }

    void Dictionary::addToken(std::string name,
                              std::tuple<TokenValue, TokenType, int> tokenMeta)
    {
        dictionary_.insert(std::pair<decltype(name), decltype(tokenMeta)>(name, tokenMeta));
    }

    // if we can find it in the dictionary, we change the token type
    std::tuple<TokenType, TokenValue, int> Dictionary::lookup(const std::string& name) const
    {
        TokenValue tokenValue = TokenValue::UNRESERVED;
        TokenType  tokenType  = TokenType::IDENTIFIER;
        int        precedence = -1;
        auto iter = dictionary_.find(name);

        if (iter != dictionary_.end())
        {
            tokenValue = std::get<0>(iter->second);
            tokenType  = std::get<1>(iter->second);
            precedence = std::get<2>(iter->second);
        }

        return std::make_tuple(tokenType, tokenValue, precedence);
    }

    bool Dictionary::haveToken(const std::string& name) const
    {
        auto iter = dictionary_.find(name);

        if (iter != dictionary_.end())
        {
            return true;
        }

        return false;
    }
}