#include "akinator.h"
#include <cstdio>

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
    assert (parent->left  == nullptr);
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

    printf ("was here\n");

    parent->right = child;
    child->parent = parent;
}

errCodes VerifyTree ()
{
    return TreeOk;
}

void DupmTreePrev (treeNode* node)
{
    printf ("(");
    printf (IOTYPE, node->value);
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
    printf (IOTYPE, node->value);
    if (node->right) DupmTreeIn (node->right);
    printf (")");
}

tree* BuildPrev (const char* fileName, String* strings)
{
    tree* tree = NewTree (GLOBAL_TREE_VAL_POISON);
    printf ("bfr parse\n");

    FILE* treeFile = fopen (fileName, "r");

    ParseRec (treeFile, tree->root, 0, strings);
    printf ("aft parse\n");
    return tree;
}

void ParseRec (FILE* treeFile, treeNode* node, size_t nParsingNode, String* strings)
{
    if (fgetc (treeFile) == '(')
    {
        node->value = strings [nParsingNode].strPtr;
        fseek (treeFile, strings [nParsingNode].len, SEEK_CUR);
        nParsingNode++;
        GraphDump (node);

        if (fgetc (treeFile) == ')') return;
        ungetc ('(', treeFile);

        treeNode* left  = NewTreeNode (GLOBAL_TREE_VAL_POISON);
        treeNode* right = NewTreeNode (GLOBAL_TREE_VAL_POISON);

        //TODO - вместо создания и удаления не создавать
        ParseRec (treeFile, left, nParsingNode, strings);
        (left->value == GLOBAL_TREE_VAL_POISON) ? DelTreeNode (left) : LinkLeft  (left, node);
        GraphDump (node);
        
        ParseRec (treeFile, right, nParsingNode, strings);
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
    fprintf (dotFile, "\tnode%p[shape=Mrecord, style=filled, fillcolor=\"#%zu\" label = \"{ptr = %p | value =" IOTYPE "| parent = %p | left = %p | right = %p}\"];\n",
                         node, (size_t) (node + 7) * 7 % 0x1000000, node, node->value, node->parent, node->left, node->right);
    if (node->left)  CreateNodes (node->left, dotFile);
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