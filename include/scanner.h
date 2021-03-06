// THIS FILE IS PART OF MJava-Compiler PROJECT
// scanner.h - scanner
 
// Created by Li Taiji 2020-03-18
// Copyright (c) 2020 Li Taiji All rights reserved

#ifndef SCANNER_H_
#define SCANNER_H_

#include "dictionary.h"
#include "token.h"
#include <fstream>
#include <string>

namespace MJava
{

    class Scanner
    {
      public:
        explicit        Scanner(const std::string& srcFileName);
        Token           getToken() const;
        Token           getNextToken();
        static bool     getErrorFlag();
        static void     setErrorFlag(bool flag);

      private:
        void            getNextChar();
        char            peekChar();
        void            addToBuffer(char c);
        void            reduceBuffer();

        void            makeToken(TokenType tt, TokenValue tv,
                                  const TokenLocation& loc, const std::string& name, int symbolPrecedence);

        void            makeToken(TokenType tt, TokenValue tv,
                                  const TokenLocation& loc, int intValue, const std::string& name);

        void            makeToken(TokenType tt, TokenValue tv,
                                  const TokenLocation& loc, double realValue, const std::string& name);
        void            makeToken(TokenType tt, TokenValue tv,
                                  const TokenLocation& loc, const std::string& strVale, const std::string& name);

        void            handleEOFState();
        void            handleIdentifierState();
        void            handleNumberState();
        void            handleCharState();
        void            handleStringState();
        void            handleOperationState();
        void            preprocess();
        void            handleLineComment();
        void            handleBlockComment();
        TokenLocation   getTokenLocation() const;

        void            handleDigit();
        void            handleXDigit();
        void            handleODigit();
        void            handleFraction();
        void            handleExponent();
        void            errorReport(const std::string& msg);

      public:
        enum class State
        {
            NONE,
            END_OF_FILE,
            IDENTIFIER,
            NUMBER,
            SINGLE_CHAR,
            STRING,
            OPERATION
        };

      private:
        std::string         fileName_;
        std::ifstream       input_;
        long                line_;
        long                column_;
        TokenLocation       loc_;
        char                currentChar_;
        State               state_;
        Token               token_;
        Dictionary          dictionary_;
        std::string         buffer_;
        static bool         errorFlag_;

    };

    inline Token Scanner::getToken() const
    {
        return token_;
    }

    inline bool Scanner::getErrorFlag()
    {
        return errorFlag_;
    }

    inline TokenLocation Scanner::getTokenLocation() const
    {
        return TokenLocation(fileName_, line_, column_);
    }
} // namespace MJava

#endif // scanner.h