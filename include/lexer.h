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
    INTEGER,
    ERROR
};

enum LexerParseState {
    LEXER_PARSE_OK = 0,
    LEXER_PARSE_EMPTY,
    LEXER_PARSE_INVALID_IDENTIFIER,
    LEXER_PARSE_INVALID_NUMBER,
    LEXER_PARSE_INVAILD_CHARACTERS,
    LEXER_PARSE_INTEGER_TOO_BIG
};

void lineScanner(const char *line, char *tokens, int *toklen);
void fileScanner(FILE *fp, FILE* of);

union Value
{
    int integer;
    bool boolean;
    char *literal;
};

class Token
{
    public:
        void setTokenType(TokenType tokenType);
        void setValue(int integer);
        void setValue(bool boolean);
        void setValue(char *literal);
        TokenType getTokenType() const;
        Value getValue() const;

    private:
        TokenType tokenType;
        Value value;
};

class Lexer
{
    public:
        Lexer();
        Lexer(int length);
        Lexer(const Lexer &lexer);
        ~Lexer();
        void setText(const char *text);
        char *getText() const;
        LexerParseState getToken(Token *token);

    private:
        char *text;
};

extern const int MAX_BUFFER;
extern const int LINE_BUFFER;
extern const char *labels[];
extern const char *literals[];
extern const std::unordered_map<char*, TokenType> tokenTypeMap;

#endif