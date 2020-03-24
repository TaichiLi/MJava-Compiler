// THIS FILE IS PART OF MJava-Compiler PROJECT
// main.cpp - The driver of the lexer
 
// Created by Li Taiji 2020-03-18
// Copyright (c) 2020 Li Taiji All rights reserved

#include <fstream>
#include <string>
#include "scanner.h"

int main(int argc,char** argv)
{
    if (argc < 2)
    {
        std::cerr <<"Missing source file!\n";
        std::cout <<"Usage: Lexer.exe <Source File> [Output File]\nSource file is required. Output File is \"tokenOut.txt\".\n";
        return 0;
    }
    if (argc > 3)
    {
        std::cerr << "Too many Arguments!\n";
        std::cout << "Usage: Lexer.exe <Source File> [Output File]\nSource file is required. Output File is \"tokenOut.txt\".\n";
        return 0;
    }
    if (argc == 3)
    {
        std::ofstream of;
        of.open(std::string(argv[2]));
        if (of.fail())
        {
            std::cout << "Output file can not be created!";
            return 0;
        }
        MJava::Scanner scanner = MJava::Scanner(argv[1]);
        while(scanner.getToken().getTokenType() != MJava::TokenType::END_OF_FILE)
        {
            of << scanner.getNextToken().toString() << std::endl;;
        }
        of.close();
    }
    std::ofstream of;
    of.open("./tokenOut.txt");
    if (of.fail())
    {
        std::cout << "Output file can not be created!";
        return 0;
    }
    MJava::Scanner scanner = MJava::Scanner(argv[1]);
    while(scanner.getToken().getTokenType() != MJava::TokenType::END_OF_FILE)
    {
        of << scanner.getNextToken().toString() << std::endl;
    }
    of.close();
    return 0;
}