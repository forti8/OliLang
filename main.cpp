// libs
#include <iostream>
#include <string>
#include <fstream>

// includes language analysis libraries
#include "src/lexer/lexer.h"
#include "src/parser/parser.h"
#include "src/parser/astnode.h"
#include "src/interpreter/interpreter.h"

void defineConsoleColor (int c, std::string txt) {
    std::cout << "\033[0;" << c << "m" << txt << "\033[0m";
    return;
}

// main function
int main(int argc, char const *argv[]) {

    // argc defines how many arguments were used by default 1
    if (argc > 1) {

        // prefix of each program argument/parameter (argv). It is not necessary but it is convenient
        std::string prefix = "--";
        
        // run command
        if (argv[1] == prefix + "run") {

            // variables for file access
            std::string arqName;
            std::string lin;
            
            // ensures that the argument referring to the path of the .oli file is set
            if (argc > 2) {

                // passes the value of argv which is the absolute path of the file to be read
                arqName = argv[2];

                // each language analysis process is a class
                // being stored in a variable to be used for its functions
                Lexer OlivaLexer;
                Parser OlivaParser;
                Interpreter OlivaInterpreter;

                // current line to report errors
                int currentLine = 0;

                // stores the return of each step to correctly follow the interpretation flow
                std::vector <Token> tokens;
                AstNode* ast;

                // just "finds" the file by its path
                std::ifstream file (arqName);

                // informs if opening error occurs
                if (!file.is_open()) {
                    defineConsoleColor(31, "Error opening \"" + arqName + "\"\a");
                    exit(1);
                }

                // file read loop
                while (getline(file, lin)) {
                    
                    // just for the parser to separate correctly
                    int i = 0;
                    
                    tokens = OlivaLexer.Tokenize(lin);
                    ast = OlivaParser.ParseLine(tokens, ++currentLine, i);
                    OlivaInterpreter.interpretLine(ast, currentLine);
                };

                // closes the file after interpretation
                file.close();
            }

            else {
                defineConsoleColor(31, "the file path was not declared\n\a");
            }
        }

        // --version
        else if (argv[1] == prefix + "version") {
            defineConsoleColor(36, "Oliva version 0.0.0.1 (Earendel)\n");
            defineConsoleColor(36, "Oliva Parser version 0.0.0.1\n");
            defineConsoleColor(36, "Oliva Runtime version 0.0.0.1\n");
        }

        // --help
        else if (argv[1] == prefix + "help") {
            defineConsoleColor(37, "List of available commands\n");
            defineConsoleColor(37, "--run executes an oli file\n");
            defineConsoleColor(37, "--version displays the language version being used\n");
            defineConsoleColor(37, "--help displays a list of all available commands\n");
        }
    }

    system("pause");
    return 0;
}
