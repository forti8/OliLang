#pragma once
#include <string>
#include <vector>
#include "../parser/astnode.h"

#ifndef OLI_LEXIC
#define OLI_LEXIC

struct Token {
    NodeType Type;
    std::string Value;
    int col;
};

class Lexer {
    public:
        std::vector<Token> Tokenize (std::string& Line);
};

#endif