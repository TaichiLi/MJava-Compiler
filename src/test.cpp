// THIS FILE IS PART OF MJava-Compiler PROJECT
// test.cpp - The unit test of the lexer
 
// Created by Li Taiji 2020-03-18
// Copyright (c) 2020 Li Taiji All rights reserved

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "lexer.h"

using lexer::Lexer;

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

void testIdentifiers()
{
    Lexer lexer = Lexer();
    lexer.parse("a");
    EXPECT_EQ_STRING("IDENTIFIER a\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("abc");
    EXPECT_EQ_STRING("IDENTIFIER abc\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("abc123");
    EXPECT_EQ_STRING("IDENTIFIER abc123\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("abc_123");
    EXPECT_EQ_STRING("IDENTIFIER abc_123\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("abc123_");
    EXPECT_EQ_STRING("IDENTIFIER abc123_\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("a1b2_c3");
    EXPECT_EQ_STRING("IDENTIFIER a1b2_c3\n", lexer.getTokens(), lexer.getToklen());
}

void testInteger()
{
    Lexer lexer = Lexer();
    lexer.parse("0");
    EXPECT_EQ_STRING("INTEGER 0\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("10");
    EXPECT_EQ_STRING("INTEGER 10\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("1234567890");
    EXPECT_EQ_STRING("INTEGER 1234567890\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("0123456789");
    EXPECT_EQ_STRING("INTEGER 0123456789\n", lexer.getTokens(), lexer.getToklen());
}

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

void testExpression()
{
    Lexer lexer = Lexer();
    lexer.parse("class Foo extends Bar{}");
    EXPECT_EQ_STRING("CLASS class\nIDENTIFIER Foo\nEXTENDS extends\nIDENTIFIER Bar\nLBRACE {\nRBRACE }\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("public static void main(int number, String string, boolean bool);");
    EXPECT_EQ_STRING("PUBLIC public\nSTATIC static\nVOID void\nMAIN main\nLPAREN (\nINT int\nIDENTIFIER number\nCOMMA ,\nSTRING String\nIDENTIFIER string\nCOMMA ,\nBOOLEAN boolean\nIDENTIFIER bool\nRPAREN )\nSEMICOLON ;\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("this.array = new int[5];");
    EXPECT_EQ_STRING("THIS this\nDOT .\nIDENTIFIER array\nASSIGN =\nNEW new\nINT int\nLBRACK [\nINTEGER 5\nRBRACK ]\nSEMICOLON ;\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("if (!array.length < 5) return false; else System.out.println(5);");
    EXPECT_EQ_STRING("IF if\nLPAREN (\nNOT !\nIDENTIFIER array\nDOT .\nLENGTH length\nLT <\nINTEGER 5\nRPAREN )\nRETURN return\nFALSE false\nSEMICOLON ;\nELSE else\nPRINT System.out.println\nLPAREN (\nINTEGER 5\nRPAREN )\nSEMICOLON ;\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("while(true);");
    EXPECT_EQ_STRING("WHILE while\nLPAREN (\nTRUE true\nRPAREN )\nSEMICOLON ;\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("a0.b_ = 5;");
    EXPECT_EQ_STRING("IDENTIFIER a0\nDOT .\nIDENTIFIER b_\nASSIGN =\nINTEGER 5\nSEMICOLON ;\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("a1 = b2.get();");
    EXPECT_EQ_STRING("IDENTIFIER a1\nASSIGN =\nIDENTIFIER b2\nDOT .\nIDENTIFIER get\nLPAREN (\nRPAREN )\nSEMICOLON ;\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("(1 + 2) * (3 - 4)");
    EXPECT_EQ_STRING("LPAREN (\nINTEGER 1\nADD +\nINTEGER 2\nRPAREN )\nMULTI *\nLPAREN (\nINTEGER 3\nSUB -\nINTEGER 4\nRPAREN )\n", lexer.getTokens(), lexer.getToklen());
}

void testError()
{
    Lexer lexer = Lexer();
    lexer.parse("0.0");
    EXPECT_EQ_STRING("ERROR: Floating Numbers are not supported 0.0\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("&");
    EXPECT_EQ_STRING("ERROR: Unknown character &\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse(">");
    EXPECT_EQ_STRING("ERROR: Unknown character >\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("|");
    EXPECT_EQ_STRING("ERROR: Unknown character |\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("/");
    EXPECT_EQ_STRING("ERROR: Unknown character /\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("_abc123");
    EXPECT_EQ_STRING("ERROR: Identifiers can not begin with an underscore _abc123\n", lexer.getTokens(), lexer.getToklen());
    lexer.parse("123abc");
    EXPECT_EQ_STRING("ERROR: Identifiers can not begin with a number 123abc\n", lexer.getTokens(), lexer.getToklen());
}

int main()
{
    testIdentifiers();
    testInteger();
    testKeywords();
    testSymbols();
    testExpression();
    testError();
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return 0;
}