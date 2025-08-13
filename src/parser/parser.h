#pragma once
#include <vector>
#include "astnode.h"
#include "../lexer/lexer.h"

#ifndef OLI_PARSER
#define OLI_PARSER
    class Parser
    {
        public:
            AstNode* ParseLine (std::vector<Token> Tokens, int currentLine);
            AstNode* ParseExpression (std::vector<Token>, int currentLine, int& i);
            AstNode* ParseAssignment (std::vector<Token>, int currentLine, int& i);
            AstNode* ParseCondition (std::vector<Token> Tokens, int currentLine, int& i);
    };
#endif