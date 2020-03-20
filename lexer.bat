@echo off
if exist .\bin\Lexer.exe (
    .\bin\Lexer.exe %1 %2
) else ( 
    g++ .\src\main.cpp .\src\lexer.cpp -I .\include -o .\bin\Lexer.exe && .\bin\Lexer.exe %1 %2
)
