#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include "token.h"


class Scanner {
    std::string input;     
    int first{0}, current{0};

public:
    explicit Scanner(const char* txt) : input(txt) {}

    Token* nextToken();     
    void reset() { first = current = 0; }

    ~Scanner() = default;
};

void test_scanner(Scanner*);

#endif 