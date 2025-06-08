#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class Token {
public:
    enum Type {
        /* símbolos y operadores */
        PLUS, MINUS, MUL, DIV,
        LT, LE, EQ, GT, GE,
        PI, PD, COMA, PC, DOT,
        ASSIGN, COLON,

        /* literales e identificadores */
        NUM, ID,

        /* palabras clave */
        PROGRAM, VAR, FUNCTION,
        BEGIN_KW, END_KW,
        IF, THEN, ELSE,
        WHILE, DO,
        FOR, IN, RANGE,
        WRITELN,
        RETURN,
        TRUE, FALSE,

        /* control */
        ERR, END
    };

    Type        type;
    std::string text;

    Token(Type t);                                   // vacía
    Token(Type t, char c);                           // un solo char
    Token(Type t, const std::string& src,
          int first, int len);                       // sub-cadena
};

/* para imprimir tokens */
std::ostream& operator<<(std::ostream&, const Token&);
std::ostream& operator<<(std::ostream&, const Token*);

#endif  /* TOKEN_H */
