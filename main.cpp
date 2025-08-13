#include <iostream>
#include <string>
#include <fstream>
#include "src/lexer/lexer.h"
#include "src/parser/parser.h"
#include "src/parser/astnode.h"
#include "src/interpreter/interpreter.h"

int main(int argc, char const *argv[])
{
    std::string arqName;
    std::string lin;
    
    std::cout << "set the path of the .oliva file (default is current folder): ";
    std::cin >> arqName;
    
    Lexer OlivaLexer;
    Parser OlivaParser;
    Interpreter OlivaInterpreter;
    int currentLine = 0;

    std::vector <Token> tokensLine;
    AstNode* astLine;

    std::ifstream file (arqName);

    if (!file.is_open())
    {
        std::cerr << "Error opening \"" << arqName << "\"";
        exit(1);
    }

    while (getline(file, lin))
    {
        tokensLine = OlivaLexer.Tokenize(lin);
        astLine = OlivaParser.ParseLine(tokensLine, ++currentLine);
        OlivaInterpreter.interpretLine(astLine, currentLine);
    };
    
    file.close();
    system("pause");
    return 0;
}
