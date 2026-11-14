#include "akinator.h"

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
#undef ANSLEN