// libs
#include "interpreter.h"
#include "../parser/astnode.h"
#include <iostream>
#include <cstdlib>

// intepret a individual astnode*
void Interpreter::interpretLine(AstNode* node, int ln) {
    
    // non exists node
    if (!node) return;

    // command booleans cases
    bool isAnCommand = node->Type == NodeType::command;
    bool printCase = node->Value == "print";
    bool scanCase = node->Value == "scan";

    // command condition
    if (isAnCommand) {

        // print command
        if (printCase) {

            // astnode argument (printable value)
            AstNode* arg = node->Children[0];

            // argument is a direct value
            bool argumentIsString = arg->Type == NodeType::string;
            bool argumentIsInteger = arg->Type == NodeType::integer_OLI;
            bool argumentIsFloat = arg->Type == NodeType::float_OLI;
            bool argumentIsDirectValue = argumentIsString || argumentIsInteger || argumentIsFloat;

            // argument is a variable
            bool argumentIsVariable = arg->Type == NodeType::ident;
            bool argumentVariableIsDefined = variables.find(arg->Value) != variables.end();

            // direct value case
            if (argumentIsDirectValue) {
                std::cout << arg->Value;
            } 
            
            // variable (ident -> astcode 15)
            else if (argumentIsVariable) {
                
                // defined variable
                if (argumentVariableIsDefined) {
                    std::cout << variables[arg->Value];
                } 
                
                // not defined variable
                else {
                    std::cerr << "\nError: Variable '" << arg->Value << "' not declared (" << ln << ";" << 0 << ")\n";
                }
            }
        }

        // scan command
        else if (scanCase) {

            // astnode argument
            AstNode* arg = node->Children[0];

            // argument is a variable
            bool argumentIsVariable = arg->Type == NodeType::ident;
            bool argumentVariableIsDefined = variables.find(arg->Value) != variables.end();

            // is variable (required variable)
            if (argumentIsVariable) {

                // variable defined
                if (argumentVariableIsDefined) {
                    std::cin >> variables[arg->Value];
                }

                // variable not defined
                else {
                    std::cerr << "\nError: Variable '" << arg->Value << "' not declared (" << ln << ";" << 0 << ")\n";
                }
            }

            // not is a variable
            else  {
                std::cerr << "\nError: Variable '" << arg->Value << "' not declared (" << ln << ";" << 0 << ")\n";
            }
        }
        
        // ifcase
        else if (node->Value == "if") {

            // ast arguments of if
            AstNode* arg = node->Children[0];
            bool condictionResult = Interpreter::condition(arg);

            // interprets each line within the scope
            if (condictionResult) {

                // for each n in node->Children[1]->Children
                for (auto n : node->Children[1]->Children) {
                    Interpreter::interpretLine(n, ln);
                };
            }
        }
    }
    

    
    else if (node->Type == NodeType::varset) {
        AstNode* valueNode = node->Children[0];
        std::string val = evalue(valueNode);
        variables[node->Value] = val;
    }
}

// evalue an node
std::string Interpreter::evalue(AstNode* node) {

    // case to defined value sentence (no required evalue)
    bool intergerCase = node->Type == NodeType::integer_OLI;
    bool floatCase = node->Type == NodeType::float_OLI;
    bool stringCase = node->Type == NodeType::string;

    // no required evalue about this sentence
    bool definedValueSentence = intergerCase || floatCase || stringCase;
    if (definedValueSentence) return node->Value;

    // required evalue about this sentence
    // variable (identified)
    if (node->Type == NodeType::ident) {

        // variabled defined
        if (variables.find(node->Value) != variables.end()) {
            return variables[node->Value];
        }

        // variable not defined 
        else {
            std::cerr << "\nError: Variable '" << node->Value << "' not declared\n";
        }
    }

    // define operation type
    bool addCase = node->Type == NodeType::add;
    bool subCase = node->Type == NodeType::sub;
    bool mulCase = node->Type == NodeType::mul;
    bool divCase = node->Type == NodeType::div_OLI;

    // define if this is a operation (primitive)
    bool primitiveOperationCase = addCase || subCase || mulCase || divCase;
    if (primitiveOperationCase) {

        // left or right hand is an float number
        bool childrenLeftFloatCase = node->Children[0]->Type == NodeType::float_OLI;
        bool childrenRightFloatCase = node->Children[1]->Type == NodeType::float_OLI;

        // left and right hand is an integer number
        bool childrenLeftIntegerCase = node->Children[0]->Type == NodeType::integer_OLI;
        bool childrenRightIntegerCase = node->Children[1]->Type == NodeType::integer_OLI;

        // if left or right hand is an float number -> define a float operation
        bool floatOperation = childrenLeftFloatCase || childrenRightFloatCase;

        // define a integer operation
        bool integerOperation = !floatOperation;

        // float operation case
        if (floatOperation) {

            // left/right hand
            float left = std::stof(evalue(node->Children[0]));
            float right = std::stof(evalue(node->Children[1]));

            // result variable
            float result = .0;

            // exec. operation
            /*
                @NodeType
                    add 0; 
                    sub +1;
                    div +2;
                    mul +3;
            */
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

        // integer operation
        else if (integerOperation) {

            // left/right hand
            int left = std::stoi(evalue(node->Children[0]));
            int right = std::stoi(evalue(node->Children[1]));

            // integer result variable
            int result = 0;

            // operation switch case
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

    // null STRING
    return "";
}


// interpret an condition
bool Interpreter::condition(AstNode* node) {

    // left and rigth hand of expression
    AstNode* left = node->Children[0];
    AstNode* right = node->Children[1];

    // sentence's response
    bool res = true; 

    // switch case of NodeType of operation
    switch (node->Type) {

        // like anything (no same type)
        case NodeType::equal: {
            // number case 
            bool leftIsAnNumber = (left->Type == NodeType::float_OLI || left->Type  == NodeType::integer_OLI);
            bool rightIsAnNumber = (right->Type == NodeType::float_OLI || right->Type == NodeType::integer_OLI);

            // reference case
            bool leftIsAnVariable = (left->Type == NodeType::ident);
            bool rightIsAnVariable = (right->Type == NodeType::ident);

            // case number condition
            bool numberCondition = leftIsAnNumber && rightIsAnNumber;
            if (numberCondition) {
                res = std::stof(left->Value) == std::stof(right->Value);
            }

            else if (leftIsAnVariable || rightIsAnVariable) {

                std::string value1 = left->Value;
                std::string value2 = right->Value;

                if (leftIsAnVariable) {

                    // variabled defined
                    if (variables.find(left->Value) != variables.end()) {
                        value1 = variables[left->Value];
                    }

                    // variable not defined 
                    else {
                        std::cerr << "\nError: Variable '" << left->Value << "' not declared\n";
                    }
                }

                if (rightIsAnVariable) {

                    // variabled defined
                    if (variables.find(right->Value) != variables.end()) {
                        value2 = variables[right->Value];
                    }

                    // variable not defined 
                    else {
                        std::cerr << "\nError: Variable '" << right->Value << "' not declared\n";
                    }
                }

                res = std::stof(value1) == std::stof(value2);
            }

            // other
            else {
                res = left->Value == right->Value;
            }

            break;
        } 

        // like anything (no same type)
        case NodeType::diff: {

            // number case 
            bool leftIsAnNumber = (left->Type == NodeType::float_OLI || left->Type  == NodeType::integer_OLI);
            bool rightIsAnNumber = (right->Type == NodeType::float_OLI || right->Type == NodeType::integer_OLI);

            // reference case
            bool leftIsAnVariable = (left->Type == NodeType::ident);
            bool rightIsAnVariable = (right->Type == NodeType::ident);

            // case number condition
            bool numberCondition = leftIsAnNumber && rightIsAnNumber;
            if (numberCondition) {
                res = std::stof(left->Value) != std::stof(right->Value);
            }

            else if (leftIsAnVariable || rightIsAnVariable) {

                std::string value1 = left->Value;
                std::string value2 = right->Value;

                if (leftIsAnVariable) {

                    // variabled defined
                    if (variables.find(left->Value) != variables.end()) {
                        value1 = variables[left->Value];
                    }

                    // variable not defined 
                    else {
                        std::cerr << "\nError: Variable '" << left->Value << "' not declared\n";
                    }
                }

                 if (rightIsAnVariable) {

                    // variabled defined
                    if (variables.find(right->Value) != variables.end()) {
                        value2 = variables[right->Value];
                    }

                    // variable not defined 
                    else {
                        std::cerr << "\nError: Variable '" << right->Value << "' not declared\n";
                    }
                }

                res = std::stof(value1) != std::stof(value2);
            }
            
            // other
            else {
                res = left->Value != right->Value;
            }

            break;
        } 

        case NodeType::less: {
            // less than
            res = std::stof(evalue(left)) < std::stof(evalue(right));
            break;
        }


        case NodeType::more: {
            // more than
            res = std::stof(evalue(left)) > std::stof(evalue(right));
            break;
        }


        default:
            res = true;
        break;
    }

    return res;
}