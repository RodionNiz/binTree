#include "akinator.h"
#include <cstdio>

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
        NewAnswer (*badNode);
        return;
    } 

    VERIFY_TREE (node);
}

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

void BufferClean ()
{
    while (1)
    {
        int tempChar = getchar ();                                  
        if (tempChar == '\n') break;
    }
}

#define ANSLEN 20
void NewAnswer (treeNode* ansNode)
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
}


void Definition (treeNode* node)
{
    char obj1 [ANSLEN] = {};
    scanf ("%s", obj1);
    treeNode* node1 = FindNode (node, (const char*) obj1);
    //TODO - ввод до успеха

    treeNode* desiredNode = FindNode (node, object);
    printf ("%s on(a)\n", desiredNode->value);
    int deep = 0;
    treeNode** way = GetWay (desiredNode, &deep); 
    PrintfWay (way, desiredNode, deep);
}


void Compare (treeNode* node)
{
    char obj1 [ANSLEN] = {};
    char obj2 [ANSLEN] = {};
    scanf ("%s", obj1);
    scanf ("%s", obj2); 
    //TODO - ввод до успеха
    treeNode* node1 = FindNode (node, (const char*) obj1);
    treeNode* node2 = FindNode (node, (const char*) obj2);

    int deep1 = 0;
    treeNode** way1 = GetWay (node1, &deep1); 
    int deep2 = 0;
    treeNode** way2 = GetWay (node2, &deep2); 

    printf ("I %s i %s oba:\n", node1->value, node2->value);

    while (deep1 > 1 && deep2 > 1 && way1 [deep1 - 2] == way2 [deep2 - 2])
    {
        if (way1 [deep1 - 2]->left == way1 [deep1 - 1]) printf ("%s\n", way1 [deep1 - 1]->value);
        else printf ("ne %s\n", way1 [deep1 - 1]->value);
        deep1--;
        deep2--;
    }
    
    printf ("No!\n");
    if (deep1 != 0)
    {
        printf ("%s on(a)\n", node1->value);
        PrintfWay (way1, node2, deep1);
    }
    
    if (deep2 != 0)
    {
        printf ("A %s on(a)\n", node2->value);
        PrintfWay (way2, node2, deep2);
    }
}
#undef ANSLEN