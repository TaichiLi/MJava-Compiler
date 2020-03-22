// THIS FILE IS PART OF MJava-Compiler PROJECT
// lexer.cpp - The core part of the lexer
 
// Created by Li Taiji 2020-03-18
// Copyright (c) 2020 Li Taiji All rights reserved

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <map>
#include <iterator>
#include "lexer.h"

// the max length of line
const int MAX_BUFFER = 1024;
// the length of temporary variable
const int LINE_BUFFER = 256;

Lexer::Lexer()
{
    tokens = new char[MAX_BUFFER];
    toklen = 0;
}

Lexer::Lexer(const Lexer &lexer)
{
    tokens = new char[MAX_BUFFER];
    strncpy(tokens, lexer.getTokens(), MAX_BUFFER);
    toklen = lexer.getToklen();
}

Lexer::~Lexer()
{
    if (tokens != NULL)
        delete[] tokens;
}

void Lexer::parse(const char *line)
{
    lineScanner(line, this->tokens, &this->toklen);
}

char* Lexer::getTokens() const
{
    return this->tokens;
}

int Lexer::getToklen() const
{
    return this->toklen;
}

struct CharCmp
{
    bool operator()(char const *a, char const *b) const
    {
        return strcmp(a, b) < 0;
    }
};

const std::map<const char*, const char*, CharCmp> tokenLabelMap = {
    {"class", "CLASS"},
    {"public", "PUBLIC"},
    {"static", "STATIC"},
    {"void", "VOID"},
    {"main", "MAIN"},
    {"String", "STRING"},
    {"extends", "EXTENDS"},
    {"return", "RETURN"},
    {"int", "INT"},
    {"boolean", "BOOLEAN"},
    {"if", "IF"},
    {"else", "ELSE"},
    {"while", "WHILE"},
    {"System.out.println", "PRINT"},
    {"length", "LENGTH"},
    {"true", "TRUE"},
    {"false", "FALSE"},
    {"this", "THIS"},
    {"new", "NEW"},
    {"[", "LBRACK"},
    {"]", "RBRACK"},
    {"(", "LPAREN"},
    {")", "RPAREN"},
    {"{", "LBRACE"},
    {"}", "RBRACE"},
    {",", "COMMA"},
    {";", "SEMICOLON"},
    {"=", "ASSIGN"},
    {"&&", "AND"},
    {"<", "LT"},
    {"+", "ADD"},
    {"-", "SUB"},
    {"*", "MULTI"},
    {".", "DOT"},
    {"!", "NOT"}
};

void lineScanner(const char *line, char *tokens, int *toklen)
{
    tokens[0] = '\0';
    *toklen = 0;
    char *tmp = new char[LINE_BUFFER];
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
            auto it = tokenLabelMap.find(token);
            if (it != tokenLabelMap.end())
            {
                int len = sprintf(tmp, "%s %s\n", it->second, token);
                *toklen += len;
                startIndex = endIndex;
                delete[] token;
                strncat(tokens, tmp, len);
                continue;
            }
            if (strncmp("System.out.println", line + startIndex, endIndex - startIndex + 12) == 0)
            {
                // The length of "System.out.println" after 'm' is 12
                endIndex += 12;
                token = new char[endIndex - startIndex + 1];
                strncpy(token, line + startIndex, endIndex - startIndex);
                token[endIndex - startIndex] = '\0';
                startIndex = endIndex;
                int len = sprintf(tmp, "%s %s\n", "PRINT", token);
                *toklen += len;
                delete[] token;
                strncat(tokens, tmp, len);
                continue;
            }
            else
            {
                int len = sprintf(tmp, "%s %s\n", "IDENTIFIER", token);
                *toklen += len;
                strncat(tokens, tmp, len);
                startIndex = endIndex;
                continue;
            }
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
                int len = sprintf(tmp, "%s %s\n", "INTEGER", token);
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
                int index = endIndex + 1;
                while (isdigit(line[index]))
                    ++index;
                if (isalpha(line[index]) || line[index] == '_')
                {
                    char* token = new char[endIndex - startIndex + 1];
                    strncpy(token, line + startIndex, endIndex - startIndex);
                    token[endIndex - startIndex] = '\0';
                    startIndex = endIndex;
                    int len = sprintf(tmp, "%s %s\n", "INTEGER", token);
                    *toklen += len;
                    strncat(tokens, tmp, len);
                    delete[] token;
                    ++endIndex;
                    startIndex = endIndex;
                    len = sprintf(tmp, "%s %c\n", "DOT", ".");
                    *toklen += len;
                    strncat(tokens, tmp, len);
                    while (isalpha(line[index]) || isdigit(line[index]) || line[index] == '_')
                        ++index;
                    endIndex = index;
                    token = new char[endIndex - startIndex + 1];
                    strncpy(token, line + startIndex, endIndex - startIndex);
                    token[endIndex - startIndex] = '\0';
                    startIndex = endIndex;
                    if (isdigit(line[startIndex]) || line[startIndex] == '_')
                    {
                        int len = sprintf(tmp, "ERROR: Identifiers can not begin with a number %s\n", token);
                        *toklen += len;
                        strncat(tokens, tmp, len);
                        delete[] token;
                        continue;
                    }
                    else
                    {
                        int len = sprintf(tmp, "%s %s\n", "IDENTIFIER", token);
                        *toklen += len;
                        strncat(tokens, tmp, len);
                        delete[] token;
                        continue;
                    }
                }
                else
                {
                    endIndex = index;
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
        }
        char *token = new char[2];
        token[0] = line[endIndex];
        token[1] = '\0';
        auto it = tokenLabelMap.find(token);
        if (it != tokenLabelMap.end())
        {
            ++endIndex;
            startIndex = endIndex;
            int len = sprintf(tmp, "%s %s\n", it->second, it->first);
            *toklen += len;
            strncat(tokens, tmp, len);
            continue;
        }
        if (strncmp("&&", line + startIndex, 2) == 0)
        {
            endIndex += 2;
            startIndex = endIndex;
            int len = sprintf(tmp, "%s %s\n", "AND", "&&");
            *toklen += len;
            strncat(tokens, tmp, len);
            continue;
        }
        int len = sprintf(tmp, "ERROR: Unknown character %c\n", line[endIndex]);
        *toklen += len;
        strncat(tokens, tmp, len);
        ++endIndex;
        startIndex = endIndex;
    }
    delete[] tmp;
}

// scanner 
// Lexical analysis of the file line by line
void fileScanner(FILE* fp, FILE* of)
{
    char line[MAX_BUFFER];
    int lineCount = 0;
    while(fgets(line, MAX_BUFFER, fp) != NULL)
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
                auto it = tokenLabelMap.find(token);
                if (it != tokenLabelMap.end())
                {
                    fprintf(of, "#%d %s %s\n", lineCount, it->second, token);
                    delete[] token;
                    startIndex = endIndex;
                    continue;
                }
                if (strncmp("System.out.println", line + startIndex, endIndex - startIndex + 12) == 0)
                {
                    // The length of "System.out.println" after 'm' is 12
                    endIndex += 12;
                    token = new char[endIndex - startIndex + 1];
                    strncpy(token, line + startIndex, endIndex - startIndex);
                    token[endIndex - startIndex] = '\0';
                    startIndex = endIndex;
                    fprintf(of, "#%d %s %s\n", lineCount, "PRINT", token);
                    delete[] token;
                    continue;
                }
                else
                {
                    fprintf(of, "#%d %s %s\n", lineCount, "IDENTIFIER", token);
                    delete[] token;
                    startIndex = endIndex;
                    continue;
                }
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
                    fprintf(of, "#%d %s %s\n", lineCount, "INTEGER", token);
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
                    int index = endIndex + 1;
                    while (isdigit(line[index]))
                        ++index;
                    if (isalpha(line[index]) || line[index] == '_')
                    {
                        char* token = new char[endIndex - startIndex + 1];
                        strncpy(token, line + startIndex, endIndex - startIndex);
                        token[endIndex - startIndex] = '\0';
                        startIndex = endIndex;
                        fprintf(of, "#%d %s %s\n", lineCount, "INTEGER", token);
                        ++endIndex;
                        startIndex = endIndex;
                        fprintf(of, "#%d %s %c\n", lineCount, "DOT", '.');
                        delete[] token;
                        while (isalpha(line[index]) || isdigit(line[index]) || line[index] == '_')
                            ++index;
                        endIndex = index;
                        token = new char[endIndex - startIndex + 1];
                        strncpy(token, line + startIndex, endIndex - startIndex);
                        token[endIndex - startIndex] = '\0';
                        startIndex = endIndex;
                        if (isdigit(line[startIndex]) || line[startIndex] == '_')
                            fprintf(of, "#%d ERROR: Identifiers can not begin with a number %s\n", lineCount, token);
                        else 
                            fprintf(of, "#%d %s %s\n", lineCount, "IDENTIFIER", token);
                        delete[] token;
                        continue;
                    }
                    else
                    {
                        endIndex = index;
                        char* token = new char[endIndex - startIndex + 1];
                        strncpy(token, line + startIndex, endIndex - startIndex);
                        token[endIndex - startIndex] = '\0';
                        startIndex = endIndex;
                        fprintf(of, "#%d ERROR: Floating Numbers are not supported %s\n", lineCount, token);
                        delete[] token;
                        continue;
                    }
                }
            }
            char *token = new char[2];
            token[0] = line[endIndex];
            token[1] = '\0';
            auto it = tokenLabelMap.find(token);
            if (it != tokenLabelMap.end())
            {
                ++endIndex;
                startIndex = endIndex;
                fprintf(of, "#%d %s %s\n", lineCount, it->second, it->first);
                continue;
            }
            if (strncmp("&&", line + startIndex, 2) == 0)
            {
                endIndex += 2;
                startIndex = endIndex;
                fprintf(of, "#%d %s %s\n", lineCount, "AND", "&&");
                continue;
            }
            fprintf(of, "#%d ERROR: Unknown character %c\n", lineCount, line[endIndex]);
            ++endIndex;
            startIndex = endIndex;
        }
    }
}