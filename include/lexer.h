// THIS FILE IS PART OF MJava-Compiler PROJECT
// lexer.cpp - The header of the lexer
 
// Created by Li Taiji 2020-03-18
// Copyright (c) 2020 Li Taiji All rights reserved

#ifndef LEXER_H
#define LEXER_H

#include <cstdio>
#include <cstdlib>
#include <cstring>

#define BUFFER 1024

extern const char *labels[];
extern const char *literals[];

void lineScanner(const char *line, char *tokens, int *toklen);
void fileScanner(FILE *fp);

class Lexer
{
    public:
    Lexer()
    {
        tokens = new char[BUFFER];
        toklen = 0;
    }
    Lexer(const Lexer &lexer)
    {
        tokens = new char[BUFFER];
        strncpy(tokens, lexer.getTokens(), BUFFER);
        toklen = lexer.getToklen();
    }
    ~Lexer()
    {
        if (tokens != NULL)
            delete[] tokens;
    }
    void parse(const char *line)
    {
        lineScanner(line, this->tokens, &this->toklen);
    }

    char* getTokens() const
    {
        return this->tokens;
    }

    int getToklen() const
    {
        return this->toklen;
    }
    
    private:
        char *tokens;
        int toklen;
};

#endif