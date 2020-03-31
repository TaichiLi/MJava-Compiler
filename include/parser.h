// THIS FILE IS PART OF MJava-Compiler PROJECT
// parser.h - parser
 
// Created by Li Taiji 2020-03-28
// Copyright (c) 2020 Li Taiji All rights reserved

#ifndef PARSER_H_
#define PARSER_H_

#include <vector>
#include <memory>
#include "scanner.h"
#include "ast.h"
#include "token.h"

namespace MJava
{
    // others XXXPtr* such as ExprASTPtr) are located in the ast.h
    // using ConstantDeclPtr = std::unique_ptr <Constant>;

    class Parser
    {
    public:
        explicit                Parser(Scanner& scanner);
        static bool             getErrorFlag();
        static void             setErrorFlag(bool flag);
        VecExprASTPtr&          parse();
        std::string             toString();

    private:
        // parseExpression, parseBinOpRHS and parseUnaryOp,
        // parseIdentifierExpression, parseParenExpression functions are just like 
        // llvm kaleidoscope tutorial 02
        // see the link: http://llvm.org/docs/tutorial/LangImpl2.html
        ExprASTPtr              parseExpression();
        ExprASTPtr              parseBinOpRHS(int precedence, ExprASTPtr lhs);
        ExprASTPtr              parseUnaryOp();
        ExprASTPtr              parseIdentifierExpression();
        ExprASTPtr              parseParenExpression();
        ExprASTPtr              parseClass();
        // BlockASTPtr             parseClassBody();
        ExprASTPtr              parseBlockOrStatement();
        ExprASTPtr              parsePrimary();
        ExprASTPtr              parseReturnStatement();
        ExprASTPtr              parseLengthStatement();
        ExprASTPtr              parseNewStatement();

        // parse different type expressions.
        // they are also very like parseNumber function in the llvm tutorial,
        // but they are more complex
        ExprASTPtr              parseRealExpression(Token token);
        ExprASTPtr              parseIntegerExpression();
        ExprASTPtr              parseCharExpression(Token token);
        ExprASTPtr              parseStringExpression(Token token);
        ExprASTPtr              parseBooleanExpression();

        // note: assignment statement implementation will be included parseStatement.
        ExprASTPtr              parseIfStatement();
        ExprASTPtr              parseWhileStatement();

        // declaration / definition contains procedure and function.
        ExprASTPtr              parseMethodDeclaration();
        ExprASTPtr              parseVariableDeclaration();
        ExprASTPtr              parseVariableDeclaration(Token token);
        ExprASTPtr              parseMethodCallStatement(Token token);
        ExprASTPtr              parseMethodParameter();

        // I/O routines
        ExprASTPtr              parsePrintStatement();

        // Type
        Token                   parseToken(const Token& token);
        // TODO:
        // ........... many types related ............

        // Helper Functions.
    private:
        bool                    expectToken(TokenValue value, const std::string& tokenName, bool advanceToNextToken);
        bool                    expectToken(TokenType type, const std::string& tokenTypeDescription, bool advanceToNextToken);
        bool                    validateToken(TokenValue value, bool advanceToNextToken);
        bool                    validateToken(TokenType type, bool advanceToNextToken);
        void                    errorReport(const std::string& msg);
    private:
        Scanner&                scanner_;
        VecExprASTPtr           ast_;
        static bool             errorFlag_;
        std::vector<Token>      stack_;

    };

    inline bool Parser::getErrorFlag()
    {
        return errorFlag_;
    }

}

#endif // parser.h