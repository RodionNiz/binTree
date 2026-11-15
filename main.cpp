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
    Menu (newTree);
    /*
    GraphDump (newTree->root, nullptr);
    treeNode* badAnsNode = nullptr;
    treeNode* newAnsNode = nullptr;
    //TODO - return
    GraphInt (newTree->root, &badAnsNode);
    if (badAnsNode)
    {
        GraphDump (newTree->root, nullptr);

        FILE* newData = fopen (fileName, "w");
        FDumpTreePrev (newData, newTree->root);
        fclose (newData);
    }

    //TODO - флаг
    if (newAnsNode)
    {
        free (newAnsNode->value);
        free (newAnsNode->parent->value);
    }
    */
    DeleteTree (newTree);
    DelTextStates (states);
    
    fclose (file);
}