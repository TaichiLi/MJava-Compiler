// THIS FILE IS PART OF MJava-Compiler PROJECT
// parser.cpp - parser
 
// Created by Li Taiji 2020-03-28
// Copyright (c) 2020 Li Taiji All rights reserved

#include "error.h"
#include "jsonformatter.h"
#include "parser.h"
#include <cstdlib>
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
        if (size > 0)
        {
            for (size_t i = 0; i < size - 1; i++)
            {
                str << ast_[i]->toString() << ",\n";
            }
            str << ast_[size - 1]->toString();
        }

        str << "\n]";

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

    ExprASTPtr Parser::parseClassDeclaration()
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

        // if the current token is extend, then parse base class.
        if (validateToken(TokenValue::EXTENDS, true))
        {
            if (!expectToken(TokenType::IDENTIFIER, "identifier", false))
            {
                return nullptr;
            }

            baseClassName = scanner_.getToken().getTokenName();

            scanner_.getNextToken();
        }

        ExprASTPtr classBody = parseBlockOrStatement();

        return new ClassDeclarationAST(loc, className, baseClassName, classBody);
    }

    ExprASTPtr Parser::parseMethodDeclaration()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        if (!expectToken(TokenValue::PUBLIC, "public", false))
        {
            return nullptr;
        }
        
        std::vector<std::string> attributes;
        
        // push public
        attributes.push_back(scanner_.getToken().getTokenName());
        
        scanner_.getNextToken();

        // parse attributes of method.
        while (!validateToken(TokenType::TYPE, false) && !validateToken(TokenType::IDENTIFIER, false) && !validateToken(TokenValue::VOID, false))
        {
            if (!expectToken(TokenType::KEYWORD, "keyword", false))
            {
                return nullptr;
            }
            
            attributes.push_back(scanner_.getToken().getTokenName());
        
            scanner_.getNextToken();
        }
        
        std::string returnType = scanner_.getToken().getTokenName();
        
        scanner_.getNextToken();
        
        if (!validateToken(TokenValue::MAIN, false) && !validateToken(TokenType::IDENTIFIER, false))
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

        // parse parameters of method.
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

        return new MethodDeclarationAST(loc, attributes, returnType, name, parameters, body);
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

            // if the current token is '[', the type will be array.
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

    ExprASTPtr Parser::parseMethodCallStatement(const Token& token)
    {
        // consume '('
        scanner_.getNextToken();

        VecExprASTPtr args;

        // parse arguments of method call
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

        // consume possible ';'
        validateToken(TokenValue::SEMICOLON, true);

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

        // TODO: parser new statement that invokes constructor.
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
            // parse the length of array.
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

        if (!expectToken(TokenType::TYPE, "type or identifier", false))
        {
            return nullptr;
        }

        std::string type = scanner_.getToken().getTokenName();

        scanner_.getNextToken();

        // if the current token is '[', the type of variable will be array.
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

    ExprASTPtr Parser::parseVariableDeclaration(const Token& token)
    {
        std::string type = token.getTokenName();

        // if the current token is '[', the type of variable will be array.
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

        // consume possible ';'
        validateToken(TokenValue::SEMICOLON, true);

        return currentASTPtr;
    }

    ExprASTPtr Parser::parsePrimary()
    {
        Token token = scanner_.getToken();
        switch (token.getTokenType())
        {
            case TokenType::KEYWORD:
            {
                switch (token.getTokenValue())
                {
                    case TokenValue::CLASS:
                        return parseClassDeclaration();

                    case TokenValue::PUBLIC:
                        return parseMethodDeclaration();

                    case TokenValue::RETURN:
                        return parseReturnStatement();

                    case TokenValue::PRINT:
                        return parsePrintStatement();

                    case TokenValue::IF:
                        return parseIfStatement();

                    case TokenValue::WHILE:
                        return parseWhileStatement();

                    case TokenValue::FOR:
                        return parseForStatement();

                    case TokenValue::THIS:
                        return parseIdentifierExpression();

                    case TokenValue::LENGTH:
                        return parseLengthStatement();

                    case TokenValue::NEW:
                        return parseNewStatement();

                    default:
                        errorReport("I have not completed parsePrimary part");
                        std::exit(1);
                }
            }

            case TokenType::DELIMITER:
            {
                switch (token.getTokenValue())
                {
                    case TokenValue::LPAREN:
                        return parseParenExpression();
                    
                    case TokenValue::SEMICOLON:
                        return nullptr;

                    default:
                        errorReport("should not reach here, unexpected delimiter.");
                        std::exit(1);
                }
            }

            case TokenType::OPERATOR:
            {
                switch (token.getTokenValue())
                {
                    case TokenValue::NOT:
                        return parseUnaryOp();

                    default:
                        errorReport("should not reach here, unexpected operator.");
                        std::exit(1);
                }
            }

            case TokenType::TYPE:
                return parseVariableDeclaration();

            case TokenType::IDENTIFIER:
                return parseIdentifierExpression();

            case TokenType::REAL:
                return parseRealExpression();

            case TokenType::INTEGER:
                return parseIntegerExpression();

            case TokenType::CHAR_LITERAL:
                return parseCharExpression();

            case TokenType::STRING_LITERAL:
                return parseStringExpression();

            case TokenType::BOOLEAN:
                return parseBooleanExpression();

            default:
            {
                errorReport("unknown token when expecting an expression: " + scanner_.getToken().getTokenName());
                // skip the unknown token
                scanner_.getNextToken();

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

        // if the current token is identifier, then parser variable declaration.
        if (validateToken(TokenType::IDENTIFIER, false))
        {
            return parseVariableDeclaration(token);
        }

        // if the current token is '(', then parse method call statement.
        if (validateToken(TokenValue::LPAREN, false))
        {
            return parseMethodCallStatement(token);
        }

        // if the current token is '[', then parse index of array or variable declaration.
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

    ExprASTPtr Parser::parseRealExpression()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        double real = scanner_.getToken().getRealValue();

        scanner_.getNextToken();

        return new RealAST(loc, real);
    }

    ExprASTPtr Parser::parseIntegerExpression()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        int integer = scanner_.getToken().getIntValue();

        scanner_.getNextToken();

        return new IntegerAST(loc, integer);
    }

    ExprASTPtr Parser::parseCharExpression()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        char ch = scanner_.getToken().getIntValue();

        scanner_.getNextToken();

        return new CharAST(loc, ch);
    }

    ExprASTPtr Parser::parseStringExpression()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        std::string string = scanner_.getToken().getStringValue();

        scanner_.getNextToken();

        return new StringAST(loc, string);
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

            // if the precedence of current token less than the precedence of the last token, return current ast.
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
            // if the precedence of current token less than the precedence of the next token, continue to parse.
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

        // if do not have '{' , then there can be only one statement.
        if (!validateToken(TokenValue::LBRACE, false))
        {
            return parseExpression();
        }

        // consume '{'
        scanner_.getNextToken();

        VecExprASTPtr stmts;

        while (!validateToken(TokenValue::RBRACE, false) && !validateToken(TokenType::END_OF_FILE, false))
        {
            ExprASTPtr currentASTPtr = parseExpression();

            if (currentASTPtr == nullptr)
            {
                continue;
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

        if (thenPart == nullptr)
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

    ExprASTPtr Parser::parseWhileStatement()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        // current token is keyword while.
        if (!expectToken(TokenValue::WHILE, "while", true))
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
            return nullptr;
        }

        if (!expectToken(TokenValue::RPAREN, ")", true))
        {
            return nullptr;
        }

        auto body = parseBlockOrStatement();

        if (body == nullptr)
        {
            return nullptr;
        }

        return new WhileStatementAST(loc, condition, body);
    }

    ExprASTPtr Parser::parseForStatement()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        if (!expectToken(TokenValue::FOR, "for", true))
        {
            return nullptr;
        }

        if (!expectToken(TokenValue::LPAREN, "(", true))
        {
            return nullptr;
        }

        // control variable, control condition and action can all be nullptr.
        ExprASTPtr variable = parseExpression();

        ExprASTPtr condition = parseExpression();

        ExprASTPtr action = parseExpression();

        if (!expectToken(TokenValue::RPAREN, ")", true))
        {
            return nullptr;
        }

        ExprASTPtr body = parseBlockOrStatement();

        if (body == nullptr)
        {
            return nullptr;
        }

        return new ForStatementAST(loc, variable, condition, action, body);
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