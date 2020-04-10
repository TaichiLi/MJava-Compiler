// THIS FILE IS PART OF MJava-Compiler PROJECT
// ast.cpp - Abstract syntax tree
 
// Created by Li Taiji 2020-03-28
// Copyright (c) 2020 Li Taiji All rights reserved

#include "ast.h"
#include <sstream>
#include <string>
#include <typeinfo.h>

namespace MJava
{
    ExprAST::ExprAST(const TokenLocation& loc)
        : loc_(loc)
    {}
    
    std::string ExprAST::toString() const
    {
        return loc_.toString();
    }

    BlockAST::BlockAST(const TokenLocation& loc, const VecExprASTPtr& block)
        : ExprAST(loc), block_(block)
    {}

    BlockAST::~BlockAST()
    {
        for (auto exprASTPTr : block_)
        {
            if (exprASTPTr != nullptr)
            {
                delete exprASTPTr;
            }
        }
    }

    std::string BlockAST::toString() const
    {
        std::ostringstream str;

        size_t size = block_.size();

        if (size > 0)
        {
            for (size_t i = 0; i < size - 1; i++)
            {
                str << block_[i]->toString() << ",\n";
            }
            str << block_[size - 1]->toString() << "\n";
        }

        return str.str();
    }

    ClassDeclarationAST::ClassDeclarationAST(const TokenLocation& loc, const std::string& className, const std::string& baseClassName, const VecExprASTPtr& memberVariables, const VecExprASTPtr& memberMethods)
        : ExprAST(loc), className_(className), baseClassName_(baseClassName), memberVariables_(memberVariables), memberMethods_(memberMethods)
    {}

    ClassDeclarationAST::~ClassDeclarationAST()
    {
        for (auto variable : memberVariables_)
        {
            if (variable != nullptr)
            {
                delete variable;
            }
        }

        for (auto method : memberMethods_)
        {
            if (method != nullptr)
            {
                delete method;
            }
        }
    }

    std::string ClassDeclarationAST::toString() const
    {
        std::ostringstream str;
        str << "{\n\"type\": \"ClassDeclaration\",\n\"class name\": \"" << className_ << "\",\n\"base class\": \"" << baseClassName_ << "\",\n\"member variables\": [";

        size_t size = memberVariables_.size();

        if (size > 0)
        {
            for (size_t i = 0; i < size - 1; i++)
            {
                str << memberVariables_[i]->toString() << ",\n";
            }

            str << memberVariables_[size - 1]->toString() << "\n";
        }

        str << "],\n\"member methods\": [";
        size = memberMethods_.size();

        if (size > 0)
        {
            for (size_t i = 0; i < size - 1; i++)
            {
                str << memberMethods_[i]->toString() << ",\n";
            }

            str << memberMethods_[size - 1]->toString() << "\n";
        }

        str << "]\n}";

        return str.str();  
    }

    MethodBodyAST::MethodBodyAST(const TokenLocation& loc, const VecExprASTPtr& localVariables, const VecExprASTPtr& methodBody)
        : ExprAST(loc), localVariables_(localVariables), methodBody_(methodBody)
    {}

    MethodBodyAST::~MethodBodyAST()
    {
        for (auto variable : localVariables_)
        {
            if (variable != nullptr)
            {
                delete variable;
            }
        }

        for (auto exprASTPTr : methodBody_)
        {
            if (exprASTPTr != nullptr)
            {
                delete exprASTPTr;
            }
        }
    }

    std::string MethodBodyAST::toString() const
    {
        std::ostringstream str;
        str << "\"local variables\": [";

        size_t size = localVariables_.size();

        if (size > 0)
        {
            for (size_t i = 0; i < size - 1; i++)
            {
                str << localVariables_[i]->toString() << ",\n";
            }
            str << localVariables_[size - 1]->toString() << "\n";
        }

        str << "],\n\"method body\": [";

        size = methodBody_.size();

        if (size > 0)
        {
            for (size_t i = 0; i < size - 1; i++)
            {
                str << methodBody_[i]->toString() << ",\n";
            }
            str << methodBody_[size - 1]->toString() << "\n";
        }

        str << "]";
        
        return str.str();
    }

    MethodDeclarationAST::MethodDeclarationAST(const TokenLocation& loc, const std::vector<std::string>& attributes, const std::string& returnType, const std::string& name, const VecExprASTPtr& parameters, ExprASTPtr body)
        : ExprAST(loc), attributes_(attributes), returnType_(returnType), name_(name), parameters_(parameters), body_(body)
    {}

    MethodDeclarationAST::~MethodDeclarationAST()
    {
        for (auto exprASTPTr : parameters_)
        {
            if (exprASTPTr != nullptr)
            {
                delete exprASTPTr;
            }
        }

        if (body_ != nullptr)
        {
            delete body_;
        }
    }
    
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

        str << "],\n\"body\": {" << body_->toString() << "}\n}";

        return str.str();
    }

    MethodCallAST::MethodCallAST(const TokenLocation& loc, const std::string& name, const VecExprASTPtr& parameters)
        : ExprAST(loc), name_(name), parameters_(parameters)
    {}

    MethodCallAST::~MethodCallAST()
    {
        for (auto exprASTPTr : parameters_)
        {
            if (exprASTPTr != nullptr)
            {
                delete exprASTPTr;
            }
        }
    }

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

    VariableDeclarationAST::VariableDeclarationAST(const TokenLocation& loc, const std::string& type, const std::string& name)
        : ExprAST(loc), type_(type), name_(name)
    {}

    std::string VariableDeclarationAST::toString() const
    {
        return std::string("{\n\"type\": \"VarDeclaration\",\n\"variable type\": \"" + type_ + "\",\n\"variable name\": \"" + name_ + "\"\n}");
    }

    VariableAST::VariableAST(const TokenLocation& loc, const std::string& name)
        : ExprAST(loc), name_(name)
    {}

    std::string VariableAST::toString() const
    {
        return std::string("{\n\"type\": \"Variable\",\n\"name\": \"" + name_ + "\"\n}");
    }

    ArrayAST::ArrayAST(const TokenLocation& loc, const std::string& name, ExprASTPtr index)
        : ExprAST(loc), name_(name), index_(index)
    {}

    ArrayAST::~ArrayAST()
    {
        if (index_ != nullptr)
        {
            delete index_;
        }
    }

    std::string ArrayAST::toString() const
    {
        return std::string("{\n\"type\": \"array\",\n\"name\": \"" + name_ + "\",\n\"index\": " + index_->toString() + "\n}");
    }

    IfStatementAST::IfStatementAST(const TokenLocation& loc, ExprASTPtr condition, ExprASTPtr thenPart, ExprASTPtr elsePart)
        : ExprAST(loc), condition_(condition), thenPart_(thenPart), elsePart_(elsePart)
    {}

    IfStatementAST::~IfStatementAST()
    {
        if (condition_ != nullptr)
        {
            delete condition_;
        }

        if (thenPart_ != nullptr)
        {
            delete thenPart_;
        }

        if (elsePart_ != nullptr)
        {
            delete elsePart_;
        }
    }
    
    std::string IfStatementAST::toString() const
    {
        return std::string("{\n\"type\": \"IfStatement\",\n\"condition\": " + condition_->toString() + ",\n\"then part\": [" + thenPart_->toString() + "],\n\"else part\": ["+ (elsePart_ != nullptr ? elsePart_->toString() : "") + "]\n}");
    }

    WhileStatementAST::WhileStatementAST(const TokenLocation& loc, ExprASTPtr condition, ExprASTPtr body)
        : ExprAST(loc), condition_(condition), body_(body)
    {}

    WhileStatementAST::~WhileStatementAST()
    {
        if (condition_ != nullptr)
        {
            delete condition_;
        }

        if (body_ != nullptr)
        {
            delete body_;
        }
    }

    std::string WhileStatementAST::toString() const
    {
        return std::string("{\n\"type\": \"WhileStatement\",\n\"condition\": " + condition_->toString() + ",\n\"body\": [" + body_->toString() + "]\n}");
    }

    ForStatementAST::ForStatementAST(const TokenLocation& loc, ExprASTPtr variable, ExprASTPtr condition, ExprASTPtr action, ExprASTPtr body)
        : ExprAST(loc), variable_(variable), condition_(condition), action_(action), body_(body)
    {}

    ForStatementAST::~ForStatementAST()
    {
        if (variable_ != nullptr)
        {
            delete variable_;
        }

        if (condition_ != nullptr)
        {
            delete condition_;
        }

        if (action_ != nullptr)
        {
            delete action_;
        }

        if (body_ != nullptr)
        {
            delete body_;
        }
    }

    std::string ForStatementAST::toString() const
    {
        return std::string("{\n\"type\": \"ForStatement\",\n\"variable\": " + (variable_ != nullptr ? variable_->toString() : "{}") + ",\n\"condition\": " + (condition_ != nullptr ? condition_->toString() : "{}") + ",\n\"action\": " + (action_ != nullptr ? action_->toString() : "{}") + ",\n\"body\": [" + body_->toString() + "]\n}");
    }

    ReturnStatementAST::ReturnStatementAST(const TokenLocation& loc, ExprASTPtr returnStatement)
        : ExprAST(loc), returnStatement_(returnStatement)
    {}

    ReturnStatementAST::~ReturnStatementAST()
    {
        if (returnStatement_ != nullptr)
        {
            delete returnStatement_;
        }
    }

    std::string ReturnStatementAST::toString() const
    {
        return std::string("{\n\"type\": \"ReturnStatement\",\n\"expression\" :" + (returnStatement_ != nullptr ? returnStatement_->toString() : "{}") + "\n}");
    }

    PrintStatementAST::PrintStatementAST(const TokenLocation& loc, ExprASTPtr printStatement)
        : ExprAST(loc), printStatement_(printStatement)
    {}

    PrintStatementAST::~PrintStatementAST()
    {
        if (printStatement_ != nullptr)
        {
            delete printStatement_;
        }
    }

    std::string PrintStatementAST::toString() const
    {
        return std::string("{\n\"type\": \"PrintStatement\",\n\"expression\": " + printStatement_->toString() + "\n}");
    }

    NewStatementAST::NewStatementAST(const TokenLocation& loc, const std::string& type, ExprASTPtr newStatement)
        : ExprAST(loc), type_(type), newStatement_(newStatement)
    {}

    NewStatementAST::~NewStatementAST()
    {
        if (newStatement_ != nullptr)
        {
            delete newStatement_;
        }
    }

    std::string NewStatementAST::toString() const
    {
        if (typeid(*newStatement_) == typeid(MethodCallAST))
        {
            return std::string("{\n\"type\": \"NewStatement\",\n\"variable type\": \"" + type_ + "\",\n\"expression\": " + newStatement_->toString() + "\n}");
        }
        else
        {
            return std::string("{\n\"type\": \"NewStatement\",\n\"variable type\": \"" + type_ + "\",\n\"length\": " + newStatement_->toString() + "\n}");
        }
    }

    BinaryOpExpressionAST::BinaryOpExpressionAST(const TokenLocation& loc, const std::string& binaryOp, ExprASTPtr lhs, ExprASTPtr rhs)
        : ExprAST(loc), binaryOp_(binaryOp), lhs_(lhs), rhs_(rhs)
    {}

    BinaryOpExpressionAST::~BinaryOpExpressionAST()
    {
        if (lhs_ != nullptr)
        {
            delete lhs_;
        }
        
        if (rhs_ != nullptr)
        {
            delete rhs_;
        }
    }

    std::string BinaryOpExpressionAST::toString() const
    {
        return std::string("{\n\"type\": \"BinaryOpExpression\",\n\"binary operator\": \"" + binaryOp_ + "\",\n\"lhs\": " + lhs_->toString() + ",\n\"rhs\": " + rhs_->toString() + "\n}");
    }

    UnaryOpExpressionAST::UnaryOpExpressionAST(const TokenLocation& loc, const std::string& unaryOp, ExprASTPtr expression)
        : ExprAST(loc), unaryOp_(unaryOp), expression_(expression)
    {}

    UnaryOpExpressionAST::~UnaryOpExpressionAST()
    {
        if (expression_ != nullptr)
        {
            delete expression_;
        }
    }

    std::string UnaryOpExpressionAST::toString() const
    {
        return std::string("{\n\"type\": \"UnaryOpExpression\",\n\"unary operator\": \"" + unaryOp_ + "\",\n\"expression\": " + expression_->toString() + "\n}");
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