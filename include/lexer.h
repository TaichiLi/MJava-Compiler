// THIS FILE IS PART OF MJava-Compiler PROJECT
// lexer.cpp - The header of the lexer
 
// Created by Li Taiji 2020-03-18
// Copyright (c) 2020 Li Taiji All rights reserved

#ifndef LEXER_H
#define LEXER_H

#include <cstdio>
#include <cstdlib>
#include <cstring>

extern const int MAX_BUFFER;
extern const int LINE_BUFFER;
extern const char *labels[];
extern const char *literals[];

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

#endif