// include only once
#pragma once

// libs
#include <vector>
#include <string>

// if define OLI_ASTNODE
#ifndef OLI_ASTNODE
#define OLI_ASTNODE

// enumeration node type
enum NodeType
{
    // calc
    add, // 0,
    sub, // 1,
    div_OLI, // 2,
    mul, // 3,

    // condition
    equal, // 4,
    less, // 5,
    more, // 6,
    condition, // 7,
    
    command, // 8,
    assign, // 9,
    codeBlock, // 10,

    string, // 11,
    integer_OLI, // 12,
    float_OLI, // 13,

    ident, // 14,
    varset, // 15,
    varname, // 16,
    paropen, // 17,
    curlyopen, // 18,
    curlyclose, // 19,
    parclose, // 20,
    atrib, // 21,
    diff // 22,
};

// primitive astnode struct
struct AstNode {

    // :type: defined by enum nodetype
    NodeType Type;

    // :value: being of type string to be able to manipulate the content more easily 
    std::string Value;

    // each :AstNode: can contain a :child node: creating a synthetic tree
    std::vector<AstNode*> Children;

    // define a default constructor for astnode
    AstNode() = default;
    AstNode(NodeType type, const std::string& value) : Type(type), Value(value) {}
};

// a block/scope is defined as just a list of nodes in sequence
struct AstBlock {
    std::vector<AstNode*> Body;
};

#endif