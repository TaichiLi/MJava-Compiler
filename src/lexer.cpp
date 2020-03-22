// THIS FILE IS PART OF MJava-Compiler PROJECT
// lexer.cpp - The core part of the lexer
 
// Created by Li Taiji 2020-03-18
// Copyright (c) 2020 Li Taiji All rights reserved

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <iterator>
#include <unordered_map>
#include "lexer.h"

// the max length of line
const int MAX_BUFFER = 1024;
// the length of temporary variable
const int LINE_BUFFER = 256;

const std::unordered_map<char*, TokenType> tokenTypeMap = {
    {"class", CLASS},
    {"public", PUBLIC},
    {"static", STATIC},
    {"void", VOID},
    {"main", MAIN},
    {"String", STRING},
    {"extends", EXTENDS},
    {"return", RETURN},
    {"int", INT},
    {"boolean", BOOLEAN},
    {"if", IF},
    {"else", ELSE},
    {"while", WHILE},
    {"System.out.println", PRINT},
    {"length", LENGTH},
    {"true", TRUE},
    {"false", FALSE},
    {"this", THIS},
    {"new", NEW},
    {"[", LBRACK},
    {"]", RBRACK},
    {"(", LPAREN},
    {")", RPAREN},
    {"{", LBRACE},
    {"}", RBRACE},
    {",", COMMA},
    {";", SEMICOLON},
    {"=", ASSIGN},
    {"&&", AND},
    {"<", LT},
    {"+", ADD},
    {"-", SUB},
    {"*", MULTI},
    {".", DOT},
    {"!", NOT}
};

void Token::setTokenType(TokenType tokenType)
{
    this->tokenType = tokenType;
}

void Token::setValue(int integer)
{
    this->value.integer = integer;
}

void Token::setValue(bool boolean)
{
    this->value.boolean = boolean;
}

void Token::setValue(char* text)
{
    this->value.literal = text;
}

TokenType Token::getTokenType() const
{
    return this->tokenType;
}

Value Token::getValue() const
{
    return this->value;
}

Lexer::Lexer()
{
    this->text = NULL;
}

Lexer::Lexer(int length)
{
    this->text = new char[length];
}

Lexer::Lexer(const Lexer &lexer)
{
    this->text = new char[strlen(lexer.getText()) + 1];
    strcpy(this->text, lexer.getText());
}

Lexer::~Lexer()
{
    if (this->text != NULL)
        delete[] this->text;
}

void Lexer::setText(const char *text)
{
    this->text = new char[strlen(text) + 1];
    strcpy(this->text, text);
}

char* Lexer::getText() const
{
    return this->text;
}

 LexerParseState Lexer::getToken(Token *token)
{
    int index = 0;
    if (this->text[index] != '\0')
    {
        while (isspace(this->text[index]))
            ++index;
        if (this->text[index] != '\0')
            return LEXER_PARSE_EMPTY;
        if (this->text[index] == '_')
        {
            while (isalpha(this->text[index]) || isdigit(this->text[index]) || this->text[index] == '_')
                ++index;
            char *literal = new char[index + 1];
            strncpy(literal, this->text, index);
            literal[index] = '\0';
            token = new Token();
            token->setTokenType(ERROR);
            token->setValue(literal);
            this->text += index;
            return LEXER_PARSE_INVALID_IDENTIFIER;
        }
        if (isalpha(this->text[index]))
        {
            while (isalpha(this->text[index]) || isdigit(this->text[index]) || this->text[index] == '_')
                ++index;
            char *literal = new char[index + 1];
            strncpy(literal, this->text, index);
            literal[index] = '\0';
            for (auto it = tokenTypeMap.begin(); it != tokenTypeMap.end(); ++it)
            {
                if (it->second == (enum TokenType)PRINT)
                {
                    if (strncmp(it->first, this->text, index + 12) == 0)
                    {
                        // The length of "System.out.println" after 'm' is 12
                        index += 12;
                        literal = new char[index + 1];
                        strncpy(literal, this->text, index);
                        literal[index - startIndex] = '\0';
                        token = new Token();
                        token->setTokenType((enum TokenType)PRINT);
                        token->setValue(literal);
                        this->text += index;
                        break;
                    }
                }
                if (strcmp(it->first, literal) == 0)
                {
                    token = new Token();
                    token->setTokenType(it->second);
                    token->setValue(literal);
                    this->text += index;
                    break;
                }
            }
            if (index != 0)
            {
                token = new Token();
                token->setTokenType((enum TokenType)IDENTIFIER);
                token->setValue(literal);
                this->text += index;
            }
            return LEXER_PARSE_OK;
        }
        if (isdigit(this->text[index]))
        {
            while (isdigit(this->text[index]))
                ++index;
            if (!isalpha(this->text[index]) && this->text[index] != '_' && this->text[index] != '.')
            {
                char* literal = new char[index + 1];
                strncpy(literal, this->text, index);
                literal[index] = '\0';
                token = new Token();
                token->setTokenType((enum TokenType)INTEGER);
                token->setValue(literal);
                this->text += index;
                return LEXER_PARSE_OK;
            }
            else if (isalpha(this->text[index]) || this->text[index] == '_')
            {
                while (isalpha(this->text[index]) || isdigit(this->text[index]) || this->text[index] == '_')
                    ++index;
                char* literal = new char[index + 1];
                strncpy(literal, this->text, index);
                literal[index] = '\0';
                token = new Token();
                token->setTokenType((enum TokenType)ERROR);
                token->setValue(literal);
                this->text += index;
                return LEXER_PARSE_INVALID_IDENTIFIER;
            }
            else if (this->text[index] == '.')
            {
                if (isdigit(this->text[index + 1]))
                {
                    ++index;
                    while (isdigit(this->text[index]))
                        ++index;
                    char* literal = new char[index + 1];
                    strncpy(literal, this->text, index);
                    literal[index] = '\0';
                    token = new Token();
                    token->setTokenType((enum TokenType)ERROR);
                    token->setValue(literal);
                    this->text += index;
                    return LEXER_PARSE_INVALID_NUMBER;
                }
                char* literal = new char[index + 1];
                strncpy(literal, this->text, index);
                literal[index] = '\0';
                token = new Token();
                token->setTokenType((enum TokenType)INTEGER);
                token->setValue(literal);
                this->text += index;
                return LEXER_PARSE_OK;
            }
            
        }
        bool matched = false;
        for (auto it = tokenTypeMap.begin(); it != tokenTypeMap.end(); ++it)
        {
            if (this->text[index] == it->first[0])
            {
                if (it->second != (enum TokenType)AND)
                {
                    matched = true;
                    ++index;
                    token = new Token();
                    token->setTokenType(it->second);
                    token->setValue(it->first);
                    this->text += index;
                    return LEXER_PARSE_OK;
                }
                else
                {
                    if (this->text[index + 1] == '&')
                    {
                        matched = true;
                        index += 2;
                        token = new Token();
                        token->setTokenType(it->second);
                        token->setValue(it->first);
                        this->text += index;
                        return LEXER_PARSE_OK;
                    }
                }
            }
        }
        if (!matched)
        {
            token = new Token();
            token->setTokenType(ERROR);
            char *literal = new char[2];
            literal[0] = this->text[index];
            literal[1] = '\0';
            token->setValue(literal);
            ++index;
            this->text += index;
            return LEXER_PARSE_INVAILD_CHARACTERS;
        }
    }
}

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
            for (int i = (enum TokenType)CLASS; i <= (enum TokenType)NEW; i++)
            {
                if (i == (enum TokenType)PRINT)
                {
                    if (strncmp(literals[i], line + startIndex, endIndex - startIndex + 12) == 0)
                    {
                        // The length of "System.out.println" after 'm' is 12
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
                int len = sprintf(tmp, "%s %s\n", labels[(enum TokenType)IDENTIFIER], token);
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
                int len = sprintf(tmp, "%s %s\n", labels[(enum TokenType)INTEGER], token);
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
                if (isdigit(line[endIndex + 1]))
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
                continue;
            }
            
        }
        bool matched = false;
        for (int i = (enum TokenType)LBRACK; i <= (enum TokenType)NOT; i++)
        {
            if (line[endIndex] == literals[i][0])
            {
                if (i != (enum TokenType)AND)
                {
                    matched = true;
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
                        matched = true;
                        endIndex += 2;
                        startIndex = endIndex;
                        int len = sprintf(tmp, "%s %s\n", labels[(enum TokenType)AND], literals[i]);
                        *toklen += len;
                        strncat(tokens, tmp, len);
                        break;
                    }
                }
            }
        }
        if (!matched)
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
                for (int i = (enum TokenType)CLASS; i <= (enum TokenType)NEW; i++)
                {
                    if (i == (enum TokenType)PRINT)
                    {
                        if (strncmp(literals[i], line + startIndex, endIndex - startIndex + 12) == 0)
                        {
                            // The length of "System.out.println" after 'm' is 12
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
                    fprintf(of, "#%d %s %s\n", lineCount, labels[(enum TokenType)IDENTIFIER], token);
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
                    fprintf(of, "#%d %s %s\n", lineCount, labels[(enum TokenType)INTEGER], token);
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
                    if (isdigit(line[endIndex]))
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
                    continue;
                }
                
            }
            bool matched = false;
            for (int i = (enum TokenType)LBRACK; i <= (enum TokenType)NOT; i++)
            {
                if (line[endIndex] == literals[i][0])
                {
                    if (i != (enum TokenType)AND)
                    {
                        matched = true;
                        ++endIndex;
                        startIndex = endIndex;
                        fprintf(of, "#%d %s %s\n", lineCount, labels[i], literals[i]);
                        break;
                    }
                    else
                    {
                        if (line[endIndex + 1] == '&')
                        {
                            matched = true;
                            endIndex += 2;
                            startIndex = endIndex;
                            fprintf(of, "#%d %s %s\n", lineCount, labels[(enum TokenType)AND], literals[i]);
                            break;
                        }
                    }
                }
            }
            if (!matched)
            {
                fprintf(of, "#%d ERROR: Unknown character %c\n", lineCount, line[endIndex]);
                ++endIndex;
                startIndex = endIndex;
            }
        }
    }
}