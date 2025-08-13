#pragma once
#include <vector>
#include <string>

#ifndef OLI_ASTNODE
#define OLI_ASTNODE

enum NodeType
{
    // calc
    add,
    sub,
    div_OLI,
    mul,

    // condition
    equal,
    less,
    more,
    condition,

    command,
    assign,
    codeBlock,

    string,
    integer_OLI,
    float_OLI,

    ident,
    varset,
    varname,
    paropen,
    curlyopen,
    curlyclose,
    parclose,
    atrib
};

struct AstNode {
    NodeType Type;
    std::string Value;
    std::vector<AstNode*> Children;

    AstNode() = default;
    AstNode(NodeType type, const std::string& value) : Type(type), Value(value) {}
};


#endif