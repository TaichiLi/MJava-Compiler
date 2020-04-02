// THIS FILE IS PART OF MJava-Compiler PROJECT
// jsonformatter.cpp - json formatter
 
// Created by Li Taiji 2020-03-28
// Copyright (c) 2020 Li Taiji All rights reserved

#include "jsonformatter.h"
#include <cctype>
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
    bool inString = false;
    char last = 0;
    int length = JSONString.size();

    for (int i = 0; i < length; i++) {
        char c = JSONString.at(i);
        switch (c)
        {
            case '\"':
            {
                inString = !inString;
                FormattedJSON << c;
                break;
            }
            
            case '{':
            {
                tabNum++;
                FormattedJSON << c << "\n";
                FormattedJSON << getSpaceOrTab(tabNum);
                break;
            }

            case '}':
            {
                tabNum--;
                FormattedJSON << "\n";
                FormattedJSON << getSpaceOrTab(tabNum);
                FormattedJSON << c;
                break;
            }

            case ',':
            {
                FormattedJSON << c << "\n";
                FormattedJSON << getSpaceOrTab(tabNum);
                break;
            }

            case ':':
            {
                FormattedJSON << c << " ";
                break;
            }

            case '[':
            {
                tabNum++;
                char next = JSONString.at(i + 1);

                if (next == ']')
                {
                    FormattedJSON << c;
                }
                else
                {
                    FormattedJSON << c << "\n";
                    FormattedJSON << getSpaceOrTab(tabNum);
                }

                break;
            }

            case ']':
            {
                tabNum--;

                if (last == '[')
                {
                    FormattedJSON << c;
                }
                else
                {
                    FormattedJSON << "\n" << getSpaceOrTab(tabNum) << c;
                }

                break;
            }

            default:
            {
                if (inString || !isspace(c))
                {
                    FormattedJSON << c;
                }
                
                break;
            }
        }

        last = c;
    }
    
    return FormattedJSON.str();
}