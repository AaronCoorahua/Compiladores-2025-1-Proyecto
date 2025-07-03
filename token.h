#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class Token {
public:
    enum Type {
        // símbolos y operadores
        PLUS, 
        MINUS, 
        MUL, 
        DIV,
        LT, 
        LE, 
        EQ, 
        GT, 
        GE,
        PI, 
        PD, 
        COMA, 
        PC, 
        DOT,
        ASSIGN, 
        COLON,
        // literales e identificadores
        NUM, 
        ID,

        // FLOATS
        NUM_FLOAT,

        // palabras clave
        PROGRAM, 
        VAR, 
        FUNCTION,
        BEGIN_KW, 
        END_KW,
        IF, 
        THEN, 
        ELSE,
        WHILE, 
        DO,
        FOR, 
        TO, 
        DOWNTO,
        WRITELN,
        RETURN,
        TRUE, 
        FALSE,

        // STRUCT
        TYPE,
        RECORD,

        ERR, 
        END
    };

    Type        type;
    std::string text;

    Token(Type t);                                  
    Token(Type t, char c);                          
    Token(Type t, const std::string& src,
          int first, int len);                   
};

std::ostream& operator<<(std::ostream&, const Token&);
std::ostream& operator<<(std::ostream&, const Token*);

#endif  
