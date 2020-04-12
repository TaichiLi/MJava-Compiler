// THIS FILE IS PART OF MJava-Compiler PROJECT
// parser.h - parser
 
// Created by Li Taiji 2020-03-28
// Copyright (c) 2020 Li Taiji All rights reserved

#ifndef PARSER_H_
#define PARSER_H_

#include "ast.h"
#include "token.h"
#include "scanner.h"
#include <memory>
#include <vector>

namespace MJava
{
    class Parser
    {
    public:
        explicit                Parser(Scanner& scanner);
                                ~Parser();
        static bool             getErrorFlag();
        static void             setErrorFlag(bool flag);
        ProgramASTPtr           parse();
        std::string             toString();

    private:
        ExprASTPtr              parseExpression();
        ExprASTPtr              parseBinOpRHS(int precedence, ExprASTPtr lhs);
        ExprASTPtr              parseUnaryOp();
        ExprASTPtr              parseIdentifierExpression();
        ExprASTPtr              parseParenExpression();
        ExprASTPtr              parseBlockOrStatement();
        ExprASTPtr              parsePrimary();
        ExprASTPtr              parseReturnStatement();
        ExprASTPtr              parseLengthStatement();
        ExprASTPtr              parseNewStatement();

        // parse different type expressions.
        // they are also very like parseNumber function in the llvm tutorial,
        // but they are more complex
        ExprASTPtr              parseRealExpression();
        ExprASTPtr              parseIntegerExpression();
        ExprASTPtr              parseCharExpression();
        ExprASTPtr              parseStringExpression();
        ExprASTPtr              parseBooleanExpression();

        // note: assignment statement implementation will be included parseStatement.
        ExprASTPtr              parseIfStatement();
        ExprASTPtr              parseWhileStatement();
        ExprASTPtr              parseForStatement();

        // declaration / definition contains procedure and function.
        ExprASTPtr              parseMainClass();
        ExprASTPtr              parseMainMethod();
        ExprASTPtr              parseMainMethodBody();
        ExprASTPtr              parseClassDeclaration();
        VecExprASTPtr           parseClassMemberVariables();
        VecExprASTPtr           parseClassMemberMethods();
        ExprASTPtr              parseMethodDeclaration();
        ExprASTPtr              parseMethodBody();
        ExprASTPtr              parseVariableDeclaration();
        ExprASTPtr              parseVariableDeclaration(const Token& token);
        ExprASTPtr              parseMethodCallStatement(const Token& token);
        ExprASTPtr              parseMethodParameter();

        // I/O routines
        ExprASTPtr              parsePrintStatement();

        // TODO:
        // ........... many types related ............

        // Helper Functions.
    private:
        bool                    expectAST(ASTType type, const std::string& astName, ExprASTPtr ast);
        bool                    expectToken(TokenValue value, const std::string& tokenName, bool advanceToNextToken);
        bool                    expectToken(TokenType type, const std::string& tokenTypeDescription, bool advanceToNextToken);
        bool                    validateAST(ASTType type, ExprASTPtr ast);
        bool                    validateToken(TokenValue value, bool advanceToNextToken);
        bool                    validateToken(TokenType type, bool advanceToNextToken);
        void                    errorReport(const std::string& msg);
        void                    errorReport(ExprASTPtr ast, const std::string& msg);

    private:
        Scanner&                scanner_;
        ProgramASTPtr           program_;
        static bool             errorFlag_;
        std::vector<Token>      stack_;

    };

    inline bool Parser::getErrorFlag()
    {
        return errorFlag_;
    }

} // namespace MJava

#endif // parser.h