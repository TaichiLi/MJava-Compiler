// THIS FILE IS PART OF MJava-Compiler PROJECT
// ast.h - abstract syntax tree
 
// Created by Li Taiji 2020-03-28
// Copyright (c) 2020 Li Taiji All rights reserved

#ifndef AST_H_
#define AST_H_

#include <string>
#include <vector>
#include <memory>
#include "token.h"

namespace MJava
{
    class ExprAST;
    class ClassAST;

    using VecExprASTPtr = std::vector <ExprAST*>;
    using ExprASTPtr = ExprAST*;

    class ExprAST
    {
    public:
        ExprAST(const TokenLocation& loc);
        virtual       ~ExprAST() = default;
        TokenLocation getLocation() const;
        virtual std::string toString() const;

    private:
        TokenLocation loc_;
    };

    class BlockAST : public ExprAST
    {
    public:
        BlockAST(const TokenLocation& loc, const VecExprASTPtr& body);
        std::string toString() const;

    private:
        VecExprASTPtr body_;
    };

    class ClassAST : public ExprAST
    {
    public:
        ClassAST(const TokenLocation& loc, const std::string& className, bool isBaseClass, const std::string& baseClassName, ExprASTPtr classBody);
        ClassAST(const TokenLocation& loc, const std::string& className, bool isBaseClass, ExprASTPtr classBody);
        std::string toString() const;

    private:
        std::string   className_;
        bool          isBaseClass_;
        std::string   baseClassName_;
        ExprASTPtr    classBody_;
    };

    class MethodDeclarationAST : public ExprAST
    {
    public:
        MethodDeclarationAST(const TokenLocation& loc, const std::string& returnType, const std::string& name, const VecExprASTPtr& parameters, ExprASTPtr body);
        std::string toString() const;

    private:
        std::string         returnType_;
        std::string         name_;
        VecExprASTPtr       parameters_;
        ExprASTPtr          body_;
    };

    class MethodCallAST : public ExprAST
    {
    public:
        MethodCallAST(const TokenLocation& loc, const std::string& name, const VecExprASTPtr& parameters);
        std::string toString() const;

    private:
        std::string         name_;
        VecExprASTPtr       parameters_;
    };

    class VariableDeclarationAST : public ExprAST
    {
    public:
        VariableDeclarationAST(const TokenLocation& loc, const std::string& type, const std::string& name);
        std::string toString() const;

    private:
        std::string         type_;
        std::string         name_;
    };

    class VariableAST : public ExprAST
    {
    public:
        VariableAST(const TokenLocation& loc, const std::string& name, ExprASTPtr index);
        std::string toString() const;

    private:
        std::string         name_;
        ExprASTPtr          index_;
    };

    class IfStatementAST : public ExprAST
    {
    public:
        IfStatementAST(const TokenLocation& loc, ExprASTPtr condition, ExprASTPtr thenPart, ExprASTPtr elsePart);
        std::string toString() const;

    private:
        ExprASTPtr          condition_;
        ExprASTPtr          thenPart_;
        ExprASTPtr          elsePart_;
    };

    class WhileStatementAST : public ExprAST
    {
    public:
        WhileStatementAST(const TokenLocation& loc, ExprASTPtr condition, ExprASTPtr body);
        std::string toString() const;

    private:
        ExprASTPtr          condition_;
        ExprASTPtr          body_;
    };

    class ForStatementAST : public ExprAST
    {
    public:
        ForStatementAST(const TokenLocation& loc, ExprASTPtr variable, ExprASTPtr condition, ExprASTPtr action, ExprASTPtr body);
        std::string toString() const;

    private:
        ExprASTPtr          variable_;
        ExprASTPtr          condition_;
        ExprASTPtr          action_;
        ExprASTPtr          body_;
    };
    
    class ReturnStatementAST : public ExprAST
    {
    public:
        ReturnStatementAST(const TokenLocation& loc, ExprASTPtr returnStatement);
        std::string toString() const;

    private:
        ExprASTPtr          returnStatement_;
    };

    class BinaryOpExpressionAST : public ExprAST
    {
    public:
        BinaryOpExpressionAST(const TokenLocation& loc, const std::string& binaryOp, ExprASTPtr lhs, ExprASTPtr rhs);
        std::string toString() const;

    private:
        std::string         binaryOp_;
        ExprASTPtr          lhs_;
        ExprASTPtr          rhs_;
    };

    class UnaryOpExpressionAST : public ExprAST
    {
    public:
        UnaryOpExpressionAST(const TokenLocation& loc, const std::string& unaryOp, ExprASTPtr expression);
        std::string toString() const;

    private:
        std::string         unaryOp_;
        ExprASTPtr          expression_;
    };

    class BooleanAST : public ExprAST
    {
    public:
        BooleanAST(const TokenLocation& loc, bool boolean);
        std::string toString() const;

    private:
        bool                boolean_;
    };

    class IntegerAST : public ExprAST
    {
    public:
        IntegerAST(const TokenLocation& loc, int integer);
        std::string toString() const;

    private:
        int                 integer_;
    };

    class PrintStatementAST : public ExprAST
    {
    public:
        PrintStatementAST(const TokenLocation& loc, ExprASTPtr printStatement);
        std::string toString() const;
        
    private:
        ExprASTPtr          printStatement_;
    };
    
    class NewStatementAST : public ExprAST
    {
    public:
        NewStatementAST(const TokenLocation& loc, const std::string& type, ExprASTPtr length);
        std::string toString() const;
        
    private:
        std::string         type_;
        ExprASTPtr          length_;
    };
} // namespace MJava

#endif // ast.h