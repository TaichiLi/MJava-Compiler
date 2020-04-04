// THIS FILE IS PART OF MJava-Compiler PROJECT
// main.cpp - The driver of the parser
 
// Created by Li Taiji 2020-03-18
// Copyright (c) 2020 Li Taiji All rights reserved

#if defined(LEXER)
    #if defined(PARSER)
        #error Can not pass both macro definition "LEXER" and "PARSER" when compile.
    #endif
#endif

#if defined(PARSER)
    #include "parser.h"
#endif

#include "scanner.h"
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    std::string programName;
#if defined(LEXER)
    programName = "Lexer";

#elif defined(PARSER)
    programName = "Parser";

#else
    #error Please pass the macro definition "LEXER" or "PARSER" when compile.
#endif

    if (argc < 2)
    {
        std::cerr << "Missing source file!" << std::endl;
        std::cout << "Usage: " << programName << " <Source File> [Output File]\nSource file is required. Output File is \"tokenOut.txt\" by default." << std::endl;
        return 0;
    }

    if (argc > 3)
    {
        std::cerr << "Too many Arguments!" << std::endl;
        std::cout << "Usage: " << programName << " <Source File> [Output File]\nSource file is required. Output File is \"tokenOut.txt\" by default." << std::endl;
        return 0;
    }

    std::ofstream of;

    if (argc == 3)
    {
        of.open(argv[2]);
    }
    else
    {
#if defined(LEXER)
        of.open("./tokenOut.txt");

#elif defined(PARSER)
        of.open("./SyntaxOut.txt");

#else
    #error Please pass the macro definition "LEXER" or "PARSER" when compile.
#endif
    }
    
    if (of.fail())
    {
        std::cout << "Output file can not be created!" << std::endl;
        return 0;
    }
    
    MJava::Scanner scanner = MJava::Scanner(argv[1]);

#if defined(LEXER)
    while(scanner.getToken().getTokenType() != MJava::TokenType::END_OF_FILE)
    {
        of << scanner.getNextToken().toString() << std::endl;
    }

#elif defined(PARSER)
    MJava::Parser parser = MJava::Parser(scanner);
    parser.parse();
    of << parser.toString();

#else
    #error Please pass the macro definition "LEXER" or "PARSER" when compile.
#endif

    of.close();
    
    return 0;
}