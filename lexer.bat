@echo off
if exist .\bin (
    if exist .\bin\Lexer.exe (
    .\bin\Lexer.exe %1 %2
    ) else ( 
        g++ src/main.cpp src/scanner.cpp src/error.cpp src/dictionary.cpp src/token.cpp -I ./include -DLEXER -o .\bin\Lexer.exe && .\bin\Lexer.exe %1 %2
    )
) else (
    md .\bin && g++ src/main.cpp src/scanner.cpp src/error.cpp src/dictionary.cpp src/token.cpp -I ./include -DLEXER -o .\bin\Lexer.exe && .\bin\Lexer.exe %1 %2
)