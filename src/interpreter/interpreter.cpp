#include "interpreter.h"
#include "../parser/astnode.h"
#include <iostream>
#include <cstdlib>

void Interpreter::interpretLine(AstNode* node, int ln) {
    if (!node) return;

    if (node->Type == NodeType::command) {
        if (node->Value == "print") {
            AstNode* arg = node->Children[0];
            if (arg->Type == NodeType::string || arg->Type == NodeType::integer_OLI || arg->Type == NodeType::float_OLI) {
                std::cout << arg->Value;
            } else if (arg->Type == NodeType::ident) {
                if (variables.find(arg->Value) != variables.end()) {
                    std::cout << variables[arg->Value];
                } else {
                    std::cerr << "\nError: Variable '" << arg->Value << "' not declared (" << ln << ";" << 0 << ")\n";
                }
            }
        }

        else if (node->Value == "scan") {
            AstNode* arg = node->Children[0];
            if (arg->Type == NodeType::ident) {
                if (variables.find(arg->Value) != variables.end()) {
                    std::cin >> variables[arg->Value];
                }

                else {
                    std::cerr << "\nError: Variable '" << arg->Value << "' not declared (" << ln << ";" << 0 << ")\n";
                }
            }

            else  
            {
                std::cerr << "\nError: Variable '" << arg->Value << "' not declared (" << ln << ";" << 0 << ")\n";
            }
        }
        
        else if (node->Type == "if") {
            AstNode* arg = node->Children[1];
        }
    }
    

    else if (node->Type == NodeType::varset) {
        AstNode* valueNode = node->Children[0];
        variables[node->Value] = evalue(valueNode);
    }
}

std::string Interpreter::evalue(AstNode* node) {
    if (node->Type == NodeType::integer_OLI || node->Type == NodeType::float_OLI || node->Type == NodeType::string) return node->Value;

    if (node->Type == NodeType::ident) {
        if (variables.find(node->Value) != variables.end())
            return variables[node->Value];
        else {
            std::cerr << "\nError: Variable '" << node->Value << "' not declared\n";
        }
    }

    if (node->Type == NodeType::add || node->Type == NodeType::sub || node->Type == NodeType::mul || node->Type == NodeType::div_OLI) {
        if (node->Children[0]->Type == NodeType::float_OLI || node->Children[1]->Type == NodeType::float_OLI)
        {
            float left = std::stof(evalue(node->Children[0]));
            float right = std::stof(evalue(node->Children[1]));
            float result = 0;
            switch (node->Type) {
                case NodeType::add: result = left + right; break;
                case NodeType::sub: result = left - right; break;
                case NodeType::mul: result = left * right; break;
                case NodeType::div_OLI: 
                    if (right == 0) {
                        std::cerr << "\nError: Division by zero\n";
                    }
                    result = left / right; break;
                default: break;
            }

            return std::to_string(result);
        }

        else 
        {
            int left = std::stoi(evalue(node->Children[0]));
            int right = std::stoi(evalue(node->Children[1]));
            int result = 0;
            switch (node->Type) {
                case NodeType::add: result = left + right; break;
                case NodeType::sub: result = left - right; break;
                case NodeType::mul: result = left * right; break;
                case NodeType::div_OLI: 
                    if (right == 0) {
                        std::cerr << "\nError: Division by zero\n";
                    }
                    result = left / right; break;
                default: break;
            }

            return std::to_string(result);
        }
    }

    return "";
}

bool Interpreter::condition(AstNode* node)
{
    AstNode* left = node->Children[0];
    AstNode* right = node->Children[1];
    bool res = false; 
    switch (node->Type)
    {
        case NodeType::equal:
            res = left->Value == right->Value;
            if (left->Type == NodeType::float_OLI || left->Type  == NodeType::integer_OLI || right->Type == NodeType::float_OLI || right->Type == NodeType::integer_OLI)
                res = std::stof(left->Value) == std::stof(right->Value);
            return res;
        break;

        case NodeType::less:
            res = std::stof(evalue(left)) < std::stof(evalue(right));
            return res;
        break;

        case NodeType::more:
            res = std::stof(evalue(left)) < std::stof(evalue(right));
            return res;
        break;

        default
        break;
    }
    return false
}