#include "akinator.h"
#include "RayGraph.cpp"
#include "ConsolGuess.cpp"
#include <cstdio>

treeNode* NewTreeNode (value_t value)
{
    treeNode* newNode = (treeNode*) calloc (1, sizeof (treeNode));
    if (!newNode) 
    {
        printf ("New node allocation err\n");
        return nullptr;
    }

    newNode->parent = newNode;
    newNode->value = value;
    VERIFY_TREE (newNode);

    return newNode;
}

void DelTreeNode (treeNode* node)
{
    assert (node != nullptr);
    if (node->left)  DelTreeNode (node->left);
    if (node->right) DelTreeNode (node->right);
    free (node);
}

tree* NewTree (value_t rootValue)
{
    tree* newTree = (tree*) calloc (1, sizeof (tree));
    if (!newTree) 
    {
        printf ("New tree allocation err\n");
        return nullptr;
    }
    newTree->root = NewTreeNode (rootValue);
    newTree->root->parent = newTree->root;
    newTree->nOfNode = 1;

    return newTree;
}

void DeleteTree (tree* tree)
{
    assert (tree != nullptr);
    DelTreeNode (tree->root);
    free (tree);
}

void LinkLeft (treeNode* child, treeNode* parent)
{
    assert (child != nullptr);
    assert (parent != nullptr);
    assert (parent->left  == nullptr);
    assert (child->parent == child);

    parent->left = child;
    child->parent = parent;
    VERIFY_TREE (parent);
}

void LinkRight (treeNode* child, treeNode* parent)
{
    assert (child != nullptr);
    assert (parent != nullptr);
    assert (parent->right == nullptr);
    assert (child->parent == child);

    parent->right = child;
    child->parent = parent;
    GraphDump (parent, nullptr);
    VERIFY_TREE (parent);
}

errCodes VerifyTree (treeNode* node, size_t assertLine)
{
    errCtx ctx = {.badNode = nullptr, .code = TreeOk, .line = assertLine};

    Check (node, &ctx);
    if (ctx.badNode) GraphDump (node, &ctx);
    return ctx.code;
}

static void Check (treeNode* node, errCtx* ctx) 
{
    if (!node->parent) 
    {
        if (!node->value) return;
        ctx->badNode = node;
        ctx->code = ParentErr;
        return;
    }

    if (node != node->parent->left && node != node->parent->right && node != node->parent)
    {
        ctx->badNode = node;
        ctx->code = ParentErr;
        return;
    }

    if (node->left)
    {
        Check (node->left, ctx);
        if (ctx->code != TreeOk) return;
    }

    if (node->right)
    {
        Check (node->right, ctx);
        if (ctx->code != TreeOk) return;
    }
    return; 
}

void DumpTreePrev (treeNode* node)
{
    VERIFY_TREE (node);

    if (*node->value != '\0')
    {
        printf ("(");
        printf (IOTYPE, node->value);
        if (node->left)  DumpTreePrev (node->left);
        else printf ("()");
        if (node->right) DumpTreePrev (node->right);
        else printf ("()");
        printf (")");
    }
}

void FDumpTreePrev (FILE* dumpFile, treeNode* node)
{
    VERIFY_TREE (node);
    
    if (*node->value != '\0')
    {
        fprintf (dumpFile, "(");
        fprintf (dumpFile, IOTYPE, node->value);
        if (node->left)  FDumpTreePrev (dumpFile, node->left);
        if (node->right) FDumpTreePrev (dumpFile, node->right);
        fprintf (dumpFile, ")");
    }
}

void DumpTreeIn (treeNode* node)
{
    VERIFY_TREE (node);

    printf ("(");
    if (node->left)  DumpTreeIn (node->left);
    printf (IOTYPE, node->value);
    if (node->right) DumpTreeIn (node->right);
    printf (")");
}

tree* BuildPrev (FILE* treeFile, String* strings)
{
    fseek (treeFile, 0, SEEK_SET);
    tree* tree = NewTree (nullptr);

    size_t nOfParsStr = 0;
    tree->nOfNode = nOfParsStr;
    ParseRec (treeFile, tree->root, &nOfParsStr, strings);

    VERIFY_TREE (tree->root);
    return tree;
}

static void ParseRec (FILE* treeFile, treeNode* node, size_t* nParsingNode, String* strings)
{
    if (fgetc (treeFile) == '(')
    {
        node->value = strings [*nParsingNode].strPtr;
        fseek (treeFile, strings [*nParsingNode].len, SEEK_CUR);
        ++*nParsingNode;

        int curChar = fgetc (treeFile);
        if (curChar == ')') return;
        if (curChar != '(') printf ("err, expected symbol is '(', the real is \'%c\', %d\n", curChar, curChar);
        ungetc (curChar, treeFile);

        treeNode* left  = NewTreeNode (nullptr);
        treeNode* right = NewTreeNode (nullptr);
        ParseRec (treeFile, left,  nParsingNode, strings);
        ParseRec (treeFile, right, nParsingNode, strings);
        (left->value  [0] == '\0') ? DelTreeNode (left)  : LinkLeft  (left, node);
        (right->value [0] == '\0') ? DelTreeNode (right) : LinkRight (right, node);

        curChar = fgetc (treeFile);
        if (curChar != ')') printf ("err, expected symbol is ')', the real is \'%c\', %d\n", curChar, curChar);
    }
    else return;    
}

int GLOBAL_DUMP_COUNT = 0;

void GraphDump (treeNode* node, errCtx* ctx)
{
    FILE* dotFile = fopen ("dmp.txt", "w");
    fprintf (dotFile, "digraph G{\n"); 
    CreateNodes (node, dotFile);

    if (ctx) fprintf (dotFile, "\tnode%p[shape=Mrecord, style=filled, fillcolor=\"red\" label = \"{ptr = %p | value =" IOTYPE "| parent = %p | left = %p | right = %p}\"];\n", 
                                ctx->badNode, ctx->badNode, ctx->badNode->value, ctx->badNode->parent, ctx->badNode->left, ctx->badNode->right);

    CreateLinks (node, dotFile);
    fprintf (dotFile, "}\n");
    fclose  (dotFile);

    char* dumpCmd = (char*) calloc (100, sizeof(char));
    sprintf (dumpCmd, "Dot dmp.txt -Tpng -o dumps/GraphDump%d.png", GLOBAL_DUMP_COUNT);
    system  (dumpCmd);
    free    (dumpCmd);

    FILE* dmpHtml = (GLOBAL_DUMP_COUNT == 0) ? fopen ("dmpHtml.html", "w+") : fopen ("dmpHtml.html", "a");
    if (dmpHtml == nullptr)
    {
        printf ("html file open err\n");
        return;
    }

    fprintf (dmpHtml, "<pre>\n");

    if (ctx)    
    {
        fprintf (dmpHtml, "\t<h1> TreeDump: assertline:%zu, errCode %d</h1>\n", ctx->line, ctx->code);
    }

    fprintf (dmpHtml, "\n"
                      "\t\t<img src = \"dumps/GraphDump%d.png\">\n"
                      "\t  <hr>\n"
                      "<!-- ------------------------------------------------------------ -->\n", GLOBAL_DUMP_COUNT);

    fprintf (dmpHtml, "</pre>\n\n");

    GLOBAL_DUMP_COUNT++;
    fclose (dmpHtml);
}

static void CreateNodes (treeNode* node, FILE* dotFile)
{
    fprintf (dotFile, "\tnode%p[shape=Mrecord, style=filled, fillcolor=\"#%zu\" label = \"{ptr = %p | value =" IOTYPE "| parent = %p | left = %p | right = %p}\"];\n",
                         node, ColorGen (node), node, node->value, node->parent, node->left, node->right);
    if (node->left)  CreateNodes (node->left, dotFile);
    if (node->right) CreateNodes (node->right, dotFile);
}

static size_t ColorGen (treeNode* node)
{
    return ((size_t) node + 86756876) * 7 % 0x1000000;
}

static void CreateLinks (treeNode* node, FILE* dotFile)
{
    if (node->left) 
    {
        fprintf (dotFile, "\tnode%p -> node%p [label = \"da\", color = \"green\"];\n", node, node->left);
        CreateLinks (node->left, dotFile);
    }

    if (node->right)
    {
        fprintf (dotFile, "\tnode%p -> node%p [label = \"net\", color = \"red\"];\n", node, node->right);
        CreateLinks (node->right, dotFile);
    }
}

static treeNode* FindNode (treeNode* node, const char* object)
{
    if (strcmp (object, node->value) == 0)
    {
        return node;
    }

    if (node->left)
    {
        treeNode* curNode = nullptr;
        curNode = FindNode (node->left, object);
        if (curNode) return curNode;
    }

    if (node->right)
    {
        treeNode* curNode = nullptr;
        curNode = FindNode (node->right, object);
        if (curNode) return curNode;
    }

    return nullptr;
}

treeNode** GetWay (treeNode* node, int* deep)
{
    *deep = 0;
    int maxWaySize = 10;
    treeNode** way = (treeNode**) calloc (maxWaySize, sizeof (treeNode*));
    if (!way)
    {
        printf ("way allocation err\n");
        return way;
    }
    treeNode* curNode = node;

    while (curNode->parent != curNode)
    { 
        way [*deep] = curNode->parent;
        ++*deep;
        curNode = curNode->parent;
    }

    return way;
}