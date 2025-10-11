// libs
#include "lexer.h"
#include "../parser/astnode.h"
#include <vector>
#include <unordered_set>
#include <string>
#include <cctype>
#include <iostream>

// tokenizer takes the line and transforms each sentence into a token
// @type : NodeType
// @value : std::string
// @col : integer
std::vector<Token> Lexer::Tokenize(std::string& line) {

    // vector containing the tokens after passing through the tokenizer
    std::vector<Token> tokens;

    // the previous token
    Token Previous;

    // i is used to go through the entire string of the line
    // to separate it into tokens
    size_t i = 0;

    // col is used to report errors in column X defined as a positive integer
    // col belongs to the set of natural numbers
    int col = 1;

    /// one-line read loop
    while (i < line.size()) {

        // comment line
        if (line[i] == '#') break;

        // identify if the character is a space
        if (isspace(line[i])) {

            // if it is we skip it we add the increment of 1 to the column
            i++;
            col++;

            // the continue command forces the loop to go to its next iteration
            continue;
        }

        // if the character is a letter or an underscore 
        // it is identified as a word
        else if (isalpha(line[i]) || line[i] == '_') {

            // variable that will store this word
            std::string word;

            // loop to see if the character is still contained in a word
            // which can be an alphanumeric character and underscore
            while (i < line.size() && ( isalnum(line[i]) || line[i] == '_')) {
                word.push_back(line[i++]);
                col++;
            }

            // This control string is used to identify 
            // whether or not a word is a language command
            std::unordered_set<std::string> commands = {"print", "scan", "if", "else"};

            // If it is contained in the control string, the word is a command
            // being passed as a type token: NodeType::Command
            if (commands.find(word) != commands.end()) {
                tokens.push_back({NodeType::command, word, col});
                Previous = {NodeType::command, word, col};
            }

            // if it's not just an identifier
            else {
                tokens.push_back({NodeType::ident, word, col});
                Previous = {NodeType::ident, word, col};
            }

            continue;
        }

        // variable definition character
        else if (line[i] == '$') {
            
            // skip the character
            i++;

            // store variable name
            std::string varname;

            while (i < line.size() && (isalnum(line[i]) || line[i] == '_')) {
                varname.push_back(line[i++]);
                col++;
            }
            
            tokens.push_back({NodeType::varset, varname, col});
            Previous = {NodeType::varset, varname, col};
        }

        // identify a string
        else if (line[i] == '"') {

            // string buffer
            std::string str;
            i++;

            // receive full string
            while (i < line.size() && line[i] != '"') {
                str.push_back(line[i]);

                i++;
                col++;
            }

            i++; 
            col++;

            tokens.push_back({NodeType::string, str, col});
            Previous = {NodeType::string, str, col};
            continue;
        }

        // checks if it is a number
        else if (isdigit(line[i])) {

            // find the full number
            std::string fullNumber;
            bool hasDot = false;

            // analyzes if it is still a number 
            // if it is and has another point just for the while loop
            while (i < line.size() && (isdigit(line[i]) || (!hasDot && line[i] == '.'))) {

                // if a point is found then set hasDot to true
                if (line[i] == '.') hasDot = true;

                // appends the current number to the full number
                fullNumber.push_back(line[i++]);
                col++;
            }

            // if it has a floating point
            if (hasDot) {
                tokens.push_back({NodeType::float_OLI, fullNumber, col});
                Previous = {NodeType::float_OLI, fullNumber, col};
            }

            // if it is not an integer
            else {
                tokens.push_back({NodeType::integer_OLI, fullNumber, col});
                Previous = {NodeType::integer_OLI, fullNumber, col};
            }


            continue;
        }

        else if (line[i] == '!') {

            if (i + 1 < line.size() && line[i + 1] == '=') {

                tokens.push_back({NodeType::diff, "!=", col});
                Previous = {NodeType::diff, "!=", col};
                i += 2;
                col += 2;
                continue;
            } 
            
            else {

                i++;
                col++;
                continue;
            }
        }


        else if (line[i] == '=') {

            if (i + 1 < line.size() && line[i + 1] == '=') {

                tokens.push_back({NodeType::equal, "==", col});
                Previous = {NodeType::equal, "==", col};
                i += 2;
                col += 2;
                continue;
            } 
            
            else {

                tokens.push_back({NodeType::atrib, "=", col});
                Previous = {NodeType::atrib, "=", col};
                i++;
                col++;
                continue;
            }
        }


        else if (line[i] == '(') {
            tokens.push_back({NodeType::paropen, "(", col});
            Previous = {NodeType::paropen, "(", col};
        }

        else if (line[i] == ')') {
            tokens.push_back({NodeType::parclose, ")", col});
            Previous = {NodeType::parclose, ")", col};
        }

        else if (line[i] == '{') {
            tokens.push_back({NodeType::curlyopen, "{", col});
            Previous = {NodeType::curlyopen, "{", col};
        }

        else if (line[i] == '}') {
            tokens.push_back({NodeType::curlyclose, "}", col});
            Previous = {NodeType::curlyclose, "}", col};
        }

        else if (line[i] == '+') {
            tokens.push_back({NodeType::add, "+", col});
            Previous = {NodeType::add, "+", col};
        }

        else if (line[i] == '-') {
            tokens.push_back({NodeType::sub, "-", col});
            Previous = {NodeType::sub, "-", col};
        }

        else if (line[i] == '*') {
            tokens.push_back({NodeType::mul, "*", col});
            Previous = {NodeType::mul, "*", col};
        }

        else if (line[i] == '/') {
            tokens.push_back({NodeType::div_OLI, "/", col});
            Previous = {NodeType::div_OLI, "/", col};
        }

        else if (line[i] == '<') {
            tokens.push_back({NodeType::less, "<", col});
            Previous = {NodeType::less, "<", col};
        }

        else if (line[i] == '>') {
            tokens.push_back({NodeType::more, ">", col});
            Previous = {NodeType::more, ">", col};
        }
        

        i++;
        col++;
    }

    return tokens;
}
