// THIS FILE IS PART OF MJava-Compiler PROJECT
// parser.cpp - parser
 
// Created by Li Taiji 2020-03-28
// Copyright (c) 2020 Li Taiji All rights reserved

#include "error.h"
#include "jsonformatter.h"
#include "parser.h"
#include <memory>
#include <sstream>

namespace MJava
{
    bool Parser::errorFlag_ = false;

    Parser::Parser(Scanner& scanner):scanner_(scanner)
    {
        // Eat the first token.
        scanner_.getNextToken();
    }
    
    std::string Parser::toString()
    {
        std::ostringstream str;
        str << "[\n";

        size_t size = ast_.size();
        for (size_t i = 0; i < size - 1; i++)
        {
            str << ast_[i]->toString() << ",\n";
        }
        str << ast_[size - 1]->toString() << "\n]";

        return JSONFormatter::format(str.str());
    }

    VecExprASTPtr& Parser::parse()
    {
        if (scanner_.getToken().getTokenType() == TokenType::END_OF_FILE)
        {
            errorReport("The file is empty.");
            ast_.clear();
            return ast_;
        }

        for (;;)
        {
            ExprASTPtr currentASTPtr = nullptr;
            switch (scanner_.getToken().getTokenValue())
            {
                case TokenValue::CLASS:
                {
                    currentASTPtr = parseClass();
                    break;
                }

                case TokenValue::SEMICOLON:
                {
                    scanner_.getNextToken();
                    break;
                }

                default:
                {
                    currentASTPtr = parseExpression();
                    break;
                }
            }

            if (currentASTPtr != nullptr)
            {
                ast_.push_back(currentASTPtr);
            }
            
            if (scanner_.getToken().getTokenType() == TokenType::END_OF_FILE)
            {
                return ast_;
            }
        }
    }

    ExprASTPtr Parser::parseClass()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        if (!expectToken(TokenValue::CLASS, "class", true))
        {
            return nullptr;
        }

        if (!expectToken(TokenType::IDENTIFIER, "identifier", false))
        {
            return nullptr;
        }

        std::string className = scanner_.getToken().getTokenName();
        std::string baseClassName;

        scanner_.getNextToken();

        bool isBaseClass = false;

        if (!validateToken(TokenValue::EXTENDS, true))
        {
            isBaseClass = false;
        }
        else
        {
            isBaseClass = true;
            if (!expectToken(TokenType::IDENTIFIER, "identifier", false))
            {
                return nullptr;
            }

            baseClassName = scanner_.getToken().getTokenName();

            scanner_.getNextToken();
        }

        ExprASTPtr classBody = parseBlockOrStatement();

        if (isBaseClass)
        {
            return new ClassAST(loc, className, isBaseClass, baseClassName, classBody);
        }
        else
        {
            return new ClassAST(loc, className, isBaseClass, classBody);
        }   
    }

    ExprASTPtr Parser::parseMethodDeclaration()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        if (!expectToken(TokenValue::PUBLIC, "public", true))
        {
            return nullptr;
        }

        if (validateToken(TokenValue::STATIC, true))
        {
            if (!expectToken(TokenValue::VOID, "void", false))
            {
                return nullptr;
            }
            std::string returnType = scanner_.getToken().getTokenName();
            scanner_.getNextToken();

            if (!expectToken(TokenValue::MAIN, "main", false))
            {
                return nullptr;
            }
            std::string name = scanner_.getToken().getTokenName();
            scanner_.getNextToken();

            if (!expectToken(TokenValue::LPAREN, "(", true))
            {
                return nullptr;
            }

            VecExprASTPtr parameters;
            while (!validateToken(TokenValue::RPAREN, true))
            {
                ExprASTPtr parameter = parseMethodParameter();
                if (parameter != nullptr)
                {
                    parameters.push_back(parameter);
                }
            }

            ExprASTPtr body = parseBlockOrStatement();
            if (body == nullptr)
            {
                return nullptr;
            }

            return new MethodDeclarationAST(loc, returnType, name, parameters, body);
        }
        else
        {
            if (!validateToken(TokenType::TYPE, false) && !validateToken(TokenType::IDENTIFIER, false))
            {
                errorReport("Expected ' type or identifier ', but find " + scanner_.getToken().tokenTypeDescription() + " " + scanner_.getToken().getTokenName());
                return nullptr;
            }
            std::string returnType = scanner_.getToken().getTokenName();
            scanner_.getNextToken();

            if (validateToken(TokenValue::LBRACK, true))
            {
                if (!expectToken(TokenValue::RBRACK, "]", true))
                {
                    return nullptr;
                }
                returnType += "[]";
            }

            if (!expectToken(TokenType::IDENTIFIER, "identifier", false))
            {
                return nullptr;
            }
            std::string name = scanner_.getToken().getTokenName();
            scanner_.getNextToken();

            if (!expectToken(TokenValue::LPAREN, "(", true))
            {
                return nullptr;
            }

            VecExprASTPtr parameters;
            while (!validateToken(TokenValue::RPAREN, true))
            {
                ExprASTPtr parameter = parseMethodParameter();
                if (parameter != nullptr)
                {
                    parameters.push_back(parameter);
                }
            }

            ExprASTPtr body = parseBlockOrStatement();
            if (body == nullptr)
            {
                return nullptr;
            }

            return new MethodDeclarationAST(loc, returnType, name, parameters, body);
        }
    }

    ExprASTPtr Parser::parseMethodParameter()
    {
        if (!validateToken(TokenValue::RPAREN, true))
        {
            TokenLocation loc = scanner_.getToken().getTokenLocation();

            if (!validateToken(TokenType::TYPE, false) && !validateToken(TokenType::IDENTIFIER, false))
            {
                errorReport("Expected ' type or identifier ', but find " + scanner_.getToken().tokenTypeDescription() + " " + scanner_.getToken().getTokenName());
                return nullptr;
            }
            std::string type = scanner_.getToken().getTokenName();
            scanner_.getNextToken();

            if (validateToken(TokenValue::LBRACK, true))
            {
                if (!expectToken(TokenValue::RBRACK, "]", true))
                {
                    return nullptr;
                }
                type += "[]";
            }

            if (!expectToken(TokenType::IDENTIFIER, "identifier", false))
            {
                return nullptr;
            }
            std::string name = scanner_.getToken().getTokenName();
            scanner_.getNextToken();

            if (validateToken(TokenValue::RPAREN, false))
            {
                return new VariableDeclarationAST(loc, type, name);
            }

            if (!expectToken(TokenValue::COMMA, ",", true))
            {
                return nullptr;
            }

            return new VariableDeclarationAST(loc, type, name);
        }

        return nullptr;
    }

    
    ExprASTPtr Parser::parseMethodCallStatement(const Token &token)
    {
        scanner_.getNextToken();

        VecExprASTPtr args;
        if (!validateToken(TokenValue::RPAREN, true))
        {
            while (true)
            {
                ExprASTPtr arg = parseExpression();
                if (arg == nullptr)
                {
                    return nullptr;
                }

                args.push_back(arg);

                if (validateToken(TokenValue::RPAREN, true))
                {
                    break;
                }

                if (!expectToken(TokenValue::COMMA, ",", true))
                {
                    return nullptr;
                }
            }
        }

        return new MethodCallAST(token.getTokenLocation(), token.getTokenName(), args);
    }

    ExprASTPtr Parser::parseIntegerExpression()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();
        int integer = std::stoi(scanner_.getToken().getTokenName());

        scanner_.getNextToken();

        return new IntegerAST(loc, integer);
    }

    ExprASTPtr Parser::parseLengthStatement()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        if (!expectToken(TokenValue::LENGTH, "length", true))
        {
            return nullptr;
        }

        VecExprASTPtr parameters;

        return new MethodCallAST(loc, "length", parameters);
    }

    ExprASTPtr Parser::parsePrintStatement()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        if (!expectToken(TokenValue::PRINT, "System.out.println", true))
        {
            return nullptr;
        }

        if (!expectToken(TokenValue::LPAREN, "(", true))
        {
            return nullptr;
        }

        ExprASTPtr printStatement = parseExpression();
        if (printStatement == nullptr)
        {
            return nullptr;
        }

        if (!expectToken(TokenValue::RPAREN, ")", true))
        {
            return nullptr;
        }

        if (!expectToken(TokenValue::SEMICOLON, ";", true))
        {
            return nullptr;
        }

        return new PrintStatementAST(loc, printStatement);
    }

    ExprASTPtr Parser::parseReturnStatement()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        if (!expectToken(TokenValue::RETURN, "return", true))
        {
            return nullptr;
        }

        ExprASTPtr returnStatement = parseExpression();
        if (returnStatement == nullptr)
        {
            return nullptr;
        }

        return new ReturnStatementAST(loc, returnStatement);
    }

    ExprASTPtr Parser::parseNewStatement()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        if (!expectToken(TokenValue::NEW, "new", true))
        {
            return nullptr;
        }

        if (!validateToken(TokenType::TYPE, false) && !validateToken(TokenType::IDENTIFIER, false))
        {
            errorReport("Expected ' type or identifier ', but find " + scanner_.getToken().tokenTypeDescription() + " " + scanner_.getToken().getTokenName());
            return nullptr;
        }
        std::string type = scanner_.getToken().getTokenName();
        scanner_.getNextToken();

        if (validateToken(TokenValue::LPAREN, true))
        {
            if (!expectToken(TokenValue::RPAREN, ")", true))
            {
                return nullptr;
            }
        }

        ExprASTPtr length = nullptr;
        if (validateToken(TokenValue::LBRACK, true))
        {
            length = parseExpression();
            if (length == nullptr)
            {
                return nullptr;
            }

            if (!expectToken(TokenValue::RBRACK, "]", true))
            {
                return nullptr;
            }
            
            type += "[]";
        }

        if (!expectToken(TokenValue::SEMICOLON, ";", true))
        {
            return nullptr;
        }

        return new NewStatementAST(loc, type, length);
    }

    ExprASTPtr Parser::parseVariableDeclaration()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        if (!expectToken(TokenType::TYPE, "int or String", false))
        {
            return nullptr;
        }
        std::string type = scanner_.getToken().getTokenName();

        scanner_.getNextToken();

        if (validateToken(TokenValue::LBRACK, true))
        {
            if (!expectToken(TokenValue::RBRACK, "]", true))
            {
                return nullptr;
            }
            type += "[]";
        }

        if (!expectToken(TokenType::IDENTIFIER, "identifier", false))
        {
            return nullptr;
        }
        std::string name = scanner_.getToken().getTokenName();

        scanner_.getNextToken();
        
        if (!expectToken(TokenValue::SEMICOLON, ";", true))
        {
                return nullptr;
        }

        return new VariableDeclarationAST(loc, type, name);
    }

    ExprASTPtr Parser::parseVariableDeclaration(const Token &token)
    {
        std::string type = token.getTokenName();

        if (validateToken(TokenValue::LBRACK, true))
        {
            if (!expectToken(TokenValue::RBRACK, "]", true))
            {
                return nullptr;
            }
            type += "[]";
        }

        if (!expectToken(TokenType::IDENTIFIER, "identifier", false))
        {
            return nullptr;
        }
        std::string name = scanner_.getToken().getTokenName();

        scanner_.getNextToken();
        
        if (!expectToken(TokenValue::SEMICOLON, ";", true))
        {
            return nullptr;
        }

        return new VariableDeclarationAST(token.getTokenLocation(), type, name);
    }

    ExprASTPtr Parser::parseExpression()
    {
        auto lhs = parsePrimary();

        if (lhs == nullptr)
        {
            return nullptr;
        }
        ExprASTPtr currentASTPtr = parseBinOpRHS(0, lhs);

        validateToken(TokenValue::SEMICOLON, true);

        return currentASTPtr;
    }

    ExprASTPtr Parser::parsePrimary()
    {
        switch (scanner_.getToken().getTokenValue())
        {
            case TokenValue::PUBLIC:
                return parseMethodDeclaration();

            case TokenValue::PRINT:
                return parsePrintStatement();

            case TokenValue::INT: 
            case TokenValue::BOOL:
                return parseVariableDeclaration();

            case TokenValue::TRUE:
            case TokenValue::FALSE:
                return parseBooleanExpression();

            case TokenValue::LPAREN:
                return parseParenExpression();

            case TokenValue::CLASS:
                return parseClass();

            case TokenValue::IF:
                return parseIfStatement();

            case TokenValue::RETURN:
                return parseReturnStatement();

            case TokenValue::THIS:
                return parseIdentifierExpression();

            case TokenValue::LENGTH:
                return parseLengthStatement();

            case TokenValue::WHILE:
                return parseWhileStatement();

            case TokenValue::NEW:
                return parseNewStatement();

            case TokenValue::NOT:
                return parseUnaryOp();

            default: 
            {
                switch (scanner_.getToken().getTokenType())
                {
                    case TokenType::IDENTIFIER:
                    {
                        return parseIdentifierExpression();
                    }

                    case TokenType::INTEGER:
                    {
                        return parseIntegerExpression();
                    } 
                }
                errorReport("unknown token when expecting an expression");
                std::cout <<  scanner_.getToken().getTokenName() << std::endl;
                return nullptr;
            }
        }
    }

    ExprASTPtr Parser::parseIdentifierExpression()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        if (!validateToken(TokenType::IDENTIFIER, false) && !validateToken(TokenValue::THIS, false))
        {
            errorReport("Expected ' identifier or this ', but find " + scanner_.getToken().tokenTypeDescription() + " " + scanner_.getToken().getTokenName());
            return nullptr;
        }
        Token token = scanner_.getToken();

        scanner_.getNextToken();

        if (validateToken(TokenType::IDENTIFIER, false))
        {
            return parseVariableDeclaration(token);
        }

        if (validateToken(TokenValue::LPAREN, false))
        {
            return parseMethodCallStatement(token);
        }

        if (validateToken(TokenValue::LBRACK, true))
        {
            if (validateToken(TokenValue::RBRACK, true))
            {
                std::string name = scanner_.getToken().getTokenName();
                scanner_.getNextToken();

                if (!expectToken(TokenValue::SEMICOLON, ";", true))
                {
                    return nullptr;
                }

                return new VariableDeclarationAST(loc, token.getTokenName() + "[]", name);                
            }
            else
            {
                ExprASTPtr index = parseExpression();

                if (index == nullptr)
                {
                    return nullptr;
                }

                if (!expectToken(TokenValue::RBRACK, "]",  true))
                {
                    return nullptr;
                }

                return new VariableAST(loc, token.getTokenName(), index);
            }
        }

        return new VariableAST(loc, token.getTokenName(), nullptr);
    }

    ExprASTPtr Parser::parseBooleanExpression()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        bool boolean = (scanner_.getToken().getTokenName() == "true");
        scanner_.getNextToken();

        return new BooleanAST(loc, boolean);
    }

    ExprASTPtr Parser::parseBinOpRHS(int precedence, ExprASTPtr lhs)
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        ExprASTPtr expr = lhs;
        while (true)
        {
            int currentPrecedence = scanner_.getToken().getSymbolPrecedence();
            if (currentPrecedence < precedence)
            {
                return expr;
            }

            std::string binOp = scanner_.getToken().getTokenName();
            scanner_.getNextToken();

            ExprASTPtr rhs = parsePrimary();

            if (rhs == nullptr)
            {
                return nullptr;
            }

            int nextPrecedence = scanner_.getToken().getSymbolPrecedence();
            if (currentPrecedence < nextPrecedence)
            {
                rhs = parseBinOpRHS(currentPrecedence + 1, rhs);
                if (rhs == nullptr)
                {
                    return nullptr;
                }
            }

            expr = new BinaryOpExpressionAST(loc, binOp, expr, rhs);
        }

        return nullptr;
    }

    ExprASTPtr Parser::parseUnaryOp()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        std::string unaryOp = scanner_.getToken().getTokenName();

        scanner_.getNextToken();

        auto currentASTPtr = parsePrimary();
        if (currentASTPtr == nullptr)
        {
            return nullptr;
        }
        return new UnaryOpExpressionAST(loc, unaryOp, currentASTPtr);
    }

    ExprASTPtr Parser::parseParenExpression()
    {
        // consume '('
        scanner_.getNextToken();

        // parse main expression
        ExprASTPtr currentASTPtr = parseExpression();
        if (currentASTPtr == nullptr)
        {
            return nullptr;
        }

        if (!expectToken(TokenValue::RPAREN, ")", true))
        {
            return nullptr;
        }

        return currentASTPtr;
    }

    ExprASTPtr Parser::parseBlockOrStatement()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        if (!validateToken(TokenValue::LBRACE, false))
        {
            return parseExpression();
        }

        scanner_.getNextToken();

        VecExprASTPtr stmts;

        while (!validateToken(TokenValue::RBRACE, false) && !validateToken(TokenType::END_OF_FILE, false))
        {
            ExprASTPtr currentASTPtr = parseExpression();
            if (currentASTPtr == nullptr)
            {
               return parseBlockOrStatement(); 
            }

            stmts.push_back(currentASTPtr);
        }

        if (!expectToken(TokenValue::RBRACE, "}", true))
        {
            return nullptr;
        }

        return new BlockAST(loc, stmts);
    }

    ExprASTPtr Parser::parseIfStatement()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        // current token is keyword if.
        if (!expectToken(TokenValue::IF, "if", true))
        {
            return nullptr;
        }

        if (!expectToken(TokenValue::LPAREN, "(", true))
        {
            return nullptr;
        }

        auto condition = parseExpression();

        if (condition == nullptr)
        {
            errorReport("if condition is not valid.");
            return nullptr;
        }

        if (!expectToken(TokenValue::RPAREN, ")", true))
        {
            return nullptr;
        }

        ExprASTPtr thenPart = parseBlockOrStatement();

        if (!thenPart)
        {
            errorReport("then statement is not valid.");
            return nullptr;
        }

        ExprASTPtr elsePart = nullptr;

        // if we have token else.
        if (validateToken(TokenValue::ELSE, true))
        {
            elsePart = parseBlockOrStatement();
            if (!elsePart)
            {
                errorReport("else statement is not valid.");
                return nullptr;
            }
        }

        return new IfStatementAST(loc, condition, thenPart, elsePart);
    }

    // ExprASTPtr Parser::parseForStatement()
    // {
    //     TokenLocation loc = scanner_.getToken().getTokenLocation();

    //     if(!expectToken(TokenValue::FOR, "for", true))
    //     {
    //         return nullptr;
    //     }

    //     if(!validateToken(TokenType::IDENTIFIER, false))
    //     {
    //         errorReport("For statement control varible expected identifier type but find " + scanner_.getToken().toString());
    //         return nullptr;
    //     }

    //     auto controlVariable = scanner_.getToken().getIdentifierName();

    //     if(!expectToken(TokenValue::ASSIGN, ":=", true))
    //     {
    //         return nullptr;
    //     }

    //     auto startExpr = parseExpression();

    //     if(!startExpr)
    //     {
    //         return nullptr;
    //     }

    //     bool downOrder = false;

    //     if (validateToken(TokenValue::TO, false) ||
    //         validateToken(TokenValue::DOWNTO, false))
    //     {
    //         if (scanner_.getToken().getTokenValue() == TokenValue::DOWNTO)
    //         {
    //             downOrder = true;
    //         }
    //         scanner_.getNextToken();
    //     }
    //     else
    //     {
    //         errorReport("Expected to / downto keyword, but find " + scanner_.getToken().toString());
    //         return nullptr;
    //     }

    //     auto endExpr = parseExpression();

    //     if(!endExpr)
    //     {
    //         return nullptr;
    //     }

    //     if(!expectToken(TokenValue::DO, "do", true))
    //     {
    //         return nullptr;
    //     }

    //     auto body = parseBlockOrStatement();

    //     if(!body)
    //     {
    //         return nullptr;
    //     }

    //     return std::make_unique<ForStatementAST>(loc, controlVariable,
    //         std::move(startExpr), std::move(endExpr), downOrder, std::move(body));

    // }

    ExprASTPtr Parser::parseWhileStatement()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        if(!expectToken(TokenValue::WHILE, "while", true))
        {
            return nullptr;
        }

        if(!expectToken(TokenValue::LPAREN, "(", true))
        {
            return nullptr;
        }

        auto condition = parseExpression();

        if(!condition)
        {
            return nullptr;
        }

        if(!expectToken(TokenValue::RPAREN, ")", true))
        {
            return nullptr;
        }

        auto body = parseBlockOrStatement();

        if(!body)
        {
            return nullptr;
        }

        return new WhileStatementAST(loc, condition, body);
    }

    // Helper Functions.

    bool Parser::expectToken(TokenValue value, const std::string& tokenName, bool advanceToNextToken)
    {
        if (scanner_.getToken().getTokenValue() != value)
        {
            errorReport("Expected ' " + tokenName + " ', but find " + scanner_.getToken().getTokenName());
            return false;
        }

        if (advanceToNextToken)
        {
            scanner_.getNextToken();
        }

        return true;
    }

    bool Parser::expectToken(TokenType type, const std::string& tokenTypeDescription, bool advanceToNextToken)
    {
        if (scanner_.getToken().getTokenType() != type)
        {
            errorReport("Expected ' " + tokenTypeDescription + " ', but find " + scanner_.getToken().tokenTypeDescription() + " " + scanner_.getToken().getTokenName());
            return false;
        }

        if (advanceToNextToken)
        {
            scanner_.getNextToken();
        }

        return true;
    }

    void Parser::setErrorFlag(bool flag)
    {
        errorFlag_ = flag;
    }

    // validate the token.
    // if validatation is true, return true. Otherwise return false.
    // meanwhile, if validation is true and need to advance to next token,
    // we will call getNextToken function.
    //
    // Note:
    // after call valadateToken, we always need to do things to handle different conditions,
    // i.e. if it is true, what will we do. if it is false. what will we do.

    bool Parser::validateToken(TokenValue value, bool advanceToNextToken)
    {
        if (scanner_.getToken().getTokenValue() != value)
        {
            return false;
        }

        if (advanceToNextToken)
        {
            scanner_.getNextToken();
        }

        return true;
    }

    bool Parser::validateToken(TokenType type, bool advanceToNextToken)
    {
        if (scanner_.getToken().getTokenType() != type)
        {
            return false;
        }

        if (advanceToNextToken)
        {
            scanner_.getNextToken();
        }

        return true;
    }

    void Parser::errorReport(const std::string& msg)
    {
        errorSyntax(scanner_.getToken().getTokenLocation().toString() + msg);
    }
} // namespace MJava