// THIS FILE IS PART OF MJava-Compiler PROJECT
// ast.cpp - Abstract syntax tree
 
// Created by Li Taiji 2020-03-28
// Copyright (c) 2020 Li Taiji All rights reserved

#include "ast.h"

namespace MJava
{
    ExprAST::ExprAST(const TokenLocation& loc)
        : loc_(loc)
    {}

    TokenLocation ExprAST::getLocation() const
    {
        return loc_;
    }
    
    std::string ExprAST::toString() const
    {
        return std::string(getLocation().toString());
    }

    BlockAST::BlockAST(const TokenLocation& loc, VecExprASTPtr body)
        : ExprAST(loc), body_(std::move(body))
    {}

    std::string BlockAST::toString() const
    {
        std::string str;
        size_t size = body_.size();
        if (size > 0)
        {
            for (size_t i = 0; i < size - 1; i++)
            {
                str += body_[i]->toString() + ",\n";
            }
            str += body_[size - 1]->toString() + "\n";
        }    
        return str;
    }

    ClassAST::ClassAST(const TokenLocation &loc, const std::string &className, bool isBaseClass, const std::string &baseClassName, ExprASTPtr classBody)
        : ExprAST(loc), className_(className), isBaseClass_(isBaseClass), baseClassName_(baseClassName), classBody_(classBody)
    {}

    ClassAST::ClassAST(const TokenLocation &loc, const std::string &className, bool isBaseClass, ExprASTPtr classBody)
        : ExprAST(loc), className_(className), isBaseClass_(isBaseClass), classBody_(classBody)
    {}

    std::string ClassAST::toString() const
    {
        if (isBaseClass_)
        {
            std::string str = "{\n\"type\": \"ClassDeclaration\",\n\"class name\": \"" + className_ + "\",\n\"base class\": \"" + baseClassName_ + "\",\n\"body\": [" + classBody_->toString() + "]\n}";
            return str;
        }
        else
        {
            std::string str = "{\n\"type\": \"ClassDeclaration\",\n\"class name\": \"" + className_ + "\",\n\"body\": [" + classBody_->toString() + "]\n}";
            return str;
        }   
    }

    VariableDeclarationAST::VariableDeclarationAST(const TokenLocation &loc, const std::string &type, const std::string &name)
        : ExprAST(loc), type_(type), name_(name)
    {}

    std::string VariableDeclarationAST::toString() const
    {
        return std::string("{\n\"type\": \"VarDeclaration\",\n\"variable type\": \"" + type_ + "\",\n\"variable name\": \"" + name_ + "\"\n}");
    }

    VariableAST::VariableAST(const TokenLocation &loc, const std::string &name, ExprASTPtr index)
        : ExprAST(loc), name_(name), index_(index)
    {}

    std::string VariableAST::toString() const
    {
        if (index_ != nullptr)
        {
            return std::string("{\n\"type\": \"Variable\",\n\"name\": \"" + name_ + "\",\n\"index\": " + index_->toString() + "\n}");
        }
        else
        {
            return std::string("{\n\"type\": \"Variable\",\n\"name\": \"" + name_ + "\"\n}");
        }   
    }

    MethodDeclarationAST::MethodDeclarationAST(const TokenLocation &loc, const std::string &returnType, const std::string &name, VecExprASTPtr parameters, ExprASTPtr body)
        : ExprAST(loc), returnType_(returnType), name_(name), parameters_(std::move(parameters)), body_(body)
    {}
    
    std::string MethodDeclarationAST::toString() const
    {
        std::string str;
        size_t size = parameters_.size();
        if (size > 0)
        {
            for (size_t i = 0; i < size - 1; i++)
            {
                str += parameters_[i]->toString() + ",\n";
            }
            str += parameters_[size - 1]->toString() + "\n";
        }
        return std::string("{\n\"type\": \"MethodDeclaration\",\n\"return type\": \"" + returnType_ + "\",\n\"method name\": \"" + name_ + "\",\n\"parameters\": [" + str + "],\n\"body\": [" + body_->toString() + "]\n}");
    }

    MethodCallAST::MethodCallAST(const TokenLocation &loc, const std::string &name, VecExprASTPtr parameters)
        : ExprAST(loc), name_(name), parameters_(std::move(parameters))
    {}

    std::string MethodCallAST::toString() const
    {
        std::string str;
        size_t size = parameters_.size();
        if (size > 0)
        {
            for (size_t i = 0; i < size - 1; i++)
            {
                str += parameters_[i]->toString() + ",\n";
            }
            str += parameters_[size - 1]->toString() + "\n";
        }
        return std::string("{\n\"type\": \"MethodCall\",\n\"method name\": \"" + name_ + "\",\n\"parameters\": [" + str + "]\n}");
    }

    IfStatementAST::IfStatementAST(const TokenLocation& loc, ExprASTPtr condition, ExprASTPtr thenPart, ExprASTPtr elsePart)
        : ExprAST(loc), condition_(condition), thenPart_(thenPart), elsePart_(elsePart)
    {}
    
    std::string IfStatementAST::toString() const
    {
        return std::string("{\n\"type\": \"IfStatement\",\n\"condition\": " + condition_->toString() + ",\n\"then part\": [" + thenPart_->toString() + "],\n\"else part\": ["+ elsePart_->toString() + "]\n}");
    }

    WhileStatementAST::WhileStatementAST(const TokenLocation& loc, ExprASTPtr condition, ExprASTPtr body)
        : ExprAST(loc), condition_(condition), body_(body)
    {}

    std::string WhileStatementAST::toString() const
    {
        return std::string("{\n\"type\": \"WhileStatement\",\n\"condition\": " + condition_->toString() + ",\n\"body\": [" + body_->toString() + "]\n}");
    }

    ReturnStatementAST::ReturnStatementAST(const TokenLocation& loc, ExprASTPtr returnStatement)
        : ExprAST(loc), returnStatement_(returnStatement)
    {}

    std::string ReturnStatementAST::toString() const
    {
        return std::string("{\n\"type\": \"ReturnStatement\",\n\"expression\" :" + returnStatement_->toString() + "\n}");
    }

    BinaryOpExpressionAST::BinaryOpExpressionAST(const TokenLocation& loc, const std::string &binaryOp, ExprASTPtr lhs, ExprASTPtr rhs)
        : ExprAST(loc), binaryOp_(binaryOp), lhs_(lhs), rhs_(rhs)
    {}

    std::string BinaryOpExpressionAST::toString() const
    {
        return std::string("{\n\"type\": \"BinaryOpExpression\",\n\"binary operator\": \"" + binaryOp_ + "\",\n\"lhs\": " + lhs_->toString() + ",\n\"rhs\": " + rhs_->toString() + "\n}");
    }

    UnaryOpExpressionAST::UnaryOpExpressionAST(const TokenLocation& loc, const std::string &unaryOp, ExprASTPtr expression)
        : ExprAST(loc), unaryOp_(unaryOp), expression_(expression)
    {}

    std::string UnaryOpExpressionAST::toString() const
    {
        return std::string("{\n\"type\": \"UnaryOpExpression\",\n\"unary operator\": \"" + unaryOp_ + "\",\n\"expression\": " + expression_->toString() + "\n}");
    }

    BooleanAST::BooleanAST(const TokenLocation& loc, bool boolean)
        : ExprAST(loc), boolean_(boolean)
    {}

    std::string BooleanAST::toString() const
    {
        if (boolean_)
        {
            return std::string("{\n\"type\": \"BooleanLiteral\",\n\"boolean\": true\n}");
        }
        else
        {
            return std::string("{\n\"type\": \"BooleanLiteral\",\n\"boolean\": false\n}");
        }
           
    }

    IntegerAST::IntegerAST(const TokenLocation& loc, int integer)
        : ExprAST(loc), integer_(integer)
    {}

    std::string IntegerAST::toString() const
    {
        return std::string("{\n\"type\": \"IntegerLiteral\",\n\"integer\": " + std::to_string(integer_) + "\n}");
    }

    PrintStatementAST::PrintStatementAST(const TokenLocation& loc, ExprASTPtr printStatement)
        : ExprAST(loc), printStatement_(printStatement)
    {}

    std::string PrintStatementAST::toString() const
    {
        return std::string("{\n\"type\": \"PrintStatement\",\n\"expression\": " + printStatement_->toString() + "\n}");
    }

    NewStatementAST::NewStatementAST(const TokenLocation& loc, const std::string &type, ExprASTPtr length)
        : ExprAST(loc), type_(type), length_(length)
    {}

    std::string NewStatementAST::toString() const
    {
        if (length_ !=nullptr)
        {
            return std::string("{\n\"type\": \"NewStatement\",\n\"variable type\": \"" + type_ + "\",\n\"length\": " + length_->toString() + "\n}");
        }
        else
        {
            return std::string("{\n\"type\": \"NewStatement\",\n\"variable type\": \"" + type_ + "\"\n}");
        }   
    }
}