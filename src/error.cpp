// THIS FILE IS PART OF MJava-Compiler PROJECT
// error.cpp - error process
 
// Created by Li Taiji 2020-03-18
// Copyright (c) 2020 Li Taiji All rights reserved

#include "error.h"

#ifndef LEXER
    #include "parser.h"
#endif

#include "scanner.h"
#include <iostream>

namespace MJava
{
    void errorToken(const std::string& msg)
    {
        std::cerr << "Token Error:" << msg << std::endl;
        Scanner::setErrorFlag(true);
    }

#ifndef LEXER
    void errorSyntax(const std::string& msg)
    {
        std::cerr << "Syntax Error: " << msg << std::endl;
        Parser::setErrorFlag(true);
    }
#endif

} // namespace MJava