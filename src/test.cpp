#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "lexer.h"

static int test_count = 0;
static int test_pass = 0;

#define EXPECT_EQ_BASE(equality, expect, actual, format) \
    do {\
        test_count++;\
        if (equality)\
            test_pass++;\
        else {\
            fprintf(stderr, "%s:%d: expect: " format " actual: " format "\n", __FILE__, __LINE__, expect, actual);\
        }\
    } while(0)

#define EXPECT_EQ_STRING(expect, actual, alength) \
    EXPECT_EQ_BASE(sizeof(expect) - 1 == alength && memcmp(expect, actual, alength + 1) == 0, expect, actual, "%s")

void testKeywords()
{
    Lexer lexer = Lexer();
    lexer.parse("class");
    EXPECT_EQ_STRING("CLASS class\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("public");
    EXPECT_EQ_STRING("PUBLIC public\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("static");
    EXPECT_EQ_STRING("STATIC static\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("void");
    EXPECT_EQ_STRING("VOID void\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("main");
    EXPECT_EQ_STRING("MAIN main\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("String");
    EXPECT_EQ_STRING("STRING String\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("extends");
    EXPECT_EQ_STRING("EXTENDS extends\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("return");
    EXPECT_EQ_STRING("RETURN return\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("int");
    EXPECT_EQ_STRING("INT int\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("boolean");
    EXPECT_EQ_STRING("BOOLEAN boolean\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("if");
    EXPECT_EQ_STRING("IF if\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("else");
    EXPECT_EQ_STRING("ELSE else\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("while");
    EXPECT_EQ_STRING("WHILE while\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("System.out.println");
    EXPECT_EQ_STRING("PRINT System.out.println\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("length");
    EXPECT_EQ_STRING("LENGTH length\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("true");
    EXPECT_EQ_STRING("TRUE true\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("false");
    EXPECT_EQ_STRING("FALSE false\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("this");
    EXPECT_EQ_STRING("THIS this\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("new");
    EXPECT_EQ_STRING("NEW new\n", lexer.getTokens(), lexer.getToklen());
}

void testSymbols()
{
    Lexer lexer = Lexer();
    lexer.parse("[");
    EXPECT_EQ_STRING("LBRACK [\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("]");
    EXPECT_EQ_STRING("RBRACK ]\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("(");
    EXPECT_EQ_STRING("LPAREN (\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse(")");
    EXPECT_EQ_STRING("RPAREN )\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("{");
    EXPECT_EQ_STRING("LBRACE {\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("}");
    EXPECT_EQ_STRING("RBRACE }\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse(",");
    EXPECT_EQ_STRING("COMMA ,\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse(";");
    EXPECT_EQ_STRING("SEMICOLON ;\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("=");
    EXPECT_EQ_STRING("ASSIGN =\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("&&");
    EXPECT_EQ_STRING("AND &&\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("<");
    EXPECT_EQ_STRING("LT <\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("+");
    EXPECT_EQ_STRING("ADD +\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("-");
    EXPECT_EQ_STRING("SUB -\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("*");
    EXPECT_EQ_STRING("MULTI *\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse(".");
    EXPECT_EQ_STRING("DOT .\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("!");
    EXPECT_EQ_STRING("NOT !\n", lexer.getTokens(), lexer.getToklen());
}

void testError()
{
    Lexer lexer = Lexer();
    lexer.parse("0.0");
    EXPECT_EQ_STRING("ERROR: Floating Numbers are not supported 0.0\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("&");
    EXPECT_EQ_STRING("ERROR: Unknown character &\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("_abc123");
    EXPECT_EQ_STRING("ERROR: Identifiers can not begin with an underscore _abc123\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("123abc");
    EXPECT_EQ_STRING("ERROR: Identifiers can not begin with a number 123abc\n", lexer.getTokens(), lexer.getToklen());
}

int main()
{
    testKeywords();
    testSymbols();
    testError();
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return 0;
}