#include <cassert>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>

typedef int value_t;

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

value_t GLOBAL_TREE_VAL_POISON = 0xEB1DEDA;

treeNode* NewTreeNode (value_t value);
void      DelTreeNode (treeNode* node);
tree*     NewTree     (value_t rootValue);
void      DeleteTree  (tree* tree);

void LinkLeft  (treeNode* child, treeNode* parent);
void LinkRight (treeNode* child, treeNode* parent);

void Insert (tree* tree, value_t value);
void DupmTreePrev (treeNode* node);
void DupmTreeIn (treeNode* node);

tree* BuildPrev (const char* fileName);
void  ParseRec  (FILE* treeFile, treeNode* node);

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

treeNode* NewTreeNode (value_t value)
{
    treeNode* newNode = (treeNode*) calloc (1, sizeof (treeNode));
    if (!newNode) 
    {
        printf ("New node allocation err\n");
        return nullptr;
    }

    newNode->value = value;
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
    assert (parent->left == nullptr);
    assert (child->parent == nullptr);

    parent->left = child;
    child->parent = parent;
}

void LinkRight (treeNode* child, treeNode* parent)
{
    assert (child != nullptr);
    assert (parent != nullptr);
    assert (parent->right == nullptr);
    assert (child->parent == nullptr);

    parent->right = child;
    child->parent = parent;
}

void Insert (tree* tree, value_t value)
{
    treeNode* childNode = NewTreeNode (value);

    treeNode* curNode = tree->root;
    place     newPlace = Left; 

    while (1)
    {
        if (value >= curNode->value)
        {
            if (curNode->right != nullptr) 
            {
                curNode = curNode->right;
            }

            else
            {
                newPlace = Right;
                break;
            }
        }

        else
        {
            if (curNode->left != nullptr) 
            {
                curNode = curNode->left;
            }

            else
            {
                newPlace = Left;
                break;
            }
        }
    }

    if (newPlace == Left)  LinkLeft  (childNode, curNode);
    if (newPlace == Right) LinkRight (childNode, curNode);
    tree->nOfNode++;
}

errCodes VerifyTree ()
{
    return TreeOk;
}

void DupmTreePrev (treeNode* node)
{
    printf ("(");
    printf ("%d", node->value);
    if (node->left)  DupmTreePrev (node->left);
    else printf ("()");
    if (node->right) DupmTreePrev (node->right);
    else printf ("()");
    printf (")");
}

void DupmTreeIn (treeNode* node)
{
    printf ("(");
    if (node->left)  DupmTreeIn (node->left);
    printf ("%d", node->value);
    if (node->right) DupmTreeIn (node->right);
    printf (")");
}

tree* BuildPrev (const char* fileName)
{
    tree* tree = NewTree (GLOBAL_TREE_VAL_POISON);
    printf ("bfr parse\n");

    FILE* treeFile = fopen (fileName, "r");

    ParseRec (treeFile, tree->root);
    printf ("aft parse\n");
    return tree;
}

void ParseRec (FILE* treeFile, treeNode* node)
{
    if (fgetc (treeFile) == '(')
    {
        GraphDump (node);
        fscanf (treeFile, "%d", &node->value);
        GraphDump (node);

        if (fgetc (treeFile) == ')') return;

        ungetc ('(', treeFile);

        treeNode* left  = NewTreeNode (GLOBAL_TREE_VAL_POISON);
        treeNode* right = NewTreeNode (GLOBAL_TREE_VAL_POISON);

        ParseRec  (treeFile, left);
        (left->value == GLOBAL_TREE_VAL_POISON) ? DelTreeNode (left) : LinkLeft  (left, node);
        GraphDump (node);
        
        ParseRec  (treeFile, right);
        (right->value == GLOBAL_TREE_VAL_POISON) ? DelTreeNode (right) : LinkRight (right, node);

        fgetc (treeFile);
        GraphDump (node);
    }
    else return;    
}

int GLOBAL_DUMP_COUNT = 0;

void GraphDump (treeNode* node)
{
    FILE* dotFile = fopen ("dmp.txt", "w");
    fprintf (dotFile, "digraph G{\n"); 
    CreateNodes (node, dotFile);
    CreateLinks (node, dotFile);
    fprintf (dotFile, "}\n");
    fclose  (dotFile);

    char* dumpCmd = (char*) calloc (100, sizeof(char));
    sprintf (dumpCmd, "Dot C:/DEDdop/binTree/dmp.txt -Tpng -o C:/DEDdop/binTree/dumps/GraphDump%d.png", GLOBAL_DUMP_COUNT);
    system  (dumpCmd);
    free    (dumpCmd);

    FILE* dmpHtml = (GLOBAL_DUMP_COUNT == 0) ? fopen ("dmpHtml.html", "w+") : fopen ("dmpHtml.html", "a");
    if (dmpHtml == nullptr)
    {
        printf ("html file open err\n");
        return;
    }

    fprintf (dmpHtml, "<pre>\n");

    fprintf (dmpHtml, "\n"
                      "\t\t<img src = \"dumps/GraphDump%d.png\">\n"
                      "\t<hr>\n"
                      "<!-- ------------------------------------------------------------ -->\n", GLOBAL_DUMP_COUNT);

    fprintf (dmpHtml, "</pre>\n\n");

    GLOBAL_DUMP_COUNT++;
    fclose (dmpHtml);
}

void CreateNodes (treeNode* node, FILE* dotFile)
{
    fprintf (dotFile, "\tnode%p[shape=Mrecord, style=filled, fillcolor=\"#%zu\" label = \"{ptr = %p | value = %d| parent = %p | left = %p | right = %p}\"];\n", node, (size_t) (node + 7) * 7 % 0x1000000, node, node->value, node->parent, node->left, node->right);
    if (node->left) CreateNodes (node->left, dotFile);
    if (node->right) CreateNodes (node->right, dotFile);
}

void CreateLinks (treeNode* node, FILE* dotFile)
{
    if (node->left) 
    {
        fprintf (dotFile, "\tnode%p -> node%p;\n", node, node->left);
        CreateLinks (node->left, dotFile);
    }

    if (node->right)
    {
        fprintf (dotFile, "\tnode%p -> node%p;\n", node, node->right);
        CreateLinks (node->right, dotFile);
    }
}