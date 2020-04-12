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

    Parser::~Parser()
    {
        if (program_ != nullptr)
        {
            delete program_;
        }
    }

    std::string Parser::toString()
    {
        if (program_ != nullptr)
        {
            return JSONFormatter::format(program_->toString());
        }
        else
        {
            return "";
        }
    }

    // Goal ::= MainClass ( TypeDeclaration )* <EOF>
    ProgramASTPtr Parser::parse()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        VecExprASTPtr classes;

        if (scanner_.getToken().getTokenType() == TokenType::END_OF_FILE)
        {
            errorReport("The file is empty.");
            classes.clear();
            program_ = new ProgramAST(loc, classes);
            return program_;
        }

        // the first class must be main class
        ExprASTPtr mainClass = parseMainClass();

        if (mainClass != nullptr)
        {
            classes.push_back(mainClass);
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
                // program can only contain classes.
                if (expectAST(ASTType::CLASSDECLARATION, "class declaration", currentASTPtr))
                {
                    classes.push_back(currentASTPtr);
                }
            }
            
            if (scanner_.getToken().getTokenType() == TokenType::END_OF_FILE)
            {
                program_ = new ProgramAST(mainClass->getTokenLocation(), classes);
                return program_;
            }
        }
    }

    // MainClass ::= "class" Identifier "{" "public" "static" "void" "main" "(" "String" "[" "]" Identifier ")" "{" ( VarDeclaration )* ( Statement )* "}" "}"
    ExprASTPtr Parser::parseMainClass()
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

        scanner_.getNextToken();

        if (!expectToken(TokenValue::LBRACE, "{", true))
        {
            return nullptr;
        }

        // main class can only have one main method.
        ExprASTPtr mainMethod = parseMainMethod();

        if (mainMethod == nullptr)
        {
            return nullptr;
        }

        if (!expectToken(TokenValue::RBRACE, "}", true))
        {
            return nullptr;
        }

        return new MainClassAST(loc, className, mainMethod);
    }

    ExprASTPtr Parser::parseMainMethod()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        if (!expectToken(TokenValue::PUBLIC, "public", true))
        {
            return nullptr;
        }

        std::vector<std::string> attributes;
        
        // push "public"
        attributes.push_back("public");

        if (!expectToken(TokenValue::STATIC, "static", true))
        {
            return nullptr;
        }

        // push "static"
        attributes.push_back("static");

        if (!expectToken(TokenValue::VOID, "void", true))
        {
            return nullptr;
        }

        std::string returnType = "void";

        // main method name should be "main"
        if (!expectToken(TokenValue::MAIN, "main", true))
        {
            return nullptr;
        }

        std::string name = "main";

        if (!expectToken(TokenValue::LPAREN, "(", true))
        {
            return nullptr;
        }

        VecExprASTPtr parameters;

        TokenLocation parameterLoc = scanner_.getToken().getTokenLocation();

        // parameter should be String[]
        if (!expectToken(TokenValue::STRING, "String", true))
        {
            return nullptr;
        }

        if (!expectToken(TokenValue::LBRACK, "[", true))
        {
            return nullptr;
        }

        if (!expectToken(TokenValue::RBRACK, "]", true))
        {
            return nullptr;
        }

        if (!expectToken(TokenType::IDENTIFIER, "identifier", false))
        {
            return nullptr;
        }

        parameters.push_back(new VariableDeclarationAST(parameterLoc, "String[]", scanner_.getToken().getTokenName()));

        scanner_.getNextToken();

        if (!expectToken(TokenValue::RPAREN, ")", true))
        {
            return nullptr;
        }

        ExprASTPtr body = parseMainMethodBody();
        
        if (body == nullptr)
        {
            return nullptr;
        }

        return new MethodDeclarationAST(loc, attributes, returnType, name, parameters, body);
    }

    ExprASTPtr Parser::parseMainMethodBody()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        if (!expectToken(TokenValue::LBRACE, "{", true))
        {
            return nullptr;
        }

        VecExprASTPtr localVariables;
        VecExprASTPtr methodBody;

        ExprASTPtr currentASTPtr = nullptr;

        while (!validateToken(TokenValue::RBRACE, false) && !validateToken(TokenType::END_OF_FILE, false))
        {
            currentASTPtr = parseExpression();

            if (currentASTPtr != nullptr)
            {
                if (validateAST(ASTType::VARIABLEDECLARATION, currentASTPtr))
                {
                    localVariables.push_back(currentASTPtr);
                }
                else
                {
                    methodBody.push_back(currentASTPtr);
                    break;
                }
            }
        }

        if (validateToken(TokenType::END_OF_FILE, false))
        {
            errorReport("Unexpected end of file in the method body");
            return nullptr;
        }

        while (!validateToken(TokenValue::RBRACE, false) && !validateToken(TokenType::END_OF_FILE, false))
        {
            currentASTPtr = parseExpression();

            if (currentASTPtr != nullptr)
            {
                if (!validateAST(ASTType::CLASSDECLARATION, currentASTPtr) && !validateAST(ASTType::MAINCLASS, currentASTPtr) && !validateAST(ASTType::METHODDECLARATION, currentASTPtr) && !validateAST(ASTType::VARIABLEDECLARATION, currentASTPtr))
                {
                    methodBody.push_back(currentASTPtr);
                }
                else
                {
                    errorReport(currentASTPtr, "Find unexpected " + currentASTPtr->getASTTypeDescription());
                }
            }
        }

        if (!expectToken(TokenValue::RBRACE, "}", true))
        {
            return nullptr;
        }

        // main method don't have return statement.
        return new MethodBodyAST(loc, localVariables, methodBody, nullptr);
    }

    VecExprASTPtr Parser::parseClassMemberVariables()
    {
        VecExprASTPtr memberVariables;

        // All shoule be member variables before method declaration.
        while (!validateToken(TokenValue::RBRACE, false) && !validateToken(TokenValue::PUBLIC, false) && !validateToken(TokenType::END_OF_FILE, false))
        {
            ExprASTPtr memberVariable = parseExpression();

            if (memberVariable != nullptr)
            {
                if (expectAST(ASTType::VARIABLEDECLARATION, "variable declaration", memberVariable))
                {
                    memberVariables.push_back(memberVariable);
                }
            }
        }

        if (validateToken(TokenType::END_OF_FILE, false))
        {
            errorReport("Unexpected end of file in the class body");
        }

        return memberVariables;
    }

    VecExprASTPtr Parser::parseClassMemberMethods()
    {
        VecExprASTPtr memberMethods;

        while (!validateToken(TokenValue::RBRACE, false) && !validateToken(TokenType::END_OF_FILE, false))
        {
            ExprASTPtr memberMethod = parseExpression();

            if (memberMethod != nullptr)
            {
                if (expectAST(ASTType::METHODDECLARATION, "method declaration", memberMethod))
                {
                    memberMethods.push_back(memberMethod);
                }
            }
        }

        if (validateToken(TokenType::END_OF_FILE, false))
        {
            errorReport("Unexpected end of file in the class body");
        }

        return memberMethods;
    }

    // ClassDeclaration ::= "class" Identifier "{" ( VarDeclaration )* ( MethodDeclaration )* "}"
    // ClassExtendsDeclaration ::= "class" Identifier "extends" Identifier "{" ( VarDeclaration )* ( MethodDeclaration )* "}"
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

        if (!expectToken(TokenValue::LBRACE, "{", true))
        {
            return nullptr;
        }

        // In a class declaration, the first is member variables and then is member methods.
        const VecExprASTPtr& memberVariables = parseClassMemberVariables();

        const VecExprASTPtr& memberMethods = parseClassMemberMethods();

        if (!expectToken(TokenValue::RBRACE, "}", true))
        {
            return nullptr;
        }

        return new ClassDeclarationAST(loc, className, baseClassName, memberVariables, memberMethods);
    }

    // MethodBody := "{" ( VarDeclaration )* ( Statement )* "return" Expression ";" "}"
    ExprASTPtr Parser::parseMethodBody()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        if (!expectToken(TokenValue::LBRACE, "{", true))
        {
            return nullptr;
        }

        VecExprASTPtr localVariables;
        VecExprASTPtr methodBody;

        ExprASTPtr currentASTPtr = nullptr;

        while (!validateToken(TokenValue::RETURN, false) && !validateToken(TokenType::END_OF_FILE, false))
        {
            currentASTPtr = parseExpression();

            if (currentASTPtr != nullptr)
            {
                if (validateAST(ASTType::VARIABLEDECLARATION, currentASTPtr))
                {
                    localVariables.push_back(currentASTPtr);
                }
                else
                {
                    methodBody.push_back(currentASTPtr);
                    break;
                }
            }
        }

        if (validateToken(TokenType::END_OF_FILE, false))
        {
            errorReport("Unexpected end of file in the method body");
            return nullptr;
        }

        while (!validateToken(TokenValue::RETURN, false) && !validateToken(TokenType::END_OF_FILE, false))
        {
            currentASTPtr = parseExpression();

            if (currentASTPtr != nullptr)
            {
                if (!validateAST(ASTType::CLASSDECLARATION, currentASTPtr) && !validateAST(ASTType::MAINCLASS, currentASTPtr) && !validateAST(ASTType::METHODDECLARATION, currentASTPtr) && !validateAST(ASTType::VARIABLEDECLARATION, currentASTPtr))
                {
                    methodBody.push_back(currentASTPtr);
                }
                else
                {
                    errorReport(currentASTPtr, "Find unexpected " + currentASTPtr->getASTTypeDescription());
                }
            }
        }

        // method must have one return statement.
        if (!expectToken(TokenValue::RETURN, "return", false))
        {
            return nullptr;
        }

        ExprASTPtr returnStatement = parseReturnStatement();

        if (returnStatement == nullptr)
        {
            return nullptr;
        }

        if (!expectToken(TokenValue::RBRACE, "}", true))
        {
            return nullptr;
        }
        
        return new MethodBodyAST(loc, localVariables, methodBody, returnStatement);
    }

    // MethodDeclaration ::= "public" Type Identifier "(" ( FormalParameterList )? ")" "{" ( VarDeclaration )* ( Statement )* "return" Expression ";" "}"
    ExprASTPtr Parser::parseMethodDeclaration()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        if (!expectToken(TokenValue::PUBLIC, "public", true))
        {
            return nullptr;
        }

        std::vector<std::string> attributes;
        
        // push public
        attributes.push_back("public");

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

        if (validateToken(TokenValue::LBRACK, true))
        {
            if (!expectToken(TokenValue::RBRACK, "]", true))
            {
                return nullptr;
            }

            returnType += "[]";
        }

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
        // FormalParameterList ::= FormalParameter FormalParameterTail
        // FormalParameter ::= Type Identifier
        // FormalParameterTail ::= ( FormalParameterTerm )*
        // FormalParameterTerm ::= "," FormalParameter
        while (!validateToken(TokenValue::RPAREN, true))
        {
            ExprASTPtr parameter = parseMethodParameter();

            if (parameter != nullptr)
            {
                parameters.push_back(parameter);
            }
        }

        ExprASTPtr body = parseMethodBody();
        
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

        VecExprASTPtr arguments;

        // parse arguments of method call
        if (!validateToken(TokenValue::RPAREN, true))
        {
            while (true)
            {
                ExprASTPtr argument = parseExpression();

                if (argument == nullptr)
                {
                    return nullptr;
                }

                arguments.push_back(argument);

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

        return new MethodCallAST(token.getTokenLocation(), token.getTokenName(), arguments);
    }

    ExprASTPtr Parser::parseLengthStatement()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        if (!expectToken(TokenValue::LENGTH, "length", true))
        {
            return nullptr;
        }

        VecExprASTPtr arguments;

        return new MethodCallAST(loc, "length", arguments);
    }

    // PrintStatement ::= "System.out.println" "(" Expression ")" ";"
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

    // ReturnStatement ::= "return" Expression ";"
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

    // ArrayAllocationExpression ::= BooleanArrayAllocationExpression | IntegerArrayAllocationExpression
    // BooleanArrayAllocationExpression ::= "new" "boolean" "[" Expression "]"
    // IntegerArrayAllocationExpression ::= "new" "int" "[" Expression "]"
    // AllocationExpression ::= "new" Identifier "(" ")"
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

        Token token = scanner_.getToken();
        std::string type = token.getTokenName();

        scanner_.getNextToken();

        ExprASTPtr expression = nullptr;

        if (validateToken(TokenValue::LPAREN, false))
        {
            expression = parseMethodCallStatement(token);
        }

        if (validateToken(TokenValue::LBRACK, true))
        {
            // parse the length of array.
            expression = parseExpression();

            if (expression == nullptr)
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

        return new NewStatementAST(loc, type, expression);
    }

    // VarDeclaration ::= Type Identifier ";"
    // Type ::=	ArrayType | booleanType	|IntegerType |Identifier
    // BooleanArrayType ::= "boolean" "[" "]"
    // IntegerArrayType ::= "int" "[" "]"
    // BooleanType ::= "boolean"
    // IntegerType ::= "int"
    ExprASTPtr Parser::parseVariableDeclaration()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        if (!expectToken(TokenType::TYPE, "type", false))
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

    // parse all primary expression
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

                return new ArrayAST(loc, token.getTokenName(), index);
            }
        }

        return new VariableAST(loc, token.getTokenName());
    }

    // RealLiteral ::= <REALLITERAL>
    ExprASTPtr Parser::parseRealExpression()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        double real = scanner_.getToken().getRealValue();

        scanner_.getNextToken();

        return new RealAST(loc, real);
    }

    // IntegerLiteral ::= <INTEGER_LITERAL>
    ExprASTPtr Parser::parseIntegerExpression()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        int integer = scanner_.getToken().getIntValue();

        scanner_.getNextToken();

        return new IntegerAST(loc, integer);
    }

    // CharLiteral ::= <CHATLITERAL>
    ExprASTPtr Parser::parseCharExpression()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        char ch = scanner_.getToken().getIntValue();

        scanner_.getNextToken();

        return new CharAST(loc, ch);
    }

    // StringLiteral ::= <STRINGLITERAL>
    ExprASTPtr Parser::parseStringExpression()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        std::string string = scanner_.getToken().getStringValue();

        scanner_.getNextToken();

        return new StringAST(loc, string);
    }

    // TrueLiteral ::= "true"
    // FalseLiteral ::= "false"
    ExprASTPtr Parser::parseBooleanExpression()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        bool boolean = (scanner_.getToken().getTokenName() == "true");

        scanner_.getNextToken();

        return new BooleanAST(loc, boolean);
    }

    // AssignmentStatement ::= Identifier "=" Expression ";"
    // ArrayAssignmentStatement ::= Identifier "[" Expression "]" "=" Expression ";"
    // AndExpression ::= Clause "&&" Clause
    // CompareExpression ::= PrimaryExpression "<" PrimaryExpression
    // PlusExpression ::= PrimaryExpression "+" PrimaryExpression
    // MinusExpression ::= PrimaryExpression "-" PrimaryExpression
    // TimesExpression ::= PrimaryExpression "*" PrimaryExpression
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

    // NotExpression ::= "!" Clause
    // Clause ::= NotExpression | PrimaryExpression
    // MJava have only one unary operator "!", but we can add other unary operators.
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

    // BracketExpression ::= "(" Expression ")"
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

    // Block ::= "{" ( Statement )* "}"
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

            if (currentASTPtr != nullptr)
            {
                if (!validateAST(ASTType::CLASSDECLARATION, currentASTPtr) && !validateAST(ASTType::MAINCLASS, currentASTPtr) && !validateAST(ASTType::METHODDECLARATION, currentASTPtr) && !validateAST(ASTType::VARIABLEDECLARATION, currentASTPtr))
                {
                    stmts.push_back(currentASTPtr);
                }
                else
                {
                    errorReport(currentASTPtr, "Find unexpected " + currentASTPtr->getASTTypeDescription());
                }
            }
        }

        if (!expectToken(TokenValue::RBRACE, "}", true))
        {
            return nullptr;
        }

        return new BlockAST(loc, stmts);
    }

    // IfStatement ::= "if" "(" Expression ")" Statement "else" Statement
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

        ExprASTPtr condition = parseExpression();

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

        if (!expectToken(TokenValue::ELSE, "else", true))
        {
            return nullptr;
        }
        
        // MJava ifstatement must have else part
        ExprASTPtr elsePart = parseBlockOrStatement();

        if (elsePart == nullptr)
        {
            errorReport("else statement is not valid.");
            return nullptr;
        }

        return new IfStatementAST(loc, condition, thenPart, elsePart);
    }

    // WhileStatement ::= "while" "(" Expression ")" Statement
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

    bool Parser::expectAST(ASTType type, const std::string& astName, ExprASTPtr ast)
    {
        if (ast->getID() != type)
        {
            errorSyntax(ast->getTokenLocation().toString() + "Expected ' " + astName + " ', but find " + ast->getASTTypeDescription());
            return false;
        }

        return true;
    }

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

    bool Parser::validateAST(ASTType type, ExprASTPtr ast)
    {
        if (ast->getID() != type)
        {
            return false;
        }

        return true;
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

    void Parser::errorReport(ExprASTPtr ast, const std::string& msg)
    {
        errorSyntax(ast->getTokenLocation().toString() + msg);
    }

} // namespace MJava