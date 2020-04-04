// THIS FILE IS PART OF MJava-Compiler PROJECT
// ast.cpp - Abstract syntax tree
 
// Created by Li Taiji 2020-03-28
// Copyright (c) 2020 Li Taiji All rights reserved

#include "ast.h"
#include <sstream>
#include <string>

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
        return getLocation().toString();
    }

    BlockAST::BlockAST(const TokenLocation& loc, VecExprASTPtr&& body)
        : ExprAST(loc), body_(std::move(body))
    {}

    std::string BlockAST::toString() const
    {
        std::ostringstream str;

        size_t size = body_.size();

        if (size > 0)
        {
            for (size_t i = 0; i < size - 1; i++)
            {
                str << body_[i]->toString() << ",\n";
            }
            str << body_[size - 1]->toString() << "\n";
        }

        return str.str();
    }

    ClassDeclarationAST::ClassDeclarationAST(const TokenLocation& loc, const std::string& className, const std::string& baseClassName, ExprASTPtr classBody)
        : ExprAST(loc), className_(className), baseClassName_(baseClassName), classBody_(std::move(classBody))
    {}

    std::string ClassDeclarationAST::toString() const
    {
        return std::string("{\n\"type\": \"ClassDeclaration\",\n\"class name\": \"" + className_ + "\",\n\"base class\": \"" + baseClassName_ + "\",\n\"body\": [" + classBody_->toString() + "]\n}");  
    }

    VariableDeclarationAST::VariableDeclarationAST(const TokenLocation& loc, const std::string& type, const std::string& name)
        : ExprAST(loc), type_(type), name_(name)
    {}

    std::string VariableDeclarationAST::toString() const
    {
        return std::string("{\n\"type\": \"VarDeclaration\",\n\"variable type\": \"" + type_ + "\",\n\"variable name\": \"" + name_ + "\"\n}");
    }

    VariableAST::VariableAST(const TokenLocation& loc, const std::string& name, ExprASTPtr index)
        : ExprAST(loc), name_(name), index_(std::move(index))
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

    MethodDeclarationAST::MethodDeclarationAST(const TokenLocation& loc, const std::vector<std::string>& attributes, const std::string& returnType, const std::string& name, VecExprASTPtr&& parameters, ExprASTPtr body)
        : ExprAST(loc), attributes_(attributes), returnType_(returnType), name_(name), parameters_(std::move(parameters)), body_(std::move(body))
    {}
    
    std::string MethodDeclarationAST::toString() const
    {
        std::ostringstream str;
        str << "{\n\"type\": \"MethodDeclaration\",\n\"attributes\": [";

        size_t size = attributes_.size();

        if (size > 0)
        {
            for (size_t i = 0; i < size - 1; i++)
            {
                str << "\"" << attributes_[i] << "\",\n";
            }
            str << "\"" << attributes_[size - 1] << "\"\n";
        }

        str << "],\n\"return type\": \"" << returnType_ << "\",\n\"method name\": \"" << name_ << "\",\n\"parameters\": [";

        size = parameters_.size();

        if (size > 0)
        {
            for (size_t i = 0; i < size - 1; i++)
            {
                str << parameters_[i]->toString() << ",\n";
            }
            str << parameters_[size - 1]->toString() << "\n";
        }

        str << "],\n\"body\": [" << body_->toString() << "]\n}";

        return str.str();
    }

    MethodCallAST::MethodCallAST(const TokenLocation& loc, const std::string& name, VecExprASTPtr&& parameters)
        : ExprAST(loc), name_(name), parameters_(std::move(parameters))
    {}

    std::string MethodCallAST::toString() const
    {
        std::ostringstream str;
        str << "{\n\"type\": \"MethodCall\",\n\"method name\": \"" << name_ << "\",\n\"parameters\": [";

        size_t size = parameters_.size();

        if (size > 0)
        {
            for (size_t i = 0; i < size - 1; i++)
            {
                str << parameters_[i]->toString() << ",\n";
            }
            str << parameters_[size - 1]->toString() << "\n";
        }

        str << "]\n}";

        return str.str();
    }

    IfStatementAST::IfStatementAST(const TokenLocation& loc, ExprASTPtr condition, ExprASTPtr thenPart, ExprASTPtr elsePart)
        : ExprAST(loc), condition_(std::move(condition)), thenPart_(std::move(thenPart)), elsePart_(std::move(elsePart))
    {}
    
    std::string IfStatementAST::toString() const
    {
        return std::string("{\n\"type\": \"IfStatement\",\n\"condition\": " + condition_->toString() + ",\n\"then part\": [" + thenPart_->toString() + "],\n\"else part\": ["+ (elsePart_ != nullptr ? elsePart_->toString() : "") + "]\n}");
    }

    WhileStatementAST::WhileStatementAST(const TokenLocation& loc, ExprASTPtr condition, ExprASTPtr body)
        : ExprAST(loc), condition_(std::move(condition)), body_(std::move(body))
    {}

    std::string WhileStatementAST::toString() const
    {
        return std::string("{\n\"type\": \"WhileStatement\",\n\"condition\": " + condition_->toString() + ",\n\"body\": [" + body_->toString() + "]\n}");
    }

    ForStatementAST::ForStatementAST(const TokenLocation& loc, ExprASTPtr variable, ExprASTPtr condition, ExprASTPtr action, ExprASTPtr body)
        : ExprAST(loc), variable_(std::move(variable)), condition_(std::move(condition)), action_(std::move(action)), body_(std::move(body))
    {}

    std::string ForStatementAST::toString() const
    {
        return std::string("{\n\"type\": \"ForStatement\",\n\"variable\": " + (variable_ != nullptr ? variable_->toString() : "{}") + ",\n\"condition\": " + (condition_ != nullptr ? condition_->toString() : "{}") + ",\n\"action\": " + (action_ != nullptr ? action_->toString() : "{}") + ",\n\"body\": [" + body_->toString() + "]\n}");
    }

    ReturnStatementAST::ReturnStatementAST(const TokenLocation& loc, ExprASTPtr returnStatement)
        : ExprAST(loc), returnStatement_(std::move(returnStatement))
    {}

    std::string ReturnStatementAST::toString() const
    {
        return std::string("{\n\"type\": \"ReturnStatement\",\n\"expression\" :" + (returnStatement_ != nullptr ? returnStatement_->toString() : "{}") + "\n}");
    }

    BinaryOpExpressionAST::BinaryOpExpressionAST(const TokenLocation& loc, const std::string& binaryOp, ExprASTPtr lhs, ExprASTPtr rhs)
        : ExprAST(loc), binaryOp_(binaryOp), lhs_(std::move(lhs)), rhs_(std::move(rhs))
    {}

    std::string BinaryOpExpressionAST::toString() const
    {
        return std::string("{\n\"type\": \"BinaryOpExpression\",\n\"binary operator\": \"" + binaryOp_ + "\",\n\"lhs\": " + lhs_->toString() + ",\n\"rhs\": " + rhs_->toString() + "\n}");
    }

    UnaryOpExpressionAST::UnaryOpExpressionAST(const TokenLocation& loc, const std::string& unaryOp, ExprASTPtr expression)
        : ExprAST(loc), unaryOp_(unaryOp), expression_(std::move(expression))
    {}

    std::string UnaryOpExpressionAST::toString() const
    {
        return std::string("{\n\"type\": \"UnaryOpExpression\",\n\"unary operator\": \"" + unaryOp_ + "\",\n\"expression\": " + expression_->toString() + "\n}");
    }
    
    PrintStatementAST::PrintStatementAST(const TokenLocation& loc, ExprASTPtr printStatement)
        : ExprAST(loc), printStatement_(std::move(printStatement))
    {}

    std::string PrintStatementAST::toString() const
    {
        return std::string("{\n\"type\": \"PrintStatement\",\n\"expression\": " + printStatement_->toString() + "\n}");
    }

    NewStatementAST::NewStatementAST(const TokenLocation& loc, const std::string& type, ExprASTPtr length)
        : ExprAST(loc), type_(type), length_(std::move(length))
    {}

    std::string NewStatementAST::toString() const
    {
        if (length_ != nullptr)
        {
            return std::string("{\n\"type\": \"NewStatement\",\n\"variable type\": \"" + type_ + "\",\n\"length\": " + length_->toString() + "\n}");
        }
        else
        {
            return std::string("{\n\"type\": \"NewStatement\",\n\"variable type\": \"" + type_ + "\"\n}");
        }
    }

    RealAST::RealAST(const TokenLocation& loc, double real)
        : ExprAST(loc), real_(real)
    {}
    
    std::string RealAST::toString() const
    {
        return std::string("{\n\"type\": \"RealLiteral\",\n\"real\": " + std::to_string(real_) + "\n}");
    }

    IntegerAST::IntegerAST(const TokenLocation& loc, int integer)
        : ExprAST(loc), integer_(integer)
    {}

    std::string IntegerAST::toString() const
    {
        return std::string("{\n\"type\": \"IntegerLiteral\",\n\"integer\": " + std::to_string(integer_) + "\n}");
    }
    
    CharAST::CharAST(const TokenLocation& loc, char ch)
        : ExprAST(loc), ch_(ch)
    {}

    std::string CharAST::toString() const
    {
        return std::string("{\n\"type\": \"CharLiteral\",\n\"char\": \"" + std::string(1, ch_) + "\"\n}");
    }

    StringAST::StringAST(const TokenLocation& loc, const std::string& str)
        : ExprAST(loc), str_(str)
    {}

    std::string StringAST::toString() const
    {
        return std::string("{\n\"type\": \"StringLiteral\",\n\"string\": \"" + str_ + "\"\n}");
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

    
} // namespace MJava