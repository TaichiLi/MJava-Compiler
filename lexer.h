#ifndef LEXER_H
#define LEXER_H

#include <cstdio>
#include <cstdlib>

extern const char *labels[];
extern const char *literals[];

void lineScanner(const char *line, char *tokens, int *toklen);
void fileScanner(FILE *fp);

class Lexer
{
    public:
    Lexer()
    {
        tokens = (char *)malloc(1024);
        toklen = 0;
    }

    void parse(const char *line)
    {
        lineScanner(line, this->tokens, &this->toklen);
    }

    char* getTokens()
    {
        return this->tokens;
    }

    int getToklen()
    {
        return this->toklen;
    }
    
    private:
        char *tokens;
        int toklen;
};

#endif