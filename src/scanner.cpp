// THIS FILE IS PART OF MJava-Compiler PROJECT
// scanner.cpp - scanner
 
// Created by Li Taiji 2020-03-18
// Copyright (c) 2020 Li Taiji All rights reserved

#include "error.h"
#include "scanner.h"
#include <algorithm>
#include <cctype>
#include <stdexcept>

namespace MJava
{
    bool Scanner::errorFlag_ = false;

    Scanner::Scanner(const std::string& srcFileName)
        : fileName_(srcFileName), line_(1), column_(0),
          currentChar_(0), state_(State::NONE)
    {
        input_.open(fileName_);

        if (input_.fail())
        {
            errorReport("When trying to open file " + fileName_ + ", occurred error.");
        }
    }

    void Scanner::getNextChar()
    {
        currentChar_ = input_.get();

        // record the location of token
        if (currentChar_ == '\n')
        {
            line_++;
            column_ = 0;
        }
        else
        {
            column_++;
        }
    }

    char Scanner::peekChar()
    {
        char c = input_.peek();
        return c;
    }

    void Scanner::addToBuffer(char c)
    {
        buffer_.push_back(c);
    }


    void Scanner::reduceBuffer()
    {
        buffer_.pop_back();
    }



    void Scanner::makeToken(TokenType tt, TokenValue tv,
                            const TokenLocation& loc, const std::string& name, int symbolPrecedence)
    {
        token_ = Token(tt, tv, loc, name, symbolPrecedence);
        buffer_.clear();
        state_ = State::NONE;
    }

    void Scanner::makeToken(TokenType tt, TokenValue tv,
                            const TokenLocation& loc, int intValue, const std::string& name)
    {
        token_ = Token(tt, tv, loc, intValue, name);
        buffer_.clear();
        state_ = State::NONE;
    }

    void Scanner::makeToken(TokenType tt, TokenValue tv,
                            const TokenLocation& loc, double realValue, const std::string& name)
    {
        token_ = Token(tt, tv, loc, realValue, name);
        buffer_.clear();
        state_ = State::NONE;
    }

    void Scanner::makeToken(TokenType tt, TokenValue tv,
                            const TokenLocation& loc, const std::string& strValue, const std::string& name)
    {
        token_ = Token(tt, tv, loc, strValue, name);
        buffer_.clear();
        state_ = State::NONE;
    }


    void Scanner::preprocess()
    {
        do
        {
            // eat spaces
            while (std::isspace(currentChar_))
            {
                getNextChar();
            }

            handleLineComment();
            handleBlockComment();
        } while (std::isspace(currentChar_) || currentChar_ == '/'); // eat spaces and comment
    }

    void Scanner::handleLineComment()
    {
        loc_ = getTokenLocation();

        if (currentChar_ == '/' && peekChar() == '/')
        {
            // currenChar is / and eat /, update currentChar_ to /
            getNextChar();
            // currentChar is / and eat it, update currentChar_ to the next char.
            getNextChar();

            while (currentChar_ != '\n' && !input_.eof())
            {
                // skip comment content
                getNextChar();
            }

            if (!input_.eof())
            {
                // skip '\n'
                getNextChar();
            }
        }
    }

    void Scanner::handleBlockComment()
    {
        loc_ = getTokenLocation();

        if (currentChar_ == '/' && peekChar() == '*')
        {
            // currenChar is / and eat /, update currentChar_ to *
            getNextChar();
            // currentChar is * and eat it, update currentChar_ to the next char.
            getNextChar();

            while (!(currentChar_ == '*' && peekChar() == '/'))
            {
                // skip comment content
                getNextChar();

                // accident EOF
                if (input_.eof())
                {
                    errorReport(std::string("end of file happended in comment, */ is expected!, but find ") + currentChar_);
                    break;
                }
            }

            if (!input_.eof())
            {
                // eat * and update currentChar_ to /
                getNextChar();
                // eat / and update currentChar_ to the next Char
                getNextChar();
            }
        }
    }

    Token Scanner::getNextToken()
    {
        bool matched = false;

        do
        {
            // restore error flag at each time invoke getNextToken()
            Scanner::errorFlag_ = false;

            if (state_ != State::NONE)
            {
                matched = true;
            }

            switch (state_)
            {
                case State::NONE:
                    getNextChar();
                    break;

                case State::END_OF_FILE:
                    handleEOFState();
                    break;

                case State::IDENTIFIER:
                    handleIdentifierState();
                    break;

                case State::NUMBER:
                    handleNumberState();
                    break;

                case State::SINGLE_CHAR:
                    handleCharState();
                    break;

                case State::STRING:
                    handleStringState();
                    break;

                case State::OPERATION:
                    handleOperationState();
                    break;

                default:
                    errorReport("Match token state error.");
                    break;
            }

            if (state_ == State::NONE)
            {
                preprocess();

                if (input_.eof())
                {
                    state_ = State::END_OF_FILE;
                }
                else
                {
                    if (std::isalpha(currentChar_))
                    {
                        state_ = State::IDENTIFIER;
                    }
                    // if it is digit, xdigit or odigit
                    else if (std::isdigit(currentChar_))
                    {
                        state_ = State::NUMBER;
                    }
                    else if (currentChar_ == '\'')
                    {
                        state_ = State::SINGLE_CHAR;
                    }
                    else if (currentChar_ == '\"')
                    {
                        state_ = State::STRING;
                    }
                    else
                    {
                        state_ = State::OPERATION;
                    }
                }
            }
        } while (!matched || getErrorFlag());

        return token_;
    }

    void Scanner::handleEOFState()
    {
        loc_ = getTokenLocation();
        makeToken(TokenType::END_OF_FILE, TokenValue::UNRESERVED,
                  loc_, std::string("END_OF_FILE"), -1);
        // close the file
        input_.close();
    }


    void Scanner::handleNumberState()
    {
        loc_ = getTokenLocation();
        bool isFloat = false;
        bool isExponent = false;
        int numberBase = 10;

        if (currentChar_ == '0' && (peekChar() == 'x' || peekChar() == 'X'))
        {
            numberBase = 16;

            // eat 0 and update currentChar_
            getNextChar();

            // eat x or X and update currentChar_
            getNextChar();
        }

        if (currentChar_ == '0' && peekChar() >= '0' && peekChar() <= '7')
        {
            numberBase = 8;

            // eat 0 and update currentChar_
            getNextChar();
        }

        enum class NumberState
        {
            INTERGER,
            FRACTION,
            EXPONENT,
            DONE
        };

        NumberState numberState = NumberState::INTERGER;

        do
        {
            switch (numberState)
            {
                case NumberState::INTERGER:
                    if (numberBase == 10)
                    {
                        handleDigit();
                    }
                    else if (numberBase == 16)
                    {
                        handleXDigit();
                    }
                    else if (numberBase == 8)
                    {
                        handleODigit();
                    }
                    break;

                case NumberState::FRACTION:
                    handleFraction();
                    isFloat = true;
                    break;

                case NumberState::EXPONENT:
                    handleExponent();
                    isExponent = true;
                    break;

                case NumberState::DONE:
                    break;

                default:
                    errorReport("Match number state error.");
                    break;
            }

            // change number state
            if (currentChar_ == '.')
            {
                // first time, isFloat is false.
                // But maybe enter float number state again, such as 12.4.5
                // In fact, we can do it also in the parse phase. I am thinking
                // about do it in the scanner phase or the parse phase. But now
                // I do it in the scanner phase.
                if (isFloat)
                {
                    errorReport("Fraction number can not have more than one dot.");
                }

                if (isExponent)
                {
                    errorReport("Scientist number representation in MJava can not have dot.");
                }

                if (numberBase == 16)
                {
                    errorReport("Hexadecimal number in MJava can only be integer.");
                }
                
                if (numberBase == 8)
                {
                    errorReport("Octal number in MJava can only be integer.");
                }

                numberState = NumberState::FRACTION;
            }
            else if (currentChar_ == 'E' || currentChar_ == 'e')
            {
                if (isExponent)
                {
                    errorReport("Scientist presentation can not have more than one e / E");
                }

                numberState = NumberState::EXPONENT;
            }
            else
            {
                numberState = NumberState::DONE;
            }
        } while (numberState != NumberState::DONE);

        if (!getErrorFlag())
        {
            if (isFloat || isExponent)
            {
                try
                {
                    makeToken(TokenType::REAL, TokenValue::UNRESERVED, loc_,
                        std::stod(buffer_), buffer_);
                }
                catch(std::out_of_range& e)
                {
                    errorReport("Floating-point number literal: " + buffer_ + " is outside the range of the \"double\".");
                    buffer_.clear();
                    state_ = State::NONE;
                }
                catch(std::invalid_argument& e)
                {
                    errorReport("Floating-point number literal: " + buffer_ + " can not be converted to the \"double\".");
                    buffer_.clear();
                    state_ = State::NONE;
                }    
            }
            else
            {
                try
                {
                    makeToken(TokenType::INTEGER, TokenValue::UNRESERVED, loc_,
                        std::stoi(buffer_, nullptr, numberBase), buffer_);
                }
                catch(std::out_of_range& e)
                {
                    errorReport("Integer literal: " + buffer_ + " is outside the range of the \"int\".");
                    buffer_.clear();
                    state_ = State::NONE;
                }
                catch(std::invalid_argument& e)
                {
                    errorReport("Integer literal: " + buffer_ + " can not be converted to the \"int\".");
                    buffer_.clear();
                    state_ = State::NONE;
                }               
            }
        }
        else
        {
            // just clear buffer_ and set the state to State::NONE
            buffer_.clear();
            state_ = State::NONE;
        }
    }

    void Scanner::handleCharState()
    {
        loc_ = getTokenLocation();
        // eat ' and NOT update currentChar_
        // because we don't want ' (single quote).
        getNextChar();

        while (true)
        {
            // skip escape character \'
            if (currentChar_ != '\\' && peekChar() == '\'')
            {
                addToBuffer(currentChar_);
                break;
            }

            if (input_.eof())
            {
                errorReport(std::string("end of file happended in string, \' is expected!, but find ") + currentChar_);
                break;
            }

            addToBuffer(currentChar_);
            getNextChar();
        }

        if (!input_.eof())
        {
            // eat end ' and update currentChar_ .
            getNextChar();
            getNextChar();
        }

        // just one char
        if (!getErrorFlag() && buffer_.length() == 1)
        {
            makeToken(TokenType::CHAR_LITERAL, TokenValue::UNRESERVED, loc_,
                      static_cast<int>(buffer_.at(0)), buffer_);
        }
        else
        {
            errorReport("Char can contain only one character!");
            // just clear buffer_ and set the state to State::NONE
            buffer_.clear();
            state_ = State::NONE;
        }
    }

    void Scanner::handleStringState()
    {
        loc_ = getTokenLocation();
        // eat " and NOT update currentChar_
        // because we don't want " (double quote).
        getNextChar();

        while (true)
        {
            // skip escape character \"
            if (currentChar_ != '\\' && peekChar() == '\"')
            {
                addToBuffer(currentChar_);
                break;
            }

            if (input_.eof())
            {
                errorReport(std::string("end of file happended in string, \" is expected!, but find ") + currentChar_);
                break;
            }

            addToBuffer(currentChar_);
            getNextChar();
        }

        if (!input_.eof())
        {
            // eat end " and update currentChar_ .
            getNextChar();
            getNextChar();
        }

        if (!getErrorFlag())
        {
            makeToken(TokenType::STRING_LITERAL, TokenValue::UNRESERVED,
                    loc_, buffer_, buffer_); 
        }
        else
        {
            // just clear buffer_ and set the state to State::NONE
            buffer_.clear();
            state_ = State::NONE;
        }
    }

    void Scanner::handleIdentifierState()
    {
        loc_ = getTokenLocation();
        // add first char
        addToBuffer(currentChar_);
        getNextChar();

        while (std::isalnum(currentChar_) || currentChar_ == '_')
        {
            addToBuffer(currentChar_);
            getNextChar();
        }
        // end while. currentChar_ is not alpha, number and _.

        std::string copy = buffer_;
        // match "System.out.println"
        if (buffer_ == "System")
        {
            // length of "System.out.println" from the first '.' to the end
            int length = 12;
            // set current location of input stream.
            std::streampos index = input_.tellg();

            while (length > 0)
            {
                addToBuffer(currentChar_);
                getNextChar();
                --length;
            }

            // if does not match "System.out.println", roll back to the original location.
            if (buffer_ != "System.out.println")
            {
                buffer_ = copy;
                input_.seekg(index);
            }
        }

        // use dictionary to judge it is keyword or not
        auto tokenMeta = dictionary_.lookup(buffer_);
        makeToken(std::get<0>(tokenMeta), std::get<1>(tokenMeta), loc_, buffer_, std::get<2>(tokenMeta));
    }

    void Scanner::handleOperationState()
    {
        loc_ = getTokenLocation();
        // add current symbol char
        addToBuffer(currentChar_);
        // add next one symbol char
        addToBuffer(peekChar());

        if (dictionary_.haveToken(buffer_))
        {
            getNextChar();
        }
        else
        {
            reduceBuffer();
        }

        auto tokenMeta = dictionary_.lookup(buffer_);
        // token type, token value, name, symbol precedence
        makeToken(std::get<0>(tokenMeta), std::get<1>(tokenMeta), loc_, buffer_, std::get<2>(tokenMeta));
        // update currentChar_
        getNextChar();
    }

    void Scanner::handleDigit()
    {
        // add first number of integer
        addToBuffer(currentChar_);
        getNextChar();

        while (std::isdigit(currentChar_))
        {
            addToBuffer(currentChar_);
            getNextChar();
        }
        // end while. currentChar_ is not digit.

        // notice maybe currentChar_ is .(dot) or E/e,
        // so the NumberState can be changed into
        // NumberState::Fraction or NumberState::Exponent.
    }

    void Scanner::handleXDigit()
    {
        // notice: we have eat "0x" and update currentChar_
        // in the handleNumber function. so we need not
        // eat currentChar_ like handleDigit function.
        // only have "0x" or not
        bool readFlag = false;

        while (std::isxdigit(currentChar_))
        {
            readFlag = true;
            addToBuffer(currentChar_);
            getNextChar();
        }

        if (!readFlag)
        {
            errorReport("Hexadecimal number format error.");
        }
    }

    void Scanner::handleODigit()
    {
        // notice: we have eat "0" and update currentChar_
        // in the handleNumber function. so we need not
        // eat currentChar_ like handleDigit function.
        // only have "0" or not
        bool readFlag = false;

        while (currentChar_ >= '0' && currentChar_ <= '7')
        {
            readFlag = true;
            addToBuffer(currentChar_);
            getNextChar();
        }

        if (!readFlag)
        {
            errorReport("Octal number format error.");
        }
    }

    void Scanner::handleFraction()
    {
        // currentChar_ is . (dot)
        /*
            unsigned-real = digit-sequence '.' fractional-part [ 'e' scale-factor ]
                             | digit-sequence 'e' scale-factor
            unsigned-integer = digit-sequence
            fractional-part = digit-sequence
            scale-factor = [ sign ] digit-sequence
            digit-sequence = digit {digit}
        */
        if (!std::isdigit(peekChar()))
        {
            errorReport("Fraction number part should be numbers");
        }

        // eat .
        addToBuffer(currentChar_);
        getNextChar();

        while (std::isdigit(currentChar_))
        {
            addToBuffer(currentChar_);
            getNextChar();
        }
    }

    void Scanner::handleExponent()
    {
        // eat E/e
        addToBuffer(currentChar_);
        getNextChar();

        // next char will be [sign] | digital-sequence
        if (currentChar_ != '+' && currentChar_ != '-' && !std::isdigit(currentChar_))
        {
            errorReport(std::string("Scientist presentation number after e / E should be + / - or digits but find ") + '\'' + currentChar_ + '\'');
        }

        // if number has +/-
        if (currentChar_ == '+' || currentChar_ == '-')
        {
            addToBuffer(currentChar_);
            getNextChar();
        }

        // next will only be digits
        while (std::isdigit(currentChar_))
        {
            addToBuffer(currentChar_);
            getNextChar();
        }
    }

    void Scanner::errorReport(const std::string& msg)
    {
        errorToken(getTokenLocation().toString() + msg);
    }

    void Scanner::setErrorFlag(bool flag)
    {
        errorFlag_ = flag;
    }
} // namespace MJava