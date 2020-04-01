// THIS FILE IS PART OF MJava-Compiler PROJECT
// jsonformatter.h - json formatter
 
// Created by Li Taiji 2020-03-28
// Copyright (c) 2020 Li Taiji All rights reserved

#ifndef JSONFORMATTER_H_
#define JSONFORMATTER_H_

#include <string>

class JSONFormatter
{
public:
    static std::string format(const std::string &JSONString);
    
private: 
    static std::string getSpaceOrTab(int tabNum);
};

#endif // jsonformatter.h