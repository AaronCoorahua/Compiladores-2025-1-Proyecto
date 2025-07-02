#include "scanner.h"
#include <iostream>
#include <cstring>  
using namespace std;

static inline bool is_ws(char c)
{
    return c==' ' || c=='\t' || c=='\n' || c=='\r';
}

Token* Scanner::nextToken()
{
    while (current < (int)input.size() && is_ws(input[current])) ++current;
    if (current >= (int)input.size()) return new Token(Token::END);

    char c  = input[current];
    first   = current;

    if (isdigit(c)) {
        bool hasDot = false;
        while (current < (int)input.size() && (isdigit(input[current]) || (!hasDot && input[current] == '.'))) {
            if (input[current] == '.') hasDot = true;
            current++;
        }
        if (hasDot)
            return new Token(Token::NUM_FLOAT, input, first, current - first);
        else
            return new Token(Token::NUM, input, first, current - first);
    }

    if (isalpha(c))
    {
        while (current < (int)input.size() && isalnum(input[++current]));
        string w = input.substr(first, current-first);

        if      (w == "program") return new Token(Token::PROGRAM, w, 0, w.size());
        else if (w == "var") return new Token(Token::VAR, w, 0, w.size());
        else if (w == "function") return new Token(Token::FUNCTION, w, 0, w.size());
        else if (w == "begin") return new Token(Token::BEGIN_KW, w, 0, w.size());
        else if (w == "end") return new Token(Token::END_KW, w, 0, w.size());
        else if (w == "writeln") return new Token(Token::WRITELN,  w, 0, w.size());
        else if (w == "if") return new Token(Token::IF, w, 0, w.size());
        else if (w == "then") return new Token(Token::THEN, w, 0, w.size());
        else if (w == "else") return new Token(Token::ELSE, w, 0, w.size());
        else if (w == "while") return new Token(Token::WHILE, w, 0, w.size());
        else if (w == "do") return new Token(Token::DO, w, 0, w.size());
        else if (w == "for") return new Token(Token::FOR, w, 0, w.size());
        else if (w == "to") return new Token(Token::TO, w, 0, w.size());
        else if (w == "downto") return new Token(Token::DOWNTO, w, 0, w.size());
        else if (w == "true") return new Token(Token::TRUE, w, 0, w.size());
        else if (w == "false") return new Token(Token::FALSE, w, 0, w.size());
        else if (w == "return") return new Token(Token::RETURN, w, 0, w.size());
        else if (w == "type") return new Token(Token::TYPE, w, 0, w.size());
        else if (w == "record") return new Token(Token::RECORD, w, 0, w.size());
        else return new Token(Token::ID, w, 0, w.size());

    }

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

            case '>':
                if (current+1 < (int)input.size() && input[current+1]=='=')
                { current += 2; return new Token(Token::GE , "<=", 0, 2); }
            current++; return new Token(Token::GT, c);

            case '=':

                if (current+1 < (int)input.size() && input[current+1]=='=')
                { current += 2; return new Token(Token::EQ , "==", 0, 2); }
                else { current++; return new Token(Token::ASSIGN, '='); };

            case ':':
                if (current+1 < (int)input.size() && input[current+1]=='=')
                { current += 2; return new Token(Token::ASSIGN, ":=", 0, 2); }
                current++; return new Token(Token::COLON,  ':');
        }
    }

    current++;
    return new Token(Token::ERR, c);
}

void test_scanner(Scanner* sc)
{
    cout << "--- TOKENS ---\n";
    for (Token* t; (t = sc->nextToken())->type != Token::END; delete t)
        cout << *t << '\n';
    cout << "TOKEN(END)\n";
}