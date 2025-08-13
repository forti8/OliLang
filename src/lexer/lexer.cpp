#include "lexer.h"
#include "../parser/astnode.h"
#include <vector>
#include <string>
#include <cctype>
#include <iostream>

std::vector<Token> Lexer::Tokenize(std::string& line)
{
    std::vector<Token> tokens;
    Token Previous;

    size_t i = 0;
    int col = 1;
    while (i < line.size())
    {
        if (line[i] == '#') break;

        if (isspace(line[i]))
        {
            i++;
            col++;
            continue;
        }

        else if (isalpha(line[i]) || line[i] == '_')
        {
            std::string word;
            while (i < line.size() && ( isalnum(line[i]) || line[i] == '_'))
            {
                word.push_back(line[i++]);
                col++;
            }

            if (word == "print")
            {
                tokens.push_back({NodeType::command, word, col});
                Previous = {NodeType::command, word, col};
            }

            else if (word == "scan")
            {
                tokens.push_back({NodeType::command, word, col});
                Previous = {NodeType::command, word, col};
            }

            else if (word == "if")
            {
                tokens.push_back({NodeType::command, word, col});
                Previous = {NodeType::command, word, col};
            }

            else if (word == "else")
            {
                tokens.push_back({NodeType::command, word, col});
                Previous = {NodeType::command, word, col};
            }

            else if (Previous.Type == NodeType::varset)
            {
                tokens.push_back({NodeType::varname, word, col});
                Previous = {NodeType::varname, word, col};
            }
            else
            {
                tokens.push_back({NodeType::ident, word, col});
                Previous = {NodeType::ident, word, col};
            }

            continue;
        }

        else if (line[i] == '$')
        {
            tokens.push_back({NodeType::varset, "$", col});
            Previous = {NodeType::varset, "$", col};
        }

        else if (line[i] == '(')
        {
            tokens.push_back({NodeType::paropen, "(", col});
            Previous = {NodeType::paropen, "(", col};
        }

        else if (line[i] == ')')
        {
            tokens.push_back({NodeType::parclose, ")", col});
            Previous = {NodeType::parclose, ")", col};
        }

        else if (line[i] == '"')
        {
            std::string str;
            i++;
            while (i < line.size() && line[i] != '"')
            {
                str.push_back(line[i++]);
                col++;
            }

            i++; 
            col++;
            tokens.push_back({NodeType::string, str, col});
            Previous = {NodeType::string, str, col};
            continue;
        }

        else if (line[i] == '=')
        {
            if (line[++i] == '=')
            {
                tokens.push_back({NodeType::equal, "==", col});
                Previous = {NodeType::equal, "==", col};
            }

            else
            {
                tokens.push_back({NodeType::atrib, "=", col});
                Previous = {NodeType::atrib, "=", col};
                i--;
            }
        }

        else if (isdigit(line[i]))
        {
            std::string fullNumber;
            bool hasDot = false;

            while (i < line.size() && (isdigit(line[i]) || (!hasDot && line[i] == '.')))
            {
                if (line[i] == '.') hasDot = true;
                fullNumber.push_back(line[i++]);
                col++;
            }

            if (hasDot)
            {
                tokens.push_back({NodeType::float_OLI, fullNumber, col});
                Previous = {NodeType::float_OLI, fullNumber, col};
            }

            else 
            {
                tokens.push_back({NodeType::integer_OLI, fullNumber, col});
                Previous = {NodeType::integer_OLI, fullNumber, col};
            }
            continue;
        }

        else if (line[i] == '+')
        {
            tokens.push_back({NodeType::add, "+", col});
            Previous = {NodeType::add, "+", col};
        }
        else if (line[i] == '-')
        {
            tokens.push_back({NodeType::sub, "-", col});
            Previous = {NodeType::sub, "-", col};
        }
        else if (line[i] == '*')
        {
            tokens.push_back({NodeType::mul, "*", col});
            Previous = {NodeType::mul, "*", col};
        }
        else if (line[i] == '/')
        {
            tokens.push_back({NodeType::div_OLI, "/", col});
            Previous = {NodeType::div_OLI, "/", col};
        }
        else if (line[i] == '<')
        {
            tokens.push_back({NodeType::less, "<", col});
            Previous = {NodeType::less, "<", col};
        }
        else if (line[i] == '>')
        {
            tokens.push_back({NodeType::more, ">", col});
            Previous = {NodeType::more, ">", col};
        }
        

        i++;
        col++;
    }

    return tokens;
}
