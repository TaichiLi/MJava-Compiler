// THIS FILE IS PART OF MJava-Compiler PROJECT
// error.h - error process
 
// Created by Li Taiji 2020-03-18
// Copyright (c) 2020 Li Taiji All rights reserved

#ifndef ERROR_H_
#define ERROR_H_

#include <string>

namespace MJava
{
    extern void errorToken(const std::string& msg);
    extern void errorSyntax(const std::string& msg);
} // namespace MJava

#endif // error.h