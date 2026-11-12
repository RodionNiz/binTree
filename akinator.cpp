#include "akinator.h"

treeNode* NewTreeNode (value_t value)
{
    treeNode* newNode = (treeNode*) calloc (1, sizeof (treeNode));
    if (!newNode) 
    {
        printf ("New node allocation err\n");
        return nullptr;
    }

    newNode->value = value;
    GraphDump (newNode);
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
    assert (child->parent == nullptr);

    parent->left = child;
    child->parent = parent;
    GraphDump (parent);
}

void LinkRight (treeNode* child, treeNode* parent)
{
    assert (child != nullptr);
    assert (parent != nullptr);
    assert (parent->right == nullptr);
    assert (child->parent == nullptr);

    parent->right = child;
    child->parent = parent;
    GraphDump (parent);
}
//TODO - verify

errCtx VerifyTree (tree* tree)
{
    errCtx ctx = {.badNode = nullptr, .code = TreeOk};
    if (tree->root->left)  RecParChec (tree->root->left,  &ctx);
    if (tree->root->right) RecParChec (tree->root->right, &ctx);
    if (ctx.badNode) 
    {
        GraphDump (tree->root);
        return ctx;
    }
    return ctx;
}

void RecParChec (treeNode* node, errCtx* ctx) 
{

    if (!node->parent) 
    {
        ctx->badNode = node;
        ctx->code = ParentErr;
        return;
    }

    if (node != node->parent->left && node != node->parent->right)
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
        if (tempChar == '\n' || tempChar == EOF)
        {
            break;
        }
    }
}

void GuessLogic (treeNode* node, treeNode** badNode)
{
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
}

#define ANSLEN 20
//TODO - подумать над структурой
treeNode* NewAnswer (treeNode* ansNode)
{
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

void DupmTreePrev (treeNode* node)
{
    if (*node->value != '\0')
    {
        printf ("(");
        printf (IOTYPE, node->value);
        if (node->left)  DupmTreePrev (node->left);
        else printf ("()");
        if (node->right) DupmTreePrev (node->right);
        else printf ("()");
        printf (")");
    }
}

void DupmTreeIn (treeNode* node)
{
    printf ("(");
    if (node->left)  DupmTreeIn (node->left);
    printf (IOTYPE, node->value);
    if (node->right) DupmTreeIn (node->right);
    printf (")");
}

tree* BuildPrev (FILE* treeFile, String* strings)
{
    fseek (treeFile, 0, SEEK_SET);
    tree* tree = NewTree (GLOBAL_TREE_VAL_POISON);

    size_t nOfParsStr = 0;
    ParseRec (treeFile, tree->root, &nOfParsStr, strings);
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