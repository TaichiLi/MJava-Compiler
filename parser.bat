@echo off
if exist .\bin (
    if exist .\bin\Parser.exe (
        .\bin\Parser.exe %1 %2
    ) else ( 
        g++ src/main.cpp src/scanner.cpp src/error.cpp src/dictionary.cpp src/token.cpp src/ast.cpp src/parser.cpp src/jsonformatter.cpp -I ./include -DPARSER -o .\bin\Parser.exe && .\bin\Parser.exe %1 %2
    )
) else (
    md .\bin && g++ src/main.cpp src/scanner.cpp src/error.cpp src/dictionary.cpp src/token.cpp src/ast.cpp src/parser.cpp src/jsonformatter.cpp -I ./include -DPARSER -o .\bin\Parser.exe && .\bin\Parser.exe %1 %2
)