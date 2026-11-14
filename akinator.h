#ifndef AKINH

#define AKINH

#include <cassert>
#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include "raylib/raylib.h"
#include "reader.cpp"

#define ON_DBG

#ifdef ON_DBG

#define VERIFY_TREE(node) \
        do{\
            if (VerifyTree (node, __LINE__))\
            {\
                printf ("%d\n", __LINE__);\
                abort ();\
            }\
        }\
        while (0)
#else

#define VERIFY_TREE(node) 

#endif

typedef char* value_t;
#define IOTYPE "%s"

struct treeNode
{
    value_t value;
    treeNode* left;
    treeNode* right;
    treeNode* parent;
};

typedef decltype(sizeof(NULL)) usize_t;

struct tree
{
    treeNode* root;
    int nOfNode;
};


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
    ParentErr = 1,
};

struct errCtx 
{
    treeNode* badNode;
    errCodes code;
    size_t line;
};

errCodes VerifyTree (treeNode* node, size_t assertline);
static void   Check (treeNode* node, errCtx* ctx);


tree* BuildPrev (FILE* treeFile, String* strings);
static void ParseRec (FILE* treeFile, treeNode* node, size_t* nParsingNode, String* strings);

void DumpTreePrev  (treeNode* node);
void DumpTreeIn    (treeNode* node);
void FDumpTreePrev (FILE* dumpFile, treeNode* node);
void GraphDump   (treeNode* node, errCtx* ctx);
static void CreateNodes (treeNode* node, FILE* dotFile);
static void CreateLinks (treeNode* node, FILE* dotFile);
static size_t ColorGen  (treeNode* node);


static void BufferClean ();
static char GetYorN     ();
void GuessLogic  (treeNode* node, treeNode** badNode);
void NewAnswer   (treeNode* ansNode);

treeNode*  FindNode    (treeNode* node, const char* object);
treeNode** GetWay      (treeNode* node, int* deep);
void       Opredelenie (treeNode* node);
void       Difference  (treeNode* node1, treeNode* node2);
void       PrintfWay   (treeNode** way, treeNode* node, int deep);


struct windowCtx 
{
    float screenWidth;
    float screenHeight;
    Rectangle buttonY;
    Rectangle buttonN;
};

void GraphInt     (treeNode* node);
void DrawFrame    (windowCtx ctx, treeNode* node);
void DrawButtons  (windowCtx ctx);
void DrawQuest    (windowCtx ctx, treeNode* node);
void DrawCorAns   (windowCtx ctx);
char GetYorNGraph (windowCtx ctx);
void GetInputString (char* prompt, char* buffer, int maxLen);
void SplitGraph (treeNode* nodeToSplit);

#endif