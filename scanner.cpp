#include "scanner.h"
#include <iostream>
#include <cstring>   /* strchr */
using namespace std;

/*----------- util: blancos ----------------------------------------------*/
static inline bool is_ws(char c)
{
    return c==' ' || c=='\t' || c=='\n' || c=='\r';
}

/*----------- nextToken ---------------------------------------------------*/
Token* Scanner::nextToken()
{
    /* saltar espacios -------------------------------------------------- */
    while (current < (int)input.size() && is_ws(input[current])) ++current;
    if (current >= (int)input.size()) return new Token(Token::END);

    char c  = input[current];
    first   = current;

    /*---------- número --------------------------------------------------*/
    if (isdigit(c))
    {
        while (current < (int)input.size() && isdigit(input[++current]));
        return new Token(Token::NUM, input, first, current-first);
    }

    /*---------- palabra / identificador --------------------------------*/
    if (isalpha(c))
    {
        while (current < (int)input.size() && isalnum(input[++current]));
        string w = input.substr(first, current-first);

        /* tabla de palabras-clave */
#define KW(str, tk) if (w == str) return new Token(tk, w, 0, w.size());
        KW("program" , Token::PROGRAM)
        KW("var"     , Token::VAR)
        KW("function", Token::FUNCTION)
        KW("begin"   , Token::BEGIN_KW)
        KW("end"     , Token::END_KW)
        KW("writeln" , Token::WRITELN)
        KW("if"      , Token::IF)
        KW("then"    , Token::THEN)
        KW("else"    , Token::ELSE)
        KW("while"   , Token::WHILE)
        KW("do"      , Token::DO)
        KW("for",    Token::FOR)
        KW("to",     Token::TO)
        KW("downto", Token::DOWNTO)
        KW("true"    , Token::TRUE)
        KW("false"   , Token::FALSE)
        KW("return"  , Token::RETURN)
        KW("type"  , Token::TYPE)
        KW("record"  , Token::RECORD)
#undef  KW
        /* no es palabra reservada ⇒ identificador */
        return new Token(Token::ID, w, 0, w.size());
    }

    /*---------- símbolos de un carácter (+-/() etc.) ------------------*/
    if (strchr("+-*/()<>;,.", c) || c == ':' || c == '=' )
    {
        switch (c)
        {
            case '+': return current++, new Token(Token::PLUS , c);
            case '-': return current++, new Token(Token::MINUS, c);
            case '*': return current++, new Token(Token::MUL  , c);
            case '/': return current++, new Token(Token::DIV  , c);
            case '(': return current++, new Token(Token::PI   , c);
            case ')': return current++, new Token(Token::PD   , c);
            case ',': return current++, new Token(Token::COMA , c);
            case ';': return current++, new Token(Token::PC   , c);
            case '.': return current++, new Token(Token::DOT  , c);

            /* comparadores <  <=  */
            case '<':
                if (current+1 < (int)input.size() && input[current+1]=='=')
                { current += 2; return new Token(Token::LE , "<=", 0, 2); }
                current++; return new Token(Token::LT, c);

            /* comparadores > >=  */
            case '>':
                if (current+1 < (int)input.size() && input[current+1]=='=')
                { current += 2; return new Token(Token::GE , "<=", 0, 2); }
            current++; return new Token(Token::GT, c);

            /* == como operador de igualdad (opcional) */
            case '=':

                if (current+1 < (int)input.size() && input[current+1]=='=')
                { current += 2; return new Token(Token::EQ , "==", 0, 2); }
                else { current++; return new Token(Token::ASSIGN, '='); };

            /*  ':' o ':='  */
            case ':':
                if (current+1 < (int)input.size() && input[current+1]=='=')
                { current += 2; return new Token(Token::ASSIGN, ":=", 0, 2); }
                current++; return new Token(Token::COLON,  ':');
        }
    }

    /* carácter desconocido -------------------------------------------- */
    current++;
    return new Token(Token::ERR, c);
}

/*----------- depuración --------------------------------------------------*/
void test_scanner(Scanner* sc)
{
    cout << "--- TOKENS ---\n";
    for (Token* t; (t = sc->nextToken())->type != Token::END; delete t)
        cout << *t << '\n';
    cout << "TOKEN(END)\n";
}