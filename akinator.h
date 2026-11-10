#include <cassert>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include "reader.cpp"
#include <cstdio>

typedef char* value_t;
#define IOTYPE "%s"

struct treeNode
{
    value_t value;
    treeNode* left;
    treeNode* right;
    treeNode* parent;
};

struct tree
{
    treeNode* root;
    int nOfNode;
};

value_t GLOBAL_TREE_VAL_POISON = "";

treeNode* NewTreeNode (value_t value);
void      DelTreeNode (treeNode* node);
tree*     NewTree     (value_t rootValue);
void      DeleteTree  (tree* tree);

void LinkLeft  (treeNode* child, treeNode* parent);
void LinkRight (treeNode* child, treeNode* parent);

void Insert       (tree* tree, value_t value);
void DupmTreePrev (treeNode* node);
void DupmTreeIn   (treeNode* node);

tree* BuildPrev (const char* fileName, String* strings);
void ParseRec   (FILE* treeFile, treeNode* node, size_t nParsingNode, String* strings);

void GraphDump   (treeNode* node);
void CreateNodes (treeNode* node, FILE* dotFile);
void CreateLinks (treeNode* node, FILE* dotFile);


enum place 
{
    Left = 0, 
    Right = 1
};

enum errCodes 
{
    TreeOk = 0,
};