// THIS FILE IS PART OF MJava-Compiler PROJECT
// lexer.cpp - The header of the lexer
 
// Created by Li Taiji 2020-03-18
// Copyright (c) 2020 Li Taiji All rights reserved

#ifndef LEXER_H
#define LEXER_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unordered_map>

namespace lexer
{

extern const int MAX_BUFFER;
extern const int LINE_BUFFER;
struct HashCode;
struct CharCmp;
extern const std::unordered_map<const char *, const char *, HashCode, CharCmp> tokenLabelMap;

// the type of tokens
enum TokenType
{
    CLASS = 0,
    PUBLIC,
    STATIC,
    VOID,
    MAIN,
    STRING,
    EXTENDS,
    RETURN,
    INT,
    BOOLEAN,
    IF,
    ELSE,
    WHILE,
    PRINT,
    LENGTH,
    TRUE,
    FALSE,
    THIS,
    NEW,
    LBRACK,
    RBRACK,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    COMMA,
    SEMICOLON,
    ASSIGN,
    AND,
    LT,
    ADD,
    SUB,
    MULTI,
    DOT,
    NOT,
    IDENTIFIER,
    INTEGER
};

void lineScanner(const char *line, char *tokens, int *toklen);
void fileScanner(FILE *fp, FILE* of);

class Lexer
{
    public:
        Lexer();
        Lexer(const Lexer &lexer);
        ~Lexer();
        void parse(const char *line);
        char *getTokens() const;
        int getToklen() const;

    private:
        char *tokens;
        int toklen;
};

}

#endif