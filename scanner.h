#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include "token.h"


/* Analizador léxico muy simple para Pascal reducido */
class Scanner {
    std::string input;       /* código fuente completo              */
    int first{0}, current{0};

public:
    explicit Scanner(const char* txt) : input(txt) {}

    Token* nextToken();      /* obtiene y devuelve el siguiente token */
    void   reset()           { first = current = 0; }

    /* destructor por defecto: no hay recursos dinámicos */
    ~Scanner() = default;
};

/* rutina de depuración opcional */
void test_scanner(Scanner*);

#endif /* SCANNER_H */