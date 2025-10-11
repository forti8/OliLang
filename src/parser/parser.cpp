#include "parser.h"
#include "astnode.h"
#include "../lexer/lexer.h"
#include "../interpreter/interpreter.h"
#include <vector>
#include <iostream>

void reportError(std::string msg, int lin, int col) {
    std::cerr << "\033[0;31m\nError: " << msg;
    std::cerr << " (" << lin << ";" << col << ")\n\033[0m\a";
}

AstNode* Parser::ParseLine(std::vector<Token> Tokens, int currentLine, int& i) {
    if (Tokens.empty()) return nullptr;

    if (Tokens[i].Type == NodeType::command && Tokens[i].Value == "print") {
        AstNode* call = new AstNode(NodeType::command, "print");
        i++;
        if (i >= Tokens.size() || Tokens[i].Type != NodeType::paropen)
            reportError("Expected '(' after 'print'", currentLine, Tokens[i].col);

        i++;
        if (i < Tokens.size()) {
            switch (Tokens[i].Type)
            {
                case NodeType::string:
                case NodeType::float_OLI:
                case NodeType::integer_OLI:
                case NodeType::ident:
                    call->Children.push_back(new AstNode(Tokens[i].Type, Tokens[i].Value));
                    i++;
                    return call;
                break;
                
                default:
                    reportError("Invalid print argument", currentLine, Tokens[i].col); 
                break;
            }
        }

    }

    else if (Tokens[i].Type == NodeType::command && Tokens[i].Value == "scan")
    {
        AstNode* input = new AstNode(NodeType::command, "scan");
        i++;
        if (i >= Tokens.size() || Tokens[i].Type != NodeType::paropen)
            reportError("Expected '(' after 'scan'", currentLine, Tokens[i].col);

        i++;
        if (i < Tokens.size()) {
            if (Tokens[i].Type == NodeType::ident) {
                input->Children.push_back(new AstNode(NodeType::ident, Tokens[i].Value));
                return input;
            }

            else 
            {
                reportError("The reference of a variable is mandatory", currentLine, Tokens[i].col);
            }
        }

        reportError("Invalid scan argument", currentLine, Tokens[i].col);
        

    }

    else if (Tokens[i].Type == NodeType::command && Tokens[i].Value == "if")
    {
        AstNode* ifstatment = new AstNode(NodeType::command, "if");
        i++;
        if (i >= Tokens.size() || Tokens[i].Type != NodeType::paropen)
            reportError("Expected '(' after 'scan'", currentLine, Tokens[i].col);

        else 
        {
            i++;
            AstNode* condition = ParseCondition(Tokens, currentLine, i);
            AstNode* ifBlock = new AstNode(NodeType::codeBlock, "block");

            if (condition == nullptr)
                reportError("Expected condition", currentLine, Tokens[i].col);

            if (i >= Tokens.size() || Tokens[i].Type != NodeType::parclose)
                reportError("Expected ')' to close", currentLine, Tokens[i].col);

            if (i + 1 >= Tokens.size() || Tokens[++i].Type != NodeType::curlyopen)
                reportError("Expected '{' to open", currentLine, Tokens[i].col);
                
            while (i < Tokens.size() && Tokens[i].Type != NodeType::curlyclose) {
                AstNode* lineNode = ParseLine(Tokens, currentLine, i);
                if (lineNode != nullptr) {
                    ifBlock->Children.push_back(lineNode);
                }

                else {
                    i++;
                }
            }
            
            i++;


            ifstatment->Children.push_back(condition); 
            ifstatment->Children.push_back(ifBlock);

            return ifstatment;
        }
    }

    else if (Tokens[i].Type == NodeType::varset) {
        return ParseAssignment(Tokens, currentLine, i);
    }

    return nullptr;
}

AstNode* Parser::ParseAssignment(std::vector<Token> Tokens, int currentLine, int& i) {
    if (i >= Tokens.size() || Tokens[i].Value == "")
        reportError("Expected variable name", currentLine, Tokens[i].col);

    AstNode* varset = new AstNode(NodeType::varset, Tokens[i].Value);
    i++;

    if (i >= Tokens.size() || Tokens[i].Type != NodeType::atrib)
        reportError("Expected '='", currentLine, Tokens[i].col);

    i++;
    AstNode* expr = ParseExpression(Tokens, currentLine, i);
    varset->Children.push_back(expr);

    return varset;
}

AstNode* Parser::ParseExpression(std::vector<Token> Tokens, int currentLine, int& i) {
    if (i>= Tokens.size())
        reportError("Expected expression", currentLine, 0);

    AstNode* left;

    switch (Tokens[i].Type)
    {
        case NodeType::string:
            left = new AstNode(NodeType::string, Tokens[i++].Value);
        break;
        
        case NodeType::float_OLI:
            left = new AstNode(NodeType::float_OLI, Tokens[i++].Value);
        break;

        case NodeType::integer_OLI:
            left = new AstNode(NodeType::integer_OLI, Tokens[i++].Value);
        break;

        case NodeType::ident:
            left = new AstNode(NodeType::ident, Tokens[i++].Value);
        break;
        
        default:
            reportError("Invalid left-hand expression", currentLine, Tokens[i].col);
        break;
    }

    if (i < Tokens.size() && Tokens[i].Type == NodeType::add) {
        i++;
        if (i >= Tokens.size())
        reportError("Expected right-hand expression after '+'", currentLine, 0);

        AstNode* right;
        switch (Tokens[i].Type)
        {
            case NodeType::float_OLI:
                right = new AstNode(NodeType::float_OLI, Tokens[i++].Value);
            break;

            case NodeType::integer_OLI:
                right = new AstNode(NodeType::integer_OLI, Tokens[i++].Value);
            break;
            
            case NodeType::string:
                right = new AstNode(NodeType::string, Tokens[i++].Value);
            break;

            case NodeType::ident:
                right = new AstNode(NodeType::ident, Tokens[i++].Value);
            break;

            default:
                reportError("Invalid right-hand expression", currentLine, Tokens[i].col);
            break;
        }

        AstNode* addNode = new AstNode(NodeType::add, "+");
        addNode->Children.push_back(left);
        addNode->Children.push_back(right);
        return addNode;
    }

    else if (i < Tokens.size() && Tokens[i].Type == NodeType::sub) {
        i++;
        if (i >= Tokens.size())
            reportError("Expected right-hand expression after '-'", currentLine, 0);

        AstNode* right;
        switch (Tokens[i].Type)
        {
            case NodeType::float_OLI:
                right = new AstNode(NodeType::float_OLI, Tokens[i++].Value);
            break;

            case NodeType::integer_OLI:
                right = new AstNode(NodeType::integer_OLI, Tokens[i++].Value);
            break;
            
            case NodeType::string:
                right = new AstNode(NodeType::string, Tokens[i++].Value);
            break;

            case NodeType::ident:
                right = new AstNode(NodeType::ident, Tokens[i++].Value);
            break;

            default:
                reportError("Invalid right-hand expression", currentLine, Tokens[i].col);
            break;
        }
        
        AstNode* subNode = new AstNode(NodeType::sub, "-");
        subNode->Children.push_back(left);
        subNode->Children.push_back(right);
        return subNode;
    }

    else if (i < Tokens.size() && Tokens[i].Type == NodeType::mul) {
        i++;
        if (i >= Tokens.size())
            reportError("Expected right-hand expression after '*'", currentLine, 0);

        AstNode* right;
        switch (Tokens[i].Type)
        {
            case NodeType::float_OLI:
                right = new AstNode(NodeType::float_OLI, Tokens[i++].Value);
            break;

            case NodeType::integer_OLI:
                right = new AstNode(NodeType::integer_OLI, Tokens[i++].Value);
            break;
            
            case NodeType::string:
                right = new AstNode(NodeType::string, Tokens[i++].Value);
            break;

            case NodeType::ident:
                right = new AstNode(NodeType::ident, Tokens[i++].Value);
            break;

            default:
                reportError("Invalid right-hand expression", currentLine, Tokens[i].col);
            break;
        }
        
        AstNode* mulNode = new AstNode(NodeType::mul, "*");
        mulNode->Children.push_back(left);
        mulNode->Children.push_back(right);
        return mulNode;
    }

    else if (i < Tokens.size() && Tokens[i].Type == NodeType::div_OLI) {
        i++;
        if (i >= Tokens.size())
            reportError("Expected right-hand expression after '/'", currentLine, 0);

        AstNode* right;
        switch (Tokens[i].Type)
        {
            case NodeType::float_OLI:
                right = new AstNode(NodeType::float_OLI, Tokens[i++].Value);
            break;

            case NodeType::integer_OLI:
                right = new AstNode(NodeType::integer_OLI, Tokens[i++].Value);
            break;
            
            case NodeType::string:
                right = new AstNode(NodeType::string, Tokens[i++].Value);
            break;

            case NodeType::ident:
                right = new AstNode(NodeType::ident, Tokens[i++].Value);
            break;

            default:
                reportError("Invalid right-hand expression", currentLine, Tokens[i].col);
            break;
        }

        AstNode* divNode = new AstNode(NodeType::div_OLI, "/");
        divNode->Children.push_back(left);
        divNode->Children.push_back(right);
        return divNode;
    }


    return left;
}

AstNode* Parser::ParseCondition(std::vector<Token> Tokens, int currentLine, int& i) {
    AstNode* left;

    switch (Tokens[i].Type) {
        case NodeType::float_OLI: left = new AstNode(NodeType::float_OLI, Tokens[i++].Value); break;
        case NodeType::integer_OLI: left = new AstNode(NodeType::integer_OLI, Tokens[i++].Value); break;
        case NodeType::string: left = new AstNode(NodeType::string, Tokens[i++].Value); break;
        case NodeType::ident: left = new AstNode(NodeType::ident, Tokens[i++].Value); break;
        default: reportError("Invalid left-hand expression", currentLine, Tokens[i].col); break;
    }

    if (i >= Tokens.size()) return nullptr;

    NodeType opType = Tokens[i].Type;
    i++;

    AstNode* right;
    switch (Tokens[i].Type) {
        case NodeType::float_OLI: right = new AstNode(NodeType::float_OLI, Tokens[i++].Value); break;
        case NodeType::integer_OLI: right = new AstNode(NodeType::integer_OLI, Tokens[i++].Value); break;
        case NodeType::string: right = new AstNode(NodeType::string, Tokens[i++].Value); break;
        case NodeType::ident: right = new AstNode(NodeType::ident, Tokens[i++].Value); break;
        default: reportError("Invalid right-hand expression", currentLine, Tokens[i].col); break;
    }

    AstNode* cond = nullptr;
    switch (opType) {
        case NodeType::equal: cond = new AstNode(NodeType::equal, "=="); break;
        case NodeType::diff: cond = new AstNode(NodeType::diff, "!="); break;
        case NodeType::less: cond = new AstNode(NodeType::less, "<"); break;
        case NodeType::more: cond = new AstNode(NodeType::more, ">"); break;
        default: reportError("Unknown operator in condition", currentLine, Tokens[i].col); break;
    }

    cond->Children.push_back(left);
    cond->Children.push_back(right);
    return cond;
}