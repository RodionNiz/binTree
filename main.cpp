#include "akinator.cpp"
#include "Menu.cpp"
#include "akinator.h"
#include <cstdio>


int main (int argc, char* argv [])
{
    const char* fileName = ((argc - 1) >= 1) ? argv [1] : "tree.txt";
    FILE* file = fopen (fileName, "r");
    TextStates* states = ReadText (file, fileName);
    
    tree* newTree = BuildPrev (file, states->strings);
    treeNode* badAnsNode = nullptr;
    Menu (newTree, &badAnsNode);
    
    if (badAnsNode)
    {
        FILE* newData = fopen (fileName, "w");
        FDumpTreePrev (newData, newTree->root);
        free (badAnsNode->value);
        free (badAnsNode->left->value);
    }
    
    DeleteTree (newTree);
    DelTextStates (states);
    
    fclose (file);
}