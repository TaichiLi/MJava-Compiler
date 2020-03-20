// THIS FILE IS PART OF MJava-Compiler PROJECT
// lexer.cpp - The core part of the lexer
 
// Created by Li Taiji 2020-03-18
// Copyright (c) 2020 Li Taiji All rights reserved

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include "lexer.h"

// the max length of line
#define BUFFER 1024

// the id of tokens
enum Token
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
    FLASE,
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

// the labels of tokens
const char* labels[] = 
{
    "CLASS",
    "PUBLIC",
    "STATIC",
    "VOID",
    "MAIN",
    "STRING",
    "EXTENDS",
    "RETURN",
    "INT",
    "BOOLEAN",
    "IF",
    "ELSE",
    "WHILE",
    "PRINT",
    "LENGTH",
    "TRUE",
    "FALSE",
    "THIS",
    "NEW",
    "LBRACK",
    "RBRACK",
    "LPAREN",
    "RPAREN",
    "LBRACE",
    "RBRACE",
    "COMMA",
    "SEMICOLON",
    "ASSIGN",
    "AND",
    "LT",
    "ADD",
    "SUB",
    "MULTI",
    "DOT",
    "NOT",
    "IDENTIFIER",
    "INTEGER"
};

// the literals of tokens
const char* literals[] = 
{
    "class",
    "public",
    "static",
    "void",
    "main",
    "String",
    "extends",
    "return",
    "int",
    "boolean",
    "if",
    "else",
    "while",
    "System.out.println",
    "length",
    "true",
    "false",
    "this",
    "new",
    "[",
    "]",
    "(",
    ")",
    "{",
    "}",
    ",",
    ";",
    "=",
    "&&",
    "<",
    "+",
    "-",
    "*",
    ".",
    "!"
};

void lineScanner(const char *line, char *tokens, int *toklen)
{
    tokens[0] = '\0';
    *toklen = 0;
    char *tmp = new char[256];
    int length = strlen(line);
    int startIndex = 0;
    int endIndex = startIndex;
    while (endIndex < length)
    {
        while (isspace(line[endIndex]))
        {
            ++endIndex;
            ++startIndex;
        }
        if (endIndex == length)
            break;
        if (line[endIndex] == '_')
        {
            while (isalpha(line[endIndex]) || isdigit(line[endIndex]) || line[endIndex] == '_')
                ++endIndex;
            char *token = new char[endIndex - startIndex + 1];
            strncpy(token, line + startIndex, endIndex - startIndex);
            token[endIndex - startIndex] = '\0';
            startIndex = endIndex;
            int len = sprintf(tmp, "ERROR: Identifiers can not begin with an underscore %s\n", token);
            *toklen += len;
            delete[] token;
            strncat(tokens, tmp, len);
            continue;
        }
        if (isalpha(line[endIndex]))
        {
            while (isalpha(line[endIndex]) || isdigit(line[endIndex]) || line[endIndex] == '_')
                ++endIndex;
            char *token = new char[endIndex - startIndex + 1];
            strncpy(token, line + startIndex, endIndex - startIndex);
            token[endIndex - startIndex] = '\0';
            for (int i = (enum Token)CLASS; i <= (enum Token)NEW; i++)
            {
                if (i == (enum Token)PRINT)
                {
                    if (strncmp(literals[i], line + startIndex, endIndex - startIndex + 12) == 0)
                    {
                        endIndex += 12;
                        token = new char[endIndex - startIndex + 1];
                        strncpy(token, line + startIndex, endIndex - startIndex);
                        token[endIndex - startIndex] = '\0';
                        startIndex = endIndex;
                        int len = sprintf(tmp, "%s %s\n", labels[i], token);
                        *toklen += len;
                        delete[] token;
                        strncat(tokens, tmp, len);
                        break;
                    }
                }
                if (strcmp(literals[i], token) == 0)
                {
                    int len = sprintf(tmp, "%s %s\n", labels[i], token);
                    *toklen += len;
                    startIndex = endIndex;
                    delete[] token;
                    strncat(tokens, tmp, len);
                    break;
                }
            }
            if (startIndex != endIndex)
            {
                int len = sprintf(tmp, "%s %s\n", labels[(enum Token)IDENTIFIER], token);
                *toklen += len;
                strncat(tokens, tmp, len);
                startIndex = endIndex;
            }
            continue;
        }
        if (isdigit(line[endIndex]))
        {
            while (isdigit(line[endIndex]))
                ++endIndex;
            if (!isalpha(line[endIndex]) && line[endIndex] != '_' && line[endIndex] != '.')
            {
                char* token = new char[endIndex - startIndex + 1];
                strncpy(token, line + startIndex, endIndex - startIndex);
                token[endIndex - startIndex] = '\0';
                startIndex = endIndex;
                int len = sprintf(tmp, "%s %s\n", labels[(enum Token)INTEGER], token);
                *toklen += len;
                delete[] token;
                strncat(tokens, tmp, len);
                continue;
            }
            else if (isalpha(line[endIndex]) || line[endIndex] == '_')
            {
                while (isalpha(line[endIndex]) || isdigit(line[endIndex]) || line[endIndex] == '_')
                    ++endIndex;
                char* token = new char[endIndex - startIndex + 1];
                strncpy(token, line + startIndex, endIndex - startIndex);
                token[endIndex - startIndex] = '\0';
                startIndex = endIndex;
                int len = sprintf(tmp, "ERROR: Identifiers can not begin with a number %s\n", token);
                *toklen += len;
                delete[] token;
                strncat(tokens, tmp, len);
                continue;
            }
            else if (line[endIndex] == '.')
            {
                ++endIndex;
                while (isdigit(line[endIndex]))
                    ++endIndex;
                char* token = new char[endIndex - startIndex + 1];
                strncpy(token, line + startIndex, endIndex - startIndex);
                token[endIndex - startIndex] = '\0';
                startIndex = endIndex;
                int len = sprintf(tmp, "ERROR: Floating Numbers are not supported %s\n", token);
                *toklen += len;
                delete[] token;
                strncat(tokens, tmp, len);
                continue;
            }
            
        }
        int matched = 0;
        for (int i = (enum Token)LBRACK; i <= (enum Token)NOT; i++)
        {
            if (line[endIndex] == literals[i][0])
            {
                if (i != (enum Token)AND)
                {
                    matched = 1;
                    ++endIndex;
                    startIndex = endIndex;
                    int len = sprintf(tmp, "%s %s\n", labels[i], literals[i]);
                    *toklen += len;
                    strncat(tokens, tmp, len);
                    break;
                }
                else
                {
                    if (line[endIndex + 1] == '&')
                    {
                        matched = 1;
                        endIndex += 2;
                        startIndex = endIndex;
                        int len = sprintf(tmp, "%s %s\n", labels[(enum Token)AND], literals[i]);
                        *toklen += len;
                        strncat(tokens, tmp, len);
                        break;
                    }
                }
            }
        }
        if (matched == 0)
        {
            int len = sprintf(tmp, "ERROR: Unknown character %c\n", line[endIndex]);
            *toklen += len;
            strncat(tokens, tmp, len);
            ++endIndex;
            startIndex = endIndex;
        }
    }
    delete[] tmp;
}

// scanner 
// Lexical analysis of the file line by line
void fileScanner(FILE* fp)
{
    FILE *of = NULL;
    of = fopen("./tokenOut.txt", "w+");
    char line[BUFFER];
    int lineCount = 0;
    while(fgets(line, BUFFER, fp) != NULL)
    {
        ++lineCount;
        int length = strlen(line);
        int startIndex = 0;
        int endIndex = startIndex;
        while (endIndex < length)
        {
            while (isspace(line[endIndex]))
            {
                ++endIndex;
                ++startIndex;
            }
            if (endIndex == length)
                break;
            if (line[endIndex] == '_')
            {
                while (isalpha(line[endIndex]) || isdigit(line[endIndex]) || line[endIndex] == '_')
                    ++endIndex;
                char *token = new char[endIndex - startIndex + 1];
                strncpy(token, line + startIndex, endIndex - startIndex);
                token[endIndex - startIndex] = '\0';
                startIndex = endIndex;
                fprintf(of, "#%d ERROR: Identifiers can not begin with an underscore %s\n", lineCount, token);
                delete[] token;
                continue;
            }
            if (isalpha(line[endIndex]))
            {
                while (isalpha(line[endIndex]) || isdigit(line[endIndex]) || line[endIndex] == '_')
                    ++endIndex;
                char *token = new char[endIndex - startIndex + 1];
                strncpy(token, line + startIndex, endIndex - startIndex);
                token[endIndex - startIndex] = '\0';
                for (int i = (enum Token)CLASS; i <= (enum Token)NEW; i++)
                {
                    if (i == (enum Token)PRINT)
                    {
                        if (strncmp(literals[i], line + startIndex, endIndex - startIndex + 12) == 0)
                        {
                            endIndex += 12;
                            token = new char[endIndex - startIndex + 1];
                            strncpy(token, line + startIndex, endIndex - startIndex);
                            token[endIndex - startIndex] = '\0';
                            startIndex = endIndex;
                            fprintf(of, "#%d %s %s\n", lineCount, labels[i], token);
                            delete[] token;
                            break;
                        }
                    }
                    if (strcmp(literals[i], token) == 0)
                    {
                        fprintf(of, "#%d %s %s\n", lineCount, labels[i], token);
                        delete[] token;
                        startIndex = endIndex;
                        break;
                    }
                }
                if (startIndex != endIndex)
                {
                    fprintf(of, "#%d %s %s\n", lineCount, labels[(enum Token)IDENTIFIER], token);
                    delete[] token;
                    startIndex = endIndex;
                }
                continue;
            }
            if (isdigit(line[endIndex]))
            {
                while (isdigit(line[endIndex]))
                    ++endIndex;
                if (!isalpha(line[endIndex]) && line[endIndex] != '_' && line[endIndex] != '.')
                {
                    char* token = new char[endIndex - startIndex + 1];
                    strncpy(token, line + startIndex, endIndex - startIndex);
                    token[endIndex - startIndex] = '\0';
                    startIndex = endIndex;
                    fprintf(of, "#%d %s %s\n", lineCount, labels[(enum Token)INTEGER], token);
                    delete[] token;
                    continue;
                }
                else if (isalpha(line[endIndex]) || line[endIndex] == '_')
                {
                    while (isalpha(line[endIndex]) || isdigit(line[endIndex]) || line[endIndex] == '_')
                        ++endIndex;
                    char* token = new char[endIndex - startIndex + 1];
                    strncpy(token, line + startIndex, endIndex - startIndex);
                    token[endIndex - startIndex] = '\0';
                    startIndex = endIndex;
                    fprintf(of, "#%d ERROR: Identifiers can not begin with a number %s\n", lineCount, token);
                    delete[] token;
                    continue;
                }
                else if (line[endIndex] == '.')
                {
                    ++endIndex;
                    while (isdigit(line[endIndex]))
                        ++endIndex;
                    char* token = new char[endIndex - startIndex + 1];
                    strncpy(token, line + startIndex, endIndex - startIndex);
                    token[endIndex - startIndex] = '\0';
                    startIndex = endIndex;
                    fprintf(of, "#%d ERROR: Floating Numbers are not supported %s\n", lineCount, token);
                    delete[] token;
                    continue;
                }
                
            }
            int matched = 0;
            for (int i = (enum Token)LBRACK; i <= (enum Token)NOT; i++)
            {
                if (line[endIndex] == literals[i][0])
                {
                    if (i != (enum Token)AND)
                    {
                        matched = 1;
                        ++endIndex;
                        startIndex = endIndex;
                        fprintf(of, "#%d %s %s\n", lineCount, labels[i], literals[i]);
                        break;
                    }
                    else
                    {
                        if (line[endIndex + 1] == '&')
                        {
                            matched = 1;
                            endIndex += 2;
                            startIndex = endIndex;
                            fprintf(of, "#%d %s %s\n", lineCount, labels[(enum Token)AND], literals[i]);
                            break;
                        }
                    }
                }
            }
            if (matched == 0)
            {
                fprintf(of, "#%d ERROR: Unknown character %c\n", lineCount, line[endIndex]);
                ++endIndex;
                startIndex = endIndex;
            }
        }
    }
    fclose(of);
}