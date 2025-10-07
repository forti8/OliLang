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
    atrib,
    diff,
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