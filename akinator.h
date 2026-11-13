#include <cassert>
#include <stdio.h>
#include <stdlib.h>
#include "reader.cpp"

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

value_t GLOBAL_TREE_VAL_POISON = nullptr;

treeNode* NewTreeNode (value_t value);
void      DelTreeNode (treeNode* node);
tree*     NewTree     (value_t rootValue);
void      DeleteTree  (tree* tree);

void LinkLeft  (treeNode* child, treeNode* parent);
void LinkRight (treeNode* child, treeNode* parent);

void Insert       (tree* tree, value_t value);


enum errCodes 
{
    TreeOk = 0,
    ParentErr = 1
};

struct errCtx 
{
    treeNode* badNode;
    errCodes code;
    size_t line;
};

errCodes VerifyTree (treeNode* node, size_t assertline);
void   RecParChec (treeNode* node, errCtx* ctx);


tree* BuildPrev (FILE* treeFile, String* strings);
void  ParseRec  (FILE* treeFile, treeNode* node, size_t* nParsingNode, String* strings);

void DumpTreePrev  (treeNode* node);
void DumpTreeIn    (treeNode* node);
void FDumpTreePrev (FILE* dumpFile, treeNode* node);
void GraphDump   (treeNode* node, errCtx* ctx);
void CreateNodes (treeNode* node, FILE* dotFile);
void CreateLinks (treeNode* node, FILE* dotFile);

inline void BufferClean ();
char GetYorN        ();
void GuessLogic     (treeNode* node, treeNode** badNode);
treeNode* NewAnswer (treeNode* ansNode);

treeNode*  FindNode    (treeNode* node, const char* object);
treeNode** GetWay      (treeNode* node, int* deep);
void       Opredelenie (treeNode* node);
void       Difference  (treeNode* node1, treeNode* node2);
