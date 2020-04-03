// THIS FILE IS PART OF MJava-Compiler PROJECT
// token.h - token
 
// Created by Li Taiji 2020-03-18
// Copyright (c) 2020 Li Taiji All rights reserved

#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>
#include <iostream>
#include <tuple>
#include <map>
#include <cassert>

namespace MJava
{
    enum class TokenType
    {
        // in fact, we can put these 5 types to one category
        // named constant. but I want to make it cleaner.
        INTEGER,        // such as 3, 4 and so on
        BOOLEAN,        // true or false.
        REAL,           // such as 3.14
        CHAR_LITERAL,   // such as 'a','b'
        STRING_LITERAL, // such as "hello world"

        IDENTIFIER,     // such as abc
        KEYWORD,       // such as if
        TYPE,           // such as int
        OPERATOR,      // such as  + - * /
        DELIMITER,      // such as ,
        END_OF_FILE,    // end of file
        UNKNOWN
    };

    enum class TokenValue
    {
        // keyword
        CLASS = 0,
        PUBLIC,
        STATIC,
        VOID,
        MAIN,
        EXTENDS,
        RETURN,
        IF,
        ELSE,
        WHILE,
        FOR,
        PRINT,
        LENGTH,
        THIS,
        NEW,

        // type
        DOUBLE,
        INT,
        CHAR,
        STRING,
        BOOL,

        // boolean
        TRUE,
        FALSE,
        
        // symbols
        LPAREN,             // (
        RPAREN,             // )
        LBRACK,             // [
        RBRACK,             // ]
        LBRACE,             // {
        RBRACE,             // }
        COMMA,              // ,
        SEMICOLON,          // ;
        ASSIGN,             // =
        AND,                // &&
        LT,                 // <
        ADD,                // +
        SUB,                // -
        MULTI,              // *
        DOT,                // .
        NOT,                // !
        UNRESERVED
    };


    class TokenLocation
    {
      public:
        TokenLocation();
        TokenLocation(const std::string& fileName, int line, int column);

        // this method is very similar with toString method in Java.
        std::string toString() const;
      private:
        std::string fileName_;
        int line_;
        int column_;
    };

    class Token
    {
      public:
        Token();
        Token(TokenType type, TokenValue value, const TokenLocation& location,
              const std::string& name, int symbolPrecedence);
        Token(TokenType type, TokenValue value, const TokenLocation& location,
              const std::string& strValue, const std::string& name);
        Token(TokenType type, TokenValue value, const TokenLocation& location,
              int intValue, const std::string& name);
        Token(TokenType type, TokenValue value, const TokenLocation& location,
              double realValue, const std::string& name);

        // get token information
        TokenType getTokenType() const;
        TokenValue getTokenValue() const;
        const TokenLocation& getTokenLocation() const;
        std::string getTokenName() const;

        // + - * / and so on.
        int getSymbolPrecedence() const;

        // get constant values of token
        int getIntValue() const;
        double getRealValue() const;
        std::string getStringValue() const;

        // output debug information.
        // here output token location, value and type.
        // we can control it using option(such as -debug).
        // but now, we use it directly.
        void dump(std::ostream& out = std::cout) const;

        // more exact function for getting identifier name.
        // Its essential heart is just getTokenName.
        std::string getIdentifierName() const;

        std::string tokenTypeDescription() const;
        std::string toString() const;

      private:
        TokenType       type_;
        TokenValue      value_;
        TokenLocation   location_;
        std::string     name_;
        int             symbolPrecedence_;
        

        // const values of token
        int             intValue_;
        double          realValue_;
        std::string     strValue_;


    };

    inline TokenType Token::getTokenType() const
    {
        return type_;
    }

    inline TokenValue Token::getTokenValue() const
    {
        return value_;
    }

    inline std::string Token::getTokenName() const
    {
        return name_;
    }

    inline const TokenLocation& Token::getTokenLocation() const
    {
        return location_;
    }

    inline int Token::getIntValue() const
    {
        return intValue_;
    }

    inline double Token::getRealValue() const
    {
        return realValue_;
    }

    inline std::string Token::getStringValue() const
    {
        return strValue_;
    }

    inline int Token::getSymbolPrecedence() const
    {
        return symbolPrecedence_;
    }

    inline std::string Token::getIdentifierName() const
    {
        assert(type_ == TokenType::IDENTIFIER && "Token type should be identifier.");
        return name_;
    }
} // namespace MJava

#endif // token.h