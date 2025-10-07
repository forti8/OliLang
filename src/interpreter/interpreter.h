// include only once
#pragma once

// libs
#include "../parser/astnode.h"
#include <unordered_map>
#include <string>

// if define OLI_INTERPRETER
#ifndef OLI_INTERPRETER
#define OLI_INTERPRETER

// Interpreter class
class Interpreter {

    // public acess (outside class define)
    public:

        // variables map (unordered)
        std::unordered_map <std::string, std::string> variables;

        // interpret a single astnode line
        void interpretLine (AstNode* LineAstTree, int ln);

        // evalue node
        std::string evalue (AstNode* node);

        // define condition boolean result
        bool condition (AstNode* node);
};

#endif