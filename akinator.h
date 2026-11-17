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


tree* BuildPrev      (FILE* treeFile, String* strings);
static void ParseRec (FILE* treeFile, treeNode* node, size_t* nParsingNode, String* strings);

void DumpTreePrev  (treeNode* node);
void DumpTreeIn    (treeNode* node);
void FDumpTreePrev (FILE* dumpFile, treeNode* node);
void GraphDump     (treeNode* node, errCtx* ctx);
static void CreateNodes (treeNode* node, FILE* dotFile);
static void CreateLinks (treeNode* node, FILE* dotFile);
static size_t ColorGen  (treeNode* node);


static void BufferClean ();
static char GetYorN     ();
void GuessLogic         (treeNode* node, treeNode** badNode);
static void NewAnswer   (treeNode* ansNode);

static treeNode*  FindNode   (treeNode* node, const char* object);
treeNode** GetWay     (treeNode* node, int* deep);
void       Definition (treeNode* node);
void       Compare    (treeNode* node);
void       PrintfWay  (treeNode** way, treeNode* node, int deep);

struct button 
{
    Rectangle rect;
    const char* text;
};

void GraphInt       (treeNode* node, treeNode** badAnsNode);
void DrawFrame      (treeNode* node, button btnY, button btnN);
void DrawQuest      (treeNode* node);
void DrawCorAns     ();
char GetYorNGraph   (button btnY, button btnN);
void GetInputString (const char* prompt, char* buffer, int maxLen);
void SplitGraph     (treeNode* nodeToSplit);


enum workType : char
{
    No = 0,
    Guess = 1,
    Def = 2,
    Cmp = 3,
};


struct wayCtx
{
    treeNode** way;
    int deep;
};

int DrawMenuButtons ();
void Menu (tree* tree, treeNode** badAnsNode);
void ConsoleMenu (tree* tree, treeNode** badAnsNode);
workType GraphModeMenu ();
treeNode* GetNodeByStr (treeNode* node);
treeNode* GetNodeByStrGraph (treeNode* node, const char* prompt);
void DrawButton (button btn);
void GraphDef (treeNode* node);
int PrintWayGraph (wayCtx way, Vector2* printPos, treeNode* node, const char* header);
int PrintSameWayGraph (wayCtx way0, wayCtx way1, Vector2* printPos, const char* header);
void GraphCmp (treeNode* node);

#endif