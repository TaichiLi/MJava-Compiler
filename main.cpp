// THIS FILE IS PART OF MJava-Compiler PROJECT
// lexer.c - The core part of the lexer
 
// Created by Li Taiji 2020-03-18
// Copyright (c) 2020 Li Taiji All rights reserved

#include <cstdio>
#include "lexer.h"

int main(int argc,char** argv)
{
    if (argc < 2)
    {
        printf("Missing source file!\n");
        return 0;
    }
    if (argc > 2)
    {
        printf("Too many Arguments!\n");
        return 0;
    }
    FILE *sf = NULL;
    if ((sf = fopen(argv[1], "r")) == NULL)
    {
        printf("Source file do not exist!");
        return 0;
    }
    fileScanner(sf);
    return 0;
}