#pragma once
#include "../parser/astnode.h"
#include <unordered_map>
#include <string>

#ifndef OLI_INTERPRETER
#define OLI_INTERPRETER


class Interpreter
{
    public:
        std::unordered_map <std::string, std::string> variables;
        void interpretLine (AstNode* LineAstTree, int ln);
        std::string evalue (AstNode* node);
        bool condition (AstNode* node);
};

#endif