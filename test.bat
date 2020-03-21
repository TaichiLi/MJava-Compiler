@echo off
if exist .\bin (
    if exist .\bin\Lexer_test.exe (
        .\bin\Lexer_test.exe
    ) else ( 
        g++ .\src\test.cpp .\src\lexer.cpp -I .\include -o .\bin\Lexer_test.exe && .\bin\Lexer_test.exe
    )
) else (
    md .\bin && g++ .\src\test.cpp .\src\lexer.cpp -I .\include -o .\bin\Lexer_test.exe && .\bin\Lexer_test.exe
)