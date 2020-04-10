// THIS FILE IS PART OF MJava-Compiler PROJECT
// ast.h - abstract syntax tree
 
// Created by Li Taiji 2020-03-28
// Copyright (c) 2020 Li Taiji All rights reserved

#ifndef AST_H_
#define AST_H_

#include "token.h"
#include <memory>
#include <string>
#include <vector>

namespace MJava
{
    class ExprAST;

    using VecExprASTPtr = std::vector<ExprAST*>;
    using ExprASTPtr = ExprAST*;

    class ExprAST
    {
    public:
        ExprAST(const TokenLocation& loc);
        virtual ~ExprAST() = default;
        virtual std::string toString() const;

    private:
        TokenLocation loc_;
    };

    class BlockAST : public ExprAST
    {
    public:
        BlockAST(const TokenLocation& loc, const VecExprASTPtr& block);
        ~BlockAST();
        VecExprASTPtr getBlock() const { return block_; }
        std::string toString() const override;

    private:
        VecExprASTPtr       block_;
    };

    class ClassDeclarationAST : public ExprAST
    {
    public:
        ClassDeclarationAST(const TokenLocation& loc, const std::string& className, const std::string& baseClassName, const VecExprASTPtr& memberVariables, const VecExprASTPtr& memberMethods);
        ~ClassDeclarationAST();
        std::string getClassName() const { return className_; }
        std::string getBaseClassName() const { return baseClassName_; }
        VecExprASTPtr getMemberVariables() const { return memberVariables_; }
        VecExprASTPtr getMemberMemthods() const { return memberMethods_; }
        std::string toString() const override;

    private:
        std::string         className_;
        std::string         baseClassName_;
        VecExprASTPtr       memberVariables_;
        VecExprASTPtr       memberMethods_;
    };

    class MethodBodyAST : public ExprAST
    {
    public:
        MethodBodyAST(const TokenLocation& loc, const VecExprASTPtr& localVariables, const VecExprASTPtr& methodBody);
        ~MethodBodyAST();
        VecExprASTPtr getLocalVariables() const { return localVariables_; }
        VecExprASTPtr getMethodBody() const { return methodBody_; }
        std::string toString() const override;

    private:
        VecExprASTPtr       localVariables_;
        VecExprASTPtr       methodBody_;
    };

    class MethodDeclarationAST : public ExprAST
    {
    public:
        MethodDeclarationAST(const TokenLocation& loc, const std::vector<std::string>& attributes, const std::string& returnType, const std::string& name, const VecExprASTPtr& parameters, ExprASTPtr body);
        ~MethodDeclarationAST();
        std::vector<std::string> getAttributes() const { return attributes_; }
        std::string getReturnType() const { return returnType_; }
        std::string getMethodName() const { return name_; }
        VecExprASTPtr getParameters() const { return parameters_; }
        ExprASTPtr getBody() const { return body_; }
        std::string toString() const override;

    private:
        std::vector<std::string> attributes_;
        std::string         returnType_;
        std::string         name_;
        VecExprASTPtr       parameters_;
        ExprASTPtr          body_;
    };

    class MethodCallAST : public ExprAST
    {
    public:
        MethodCallAST(const TokenLocation& loc, const std::string& name, const VecExprASTPtr& parameters);
        ~MethodCallAST();
        std::string getName() const { return name_; }
        VecExprASTPtr getParameters() const { return parameters_; }
        std::string toString() const override;

    private:
        std::string         name_;
        VecExprASTPtr       parameters_;
    };

    class VariableDeclarationAST : public ExprAST
    {
    public:
        VariableDeclarationAST(const TokenLocation& loc, const std::string& type, const std::string& name);
        ~VariableDeclarationAST() = default;
        std::string getType() const { return type_; }
        std::string getName() const { return name_; }
        std::string toString() const override;

    private:
        std::string         type_;
        std::string         name_;
    };

    class VariableAST : public ExprAST
    {
    public:
        VariableAST(const TokenLocation& loc, const std::string& name);
        ~VariableAST() = default;
        std::string getName() const { return name_; }
        std::string toString() const override;

    private:
        std::string         name_;
    };

    class ArrayAST : public ExprAST
    {
    public:
        ArrayAST(const TokenLocation &loc, const std::string &name, ExprASTPtr index);
        ~ArrayAST();
        std::string getName() const { return name_; }
        ExprASTPtr getIndex() const { return index_; }
        std::string toString() const override;
    private:
        std::string         name_;
        ExprASTPtr          index_;
    };

    class IfStatementAST : public ExprAST
    {
    public:
        IfStatementAST(const TokenLocation& loc, ExprASTPtr condition, ExprASTPtr thenPart, ExprASTPtr elsePart);
        ~IfStatementAST();
        std::string toString() const override;

    private:
        ExprASTPtr          condition_;
        ExprASTPtr          thenPart_;
        ExprASTPtr          elsePart_;
    };

    class WhileStatementAST : public ExprAST
    {
    public:
        WhileStatementAST(const TokenLocation& loc, ExprASTPtr condition, ExprASTPtr body);
        ~WhileStatementAST();
        std::string toString() const override;

    private:
        ExprASTPtr          condition_;
        ExprASTPtr          body_;
    };

    class ForStatementAST : public ExprAST
    {
    public:
        ForStatementAST(const TokenLocation& loc, ExprASTPtr variable, ExprASTPtr condition, ExprASTPtr action, ExprASTPtr body);
        ~ForStatementAST();
        std::string toString() const override;

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
        ~ReturnStatementAST();
        std::string toString() const override;

    private:
        ExprASTPtr          returnStatement_;
    };

    class PrintStatementAST : public ExprAST
    {
    public:
        PrintStatementAST(const TokenLocation& loc, ExprASTPtr printStatement);
        ~PrintStatementAST();
        std::string toString() const override;
        
    private:
        ExprASTPtr          printStatement_;
    };
    
    class NewStatementAST : public ExprAST
    {
    public:
        NewStatementAST(const TokenLocation& loc, const std::string& type, ExprASTPtr newStatement);
        ~NewStatementAST();
        std::string toString() const override;
        
    private:
        std::string         type_;
        ExprASTPtr          newStatement_;
    };

    class BinaryOpExpressionAST : public ExprAST
    {
    public:
        BinaryOpExpressionAST(const TokenLocation& loc, const std::string& binaryOp, ExprASTPtr lhs, ExprASTPtr rhs);
        ~BinaryOpExpressionAST();
        std::string getBinaryOp() const { return binaryOp_; }
        ExprASTPtr getLhs() const { return lhs_; }
        ExprASTPtr getRhs() const { return rhs_; }
        std::string toString() const override;

    private:
        std::string         binaryOp_;
        ExprASTPtr          lhs_;
        ExprASTPtr          rhs_;
    };

    class UnaryOpExpressionAST : public ExprAST
    {
    public:
        UnaryOpExpressionAST(const TokenLocation& loc, const std::string& unaryOp, ExprASTPtr expression);
        ~UnaryOpExpressionAST();
        std::string getUnaryOp() const { return unaryOp_; }
        ExprASTPtr getExpression() const { return expression_; }
        std::string toString() const override;

    private:
        std::string         unaryOp_;
        ExprASTPtr          expression_;
    };

    class RealAST : public ExprAST
    {
    public:
        RealAST(const TokenLocation& loc, double real);
        ~RealAST() = default;
        double getReal() const { return real_; }
        std::string toString() const override;

    private:
        double              real_;
    };
    class IntegerAST : public ExprAST
    {
    public:
        IntegerAST(const TokenLocation& loc, int integer);
        ~IntegerAST() = default;
        int getInteger() const { return integer_; }
        std::string toString() const override;

    private:
        int                 integer_;
    };

    class CharAST : public ExprAST
    {
    public:
        CharAST(const TokenLocation& loc, char ch);
        ~CharAST() = default;
        char getChar() const { return ch_; }
        std::string toString() const override;

    private:
        char                ch_;
    };

    class StringAST : public ExprAST
    {
    public:
        StringAST(const TokenLocation& loc, const std::string& str);
        ~StringAST() = default;
        std::string getString() const { return str_; }
        std::string toString() const override;

    private:
        std::string         str_;
    };

    class BooleanAST : public ExprAST
    {
    public:
        BooleanAST(const TokenLocation& loc, bool boolean);
        ~BooleanAST() = default;
        bool getBoolean() const { return boolean_; }
        std::string toString() const override;

    private:
        bool                boolean_;
    };
} // namespace MJava

#endif // ast.h