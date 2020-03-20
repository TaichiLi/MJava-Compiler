#include <cstdio>
#include "gtest/gtest.h"
#include "lexer.h"

TEST(LexerTest, HandleIdentifierInput) {
    Lexer lexer = Lexer();
    lexer.parse("a");
    EXPECT_STREQ("IDENTIFIER a\n", lexer.getTokens());
    lexer.parse("abc");
    EXPECT_STREQ("IDENTIFIER abc\n", lexer.getTokens());
    lexer.parse("abc123");
    EXPECT_STREQ("IDENTIFIER abc123\n", lexer.getTokens());
    lexer.parse("abc_123");
    EXPECT_STREQ("IDENTIFIER abc_123\n", lexer.getTokens());
    lexer.parse("abc123_");
    EXPECT_STREQ("IDENTIFIER abc123_\n", lexer.getTokens());
    lexer.parse("a1b2_c3");
    EXPECT_STREQ("IDENTIFIER a1b2_c3\n", lexer.getTokens());
}
    
TEST(LexerTest, HandleIntegerInput) {
    Lexer lexer = Lexer();
    lexer.parse("0");
    EXPECT_STREQ("INTEGER 0\n", lexer.getTokens());
    lexer.parse("10");
    EXPECT_STREQ("INTEGER 10\n", lexer.getTokens());
    lexer.parse("1234567890");
    EXPECT_STREQ("INTEGER 1234567890\n", lexer.getTokens());
    lexer.parse("0123456789");
    EXPECT_STREQ("INTEGER 0123456789\n", lexer.getTokens());
}

TEST(LexerTest, HandleKeywordInput) {
    Lexer lexer = Lexer();
    lexer.parse("class");
    EXPECT_STREQ("CLASS class\n", lexer.getTokens());
    lexer.parse("public");
    EXPECT_STREQ("PUBLIC public\n", lexer.getTokens());
    lexer.parse("static");
    EXPECT_STREQ("STATIC static\n", lexer.getTokens());
    lexer.parse("void");
    EXPECT_STREQ("VOID void\n", lexer.getTokens());
    lexer.parse("main");
    EXPECT_STREQ("MAIN main\n", lexer.getTokens());
    lexer.parse("String");
    EXPECT_STREQ("STRING String\n", lexer.getTokens());
    lexer.parse("extends");
    EXPECT_STREQ("EXTENDS extends\n", lexer.getTokens());
    lexer.parse("return");
    EXPECT_STREQ("RETURN return\n", lexer.getTokens());
    lexer.parse("int");
    EXPECT_STREQ("INT int\n", lexer.getTokens());
    lexer.parse("boolean");
    EXPECT_STREQ("BOOLEAN boolean\n", lexer.getTokens());
    lexer.parse("if");
    EXPECT_STREQ("IF if\n", lexer.getTokens());
    lexer.parse("else");
    EXPECT_STREQ("ELSE else\n", lexer.getTokens());
    lexer.parse("while");
    EXPECT_STREQ("WHILE while\n", lexer.getTokens());
    lexer.parse("System.out.println");
    EXPECT_STREQ("PRINT System.out.println\n", lexer.getTokens());
    lexer.parse("length");
    EXPECT_STREQ("LENGTH length\n", lexer.getTokens());
    lexer.parse("true");
    EXPECT_STREQ("TRUE true\n", lexer.getTokens());
    lexer.parse("false");
    EXPECT_STREQ("FALSE false\n", lexer.getTokens());
    lexer.parse("this");
    EXPECT_STREQ("THIS this\n", lexer.getTokens());
    lexer.parse("new");
    EXPECT_STREQ("NEW new\n", lexer.getTokens());
}

TEST(LexerTest, HandleSymbolInput) {
    Lexer lexer = Lexer();
    lexer.parse("[");
    EXPECT_STREQ("LBRACK [\n", lexer.getTokens());
    lexer.parse("]");
    EXPECT_STREQ("RBRACK ]\n", lexer.getTokens());
    lexer.parse("(");
    EXPECT_STREQ("LPAREN (\n", lexer.getTokens());
    lexer.parse(")");
    EXPECT_STREQ("RPAREN )\n", lexer.getTokens());
    lexer.parse("{");
    EXPECT_STREQ("LBRACE {\n", lexer.getTokens());
    lexer.parse("}");
    EXPECT_STREQ("RBRACE }\n", lexer.getTokens());
    lexer.parse(",");
    EXPECT_STREQ("COMMA ,\n", lexer.getTokens());
    lexer.parse(";");
    EXPECT_STREQ("SEMICOLON ;\n", lexer.getTokens());
    lexer.parse("=");
    EXPECT_STREQ("ASSIGN =\n", lexer.getTokens());
    lexer.parse("&&");
    EXPECT_STREQ("AND &&\n", lexer.getTokens());
    lexer.parse("<");
    EXPECT_STREQ("LT <\n", lexer.getTokens());
    lexer.parse("+");
    EXPECT_STREQ("ADD +\n", lexer.getTokens());
    lexer.parse("-");
    EXPECT_STREQ("SUB -\n", lexer.getTokens());
    lexer.parse("*");
    EXPECT_STREQ("MULTI *\n", lexer.getTokens());
    lexer.parse(".");
    EXPECT_STREQ("DOT .\n", lexer.getTokens());
    lexer.parse("!");
    EXPECT_STREQ("NOT !\n", lexer.getTokens());
}

TEST(LexerTest, HandleExpressionInput) {
    Lexer lexer = Lexer();
    lexer.parse("class Foo extends Bar{}");
    EXPECT_STREQ("CLASS class\nIDENTIFIER Foo\nEXTENDS extends\nIDENTIFIER Bar\nLBRACE {\nRBRACE }\n", lexer.getTokens());
    lexer.parse("public static void main(int number, String string, boolean bool);");
    EXPECT_STREQ("PUBLIC public\nSTATIC static\nVOID void\nMAIN main\nLPAREN (\nINT int\nIDENTIFIER number\nCOMMA ,\nSTRING String\nIDENTIFIER string\nCOMMA ,\nBOOLEAN boolean\nIDENTIFIER bool\nRPAREN )\nSEMICOLON ;\n", lexer.getTokens());
    lexer.parse("this.array = new int[5];");
    EXPECT_STREQ("THIS this\nDOT .\nIDENTIFIER array\nASSIGN =\nNEW new\nINT int\nLBRACK [\nINTEGER 5\nRBRACK ]\nSEMICOLON ;\n", lexer.getTokens());
    lexer.parse("if (!array.length < 5) return false; else System.out.println(5);");
    EXPECT_STREQ("IF if\nLPAREN (\nNOT !\nIDENTIFIER array\nDOT .\nLENGTH length\nLT <\nINTEGER 5\nRPAREN )\nRETURN return\nFALSE false\nSEMICOLON ;\nELSE else\nPRINT System.out.println\nLPAREN (\nINTEGER 5\nRPAREN )\nSEMICOLON ;\n", lexer.getTokens());
    lexer.parse("while(true);");
    EXPECT_STREQ("WHILE while\nLPAREN (\nTRUE true\nRPAREN )\nSEMICOLON ;\n", lexer.getTokens());
    lexer.parse("a0.b_ = 5;");
    EXPECT_STREQ("IDENTIFIER a0\nDOT .\nIDENTIFIER b_\nASSIGN =\nINTEGER 5\nSEMICOLON ;\n", lexer.getTokens());
    lexer.parse("a1 = b2.get();");
    EXPECT_STREQ("IDENTIFIER a1\nASSIGN =\nIDENTIFIER b2\nDOT .\nIDENTIFIER get\nLPAREN (\nRPAREN )\nSEMICOLON ;\n", lexer.getTokens());
    lexer.parse("(1 + 2) * (3 - 4)");
    EXPECT_STREQ("LPAREN (\nINTEGER 1\nADD +\nINTEGER 2\nRPAREN )\nMULTI *\nLPAREN (\nINTEGER 3\nSUB -\nINTEGER 4\nRPAREN )\n", lexer.getTokens());
}

TEST(LexerTest, HandleErrorInput) {
    Lexer lexer = Lexer();
    lexer.parse("0.0");
    EXPECT_STREQ("ERROR: Floating Numbers are not supported 0.0\n", lexer.getTokens());
    lexer.parse("&");
    EXPECT_STREQ("ERROR: Unknown character &\n", lexer.getTokens());
    lexer.parse(">");
    EXPECT_STREQ("ERROR: Unknown character >\n", lexer.getTokens());
    lexer.parse("|");
    EXPECT_STREQ("ERROR: Unknown character |\n", lexer.getTokens());
    lexer.parse("/");
    EXPECT_STREQ("ERROR: Unknown character /\n", lexer.getTokens());
    lexer.parse("_abc123");
    EXPECT_STREQ("ERROR: Identifiers can not begin with an underscore _abc123\n", lexer.getTokens());
    lexer.parse("123abc");
    EXPECT_STREQ("ERROR: Identifiers can not begin with a number 123abc\n", lexer.getTokens());
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    return 0;
}
