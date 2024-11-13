#ifndef AKINATOR_H
#define AKINATOR_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "BinTree.h"
#include "../MyStack/StackFunc.h"


#define DEBUG

int GuessObject(Tree_t *tree);

Stack_t* DefineObjectMode(const Tree_t *tree, const char *object, Stack_t *stack);

void CompairObjectsMode(const Tree_t *tree, const char *object1, const char *object2);

void ShowTree(const Tree_t *tree);

#endif // AKINATOR_H
