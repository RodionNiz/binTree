#include "akinator.h"
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

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

#endif

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

    RecParChec (node, &ctx);
    if (ctx.badNode) GraphDump (node, &ctx);
    return ctx.code;
}

void RecParChec (treeNode* node, errCtx* ctx) 
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
        RecParChec (node->left, ctx);
        if (ctx->code != TreeOk) return;
    }
    if (node->right)
    {
        RecParChec (node->right, ctx);
        if (ctx->code != TreeOk) return;
    }
    return; 
}


inline void BufferClean ()
{
    while (1)
    {
        int tempChar = getchar ();                                  
        if (tempChar == '\n') break;
    }
}

void GuessLogic (treeNode* node, treeNode** badNode)
{
    VERIFY_TREE (node);

    printf ("%s?\nY/N:\n", node->value);
    char ans = GetYorN ();

    if (ans == 'Y')
    {
        if (node->left) GuessLogic (node->left, badNode);
        else printf ("HAHAHA idi gylai\n");
        return;
    } 

    if (ans == 'N')
    {
        if (node->right) GuessLogic (node->right, badNode);
        else *badNode = node;
        return;
    } 

    VERIFY_TREE (node);
}

#define ANSLEN 20
treeNode* NewAnswer (treeNode* ansNode)
{
    VERIFY_TREE (ansNode);

    printf ("Vot yrod. Kakoy otvet?\n");
    char* newAns = (char*) calloc (ANSLEN, sizeof (char));
    if (!newAns) printf ("new ans buffer allocation err\n");
    scanf ("%[^\n]", newAns);
    BufferClean ();
    treeNode* newAnsNode = NewTreeNode (newAns);

    treeNode* oldAnsNode = NewTreeNode (ansNode->value);

    printf ("Chem %s otlichaetsa ot %s?\n V otlichii ot %s on(a):\n", ansNode->value, newAnsNode->value, ansNode->value);
    char* newTag = (char*) calloc (ANSLEN, sizeof (char));
    if (!newTag) printf ("new tag buffer allocation err\n");
    scanf ("%[^\n]", newTag);
    BufferClean ();
    ansNode->value = newTag;

    LinkLeft  (newAnsNode, ansNode);
    LinkRight (oldAnsNode, ansNode);
    
    VERIFY_TREE (ansNode);

    return newAnsNode;
}
#undef ANSLEN

char GetYorN ()
{
    char ans = 0;
    while (1)
    {
        scanf ("%c", &ans);
        BufferClean ();
        if (ans == 'Y' || ans == 'N') break;
        printf ("Yor answer has unsupported type!\nYou can answer Y/N\n");
    }
    return ans;
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
    tree* tree = NewTree (GLOBAL_TREE_VAL_POISON);

    size_t nOfParsStr = 0;
    tree->nOfNode = nOfParsStr;
    ParseRec (treeFile, tree->root, &nOfParsStr, strings);

    VERIFY_TREE (tree->root);
    return tree;
}

void ParseRec (FILE* treeFile, treeNode* node, size_t* nParsingNode, String* strings)
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
 
        treeNode* left  = NewTreeNode (GLOBAL_TREE_VAL_POISON);
        treeNode* right = NewTreeNode (GLOBAL_TREE_VAL_POISON);
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
        fprintf (dotFile, "\tnode%p -> node%p [label = \"da\", color = \"green\"];\n", node, node->left);
        CreateLinks (node->left, dotFile);
    }

    if (node->right)
    {
        fprintf (dotFile, "\tnode%p -> node%p [label = \"net\", color = \"red\"];\n", node, node->right);
        CreateLinks (node->right, dotFile);
    }
}

treeNode* FindNode (treeNode* node, const char* object)
{
    if (strcmp(object, node->value) == 0)
    {
        return node;
    }

    treeNode* tempNode = nullptr;
    if (node->left)
    {
        tempNode = FindNode (node->left, object);
        if (tempNode) return tempNode;
    }
    
    if (node->right)
    {
        tempNode = FindNode (node->right, object);
        if (tempNode) return tempNode;
    }

    return nullptr;
}

treeNode** GetWay (treeNode* node, int* deep)
{
    int maxWaySize = 10;
    treeNode** way = (treeNode**) calloc (maxWaySize, sizeof (treeNode*));
    treeNode* curNode = node;
    int waySize = 0;

    do
    {
        ++*deep;
        way [waySize++] = curNode->parent;
        curNode = curNode->parent;
    } while (curNode->parent != curNode);

    return way;
}

void Opredelenie (treeNode* node)
{
    printf ("%s on\n", node->value);
    int deep = 0;
    treeNode** way = GetWay (node, &deep); 
    for (int i = deep; i > 0; i--) 
    {
        printf ("%s\n", way [i - 1]->value);
    }
}

void Difference (treeNode* node1, treeNode* node2)
{
    int deep1 = 0;
    treeNode** way1 = GetWay (node1, &deep1); 
    int deep2 = 0;
    treeNode** way2 = GetWay (node2, &deep2); 

    printf ("I %s i %s oba:\n", node1->value, node2->value);
    while (way1 [deep1 - 1] == way2 [deep2 - 1])
    {
        printf ("%s\n", way1 [deep1 - 1]->value);
        deep1--;
        deep2--;
    }

    if (deep1 == deep2 && deep1 == 0) return;
    
    printf ("No!\n");
    if (deep1 != 0)
    {
        printf ("%s on\n", node1->value);
        for (int i = deep1; i > 0; i--) 
        {
            printf ("%s\n", way1 [i - 1]->value);
        }
    }
    
    if (deep2 != 0)
    {
        printf ("A %s on\n", node1->value);
        for (int i = deep2; i > 0; i--) 
        {
            printf ("%s\n", way2 [i - 1]->value);
        }
    }
}
