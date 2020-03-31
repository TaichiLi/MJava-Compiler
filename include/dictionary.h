// THIS FILE IS PART OF MJava-Compiler PROJECT
// dicitonary.h - dicitonary of keywords, operators and symbols
 
// Created by Li Taiji 2020-03-18
// Copyright (c) 2020 Li Taiji All rights reserved

#ifndef DICTIONARY_H_
#define DICTIONARY_H_

#include <string>
#include <tuple>
#include <map>
#include "token.h"

namespace MJava
{
    class Dictionary
    {
      public:
        Dictionary();
        std::tuple<TokenType, TokenValue, int> lookup(const std::string& name) const;
        bool haveToken(const std::string& name) const;
      private:
        void addToken(std::string name, std::tuple<TokenValue, TokenType, int> tokenMeta);

      private:
        // four token property: token name, token value, token type, precedence.
        std::map<std::string, std::tuple<TokenValue, TokenType, int>> dictionary_;
    };
}

#endif // dictionary.h