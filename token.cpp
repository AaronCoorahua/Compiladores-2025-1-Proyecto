#include "token.h"
#include <iostream>

using namespace std;

/* --- constructores ----------------------------------------------------- */
Token::Token(Type t)                     : type(t), text("")          {}
Token::Token(Type t, char c)             : type(t), text(1, c)        {}
Token::Token(Type t,const string& src,int first,int len)
    : type(t), text(src.substr(first, len)) {}

/* --- impresión --------------------------------------------------------- */
ostream& operator<<(ostream& outs, const Token& tok)
{
    switch (tok.type)
    {
        case Token::PLUS      : outs << "TOKEN(PLUS)";      break;
        case Token::MINUS     : outs << "TOKEN(MINUS)";     break;
        case Token::MUL       : outs << "TOKEN(MUL)";       break;
        case Token::DIV       : outs << "TOKEN(DIV)";       break;

        case Token::LT        : outs << "TOKEN(LT)";        break;
        case Token::LE        : outs << "TOKEN(LE)";        break;
        case Token::EQ        : outs << "TOKEN(EQ)";        break;

        case Token::PI        : outs << "TOKEN(PI)";        break;
        case Token::PD        : outs << "TOKEN(PD)";        break;
        case Token::COMA      : outs << "TOKEN(COMA)";      break;
        case Token::PC        : outs << "TOKEN(PC)";        break;
        case Token::DOT       : outs << "TOKEN(DOT)";       break;

        case Token::ASSIGN    : outs << "TOKEN(ASSIGN)";    break;
        case Token::COLON     : outs << "TOKEN(COLON)";     break;

        case Token::NUM       : outs << "TOKEN(NUM)";       break;
        case Token::ID        : outs << "TOKEN(ID)";        break;

        case Token::PROGRAM   : outs << "TOKEN(PROGRAM)";   break;
        case Token::VAR       : outs << "TOKEN(VAR)";       break;
        case Token::FUNCTION  : outs << "TOKEN(FUNCTION)";  break;
        case Token::BEGIN_KW  : outs << "TOKEN(BEGIN)";     break;
        case Token::END_KW    : outs << "TOKEN(END)";       break;

        case Token::IF        : outs << "TOKEN(IF)";        break;
        case Token::THEN      : outs << "TOKEN(THEN)";      break;
        case Token::ELSE      : outs << "TOKEN(ELSE)";      break;

        case Token::WHILE     : outs << "TOKEN(WHILE)";     break;
        case Token::DO        : outs << "TOKEN(DO)";        break;

        case Token::FOR       : outs << "TOKEN(FOR)";       break;
        case Token::TO        : outs << "TOKEN(TO)";        break;
        case Token::DOWNTO    : outs << "TOKEN(DOWNTO";     break;

        case Token::WRITELN   : outs << "TOKEN(WRITELN)";   break;
        case Token::RETURN    : outs << "TOKEN(RETURN)";    break;

        case Token::TYPE      : outs << "TOKEN(TYPE)";       break;
        case Token::RECORD      : outs << "TOKEN(RECORD)";       break;


        case Token::TRUE      : outs << "TOKEN(TRUE)";      break;
        case Token::FALSE     : outs << "TOKEN(FALSE)";     break;

        case Token::ERR       : outs << "TOKEN(ERR)";       break;
        case Token::END       : outs << "TOKEN(END)";       break;

        default               : outs << "TOKEN(UNKNOWN)";   break;
    }
    return outs;
}

ostream& operator<<(ostream& outs, const Token* tok)
{
    return outs << *tok;
}
