#ifndef INCLUDE_BINTREE
#define INCLUDE_BINTREE

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

enum NodeChild
{
    RIGHT_CHILD,
    LEFT_CHILD,
};

enum
{
    ROOT_NULL_PTR,
    TREE_NULL_PTR,
    FILE_NULL_PTR,
    NODE_NULL_PTR,
    TREE_INSERT_ERROR,
    FILE_INSERT_ERROR,
    TREE_FILL_ERROR,
    ALLOCATED_FAILURE,
    NO_ERROR
};

const size_t STR_MAX_SIZE = 128;

typedef char Code_t;

typedef struct Node_t
{
    Code_t* data;
    Node_t* left;
    Node_t* right;
    size_t num;
} Node_t;

typedef struct Tree_t
{
    Node_t* root;
    size_t size;
} Tree_t;

size_t TreeCtor(Tree_t* tree);
Node_t* TreeInsert(Tree_t *tree, Node_t *node, char *str, const NodeChild child);
size_t TreeDtor(Tree_t* tree);

size_t TreeDump(Tree_t* tree);
size_t TreeBuild(Tree_t *tree);
size_t PrintTree(Tree_t* tree, FILE* logfile);
size_t PrintNode(Node_t* node, FILE* logfile, size_t rang, size_t nodecounter);
size_t TreeSaveInFile(Tree_t* tree);

#endif
