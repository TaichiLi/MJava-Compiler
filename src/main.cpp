// THIS FILE IS PART OF MJava-Compiler PROJECT
// main.cpp - The driver of the lexer
 
// Created by Li Taiji 2020-03-18
// Copyright (c) 2020 Li Taiji All rights reserved

#include <cstdio>
#include "lexer.h"

int main(int argc,char** argv)
{
    if (argc < 2)
    {
        printf("Missing source file!\n");
        printf("Usage: Lexer.exe <Source File> [Output File]\nSource file is required. Output File is \"tokenOut.txt\" by default.\n");
        return 0;
    }
    if (argc > 3)
    {
        printf("Too many Arguments!\n");
        printf("Usage: Lexer.exe <Source File> [Output File]\nSource file is required. Output File is \"tokenOut.txt\" by default.\n");
        return 0;
    }
    FILE *sf = NULL;
    if ((sf = fopen(argv[1], "r")) == NULL)
    {
        printf("Source file do not exist!");
        return 0;
    }
    if (argc == 3)
    {
        FILE *of = fopen(argv[2], "w+");
        if (of == NULL)
        {
            printf("Output file can not be created!");
            return 0;
        }
        lexer::fileScanner(sf, of);
        fclose(sf);
        fclose(of);
    }
    FILE *of = fopen("./tokenOut.txt", "w+");
    if (of == NULL)
    {
        printf("Output file can not be created!");
        return 0;
    }
    lexer::fileScanner(sf, of);
    fclose(sf);
    fclose(of);
    return 0;
}