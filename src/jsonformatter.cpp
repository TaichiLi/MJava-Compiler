// THIS FILE IS PART OF MJava-Compiler PROJECT
// jsonformatter.cpp - json formatter
 
// Created by Li Taiji 2020-03-28
// Copyright (c) 2020 Li Taiji All rights reserved

#include "jsonformatter.h"
#include <sstream>
#include <string>

std::string JSONFormatter::getSpaceOrTab(int tabNum)
{
    std::ostringstream tab;
    for (int i = 0; i < tabNum; i++) {
        tab << "\t";
    }
    return tab.str();
}

std::string JSONFormatter::format(const std::string& JSONString)
{
    int tabNum = 0;
    std::ostringstream FormattedJSON;
    int length = JSONString.size();

    char last = 0;
    for (int i = 0; i < length; i++) {
        char c = JSONString.at(i);
        if (isspace(c))
            continue;
        else if (c == '{') {
            tabNum++;
            FormattedJSON << c << "\n";
            FormattedJSON << getSpaceOrTab(tabNum);
        }
        else if (c == '}') {
            tabNum--;
            FormattedJSON << "\n";
            FormattedJSON << getSpaceOrTab(tabNum);
            FormattedJSON << c;
        }
        else if (c == ',') {
            FormattedJSON << c << "\n";
            FormattedJSON << getSpaceOrTab(tabNum);
        }
        else if (c == ':') {
            FormattedJSON << c << " ";
        }
        else if (c == '[') {
            tabNum++;
            char next = JSONString.at(i + 1);
            if (next == ']') {
                FormattedJSON << c;
            }
            else {
                FormattedJSON << c << "\n";
                FormattedJSON << getSpaceOrTab(tabNum);
            }
        }
        else if (c == ']') {
            tabNum--;
            if (last == '[') {
                FormattedJSON << c;
            }
            else {
                FormattedJSON << "\n" << getSpaceOrTab(tabNum) << c;
            }
        }
        else {
            FormattedJSON << c;
        }
        last = c;
    }
    return FormattedJSON.str();
}