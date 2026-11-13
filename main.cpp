#include "akinator.cpp"
#include <cstdio>


int main (int argc, char* argv [])
{
    const char* fileName = ((argc - 1) >= 1) ? argv [1] : "tree.txt";
    FILE* file = fopen (fileName, "r");

    TextStates* states = ReadText (file, fileName);

    tree* newTree = BuildPrev (file, states->strings);

    GraphDump (newTree->root, nullptr);

    treeNode* node1 = FindNode (newTree->root, "Igor Sizanov");
    treeNode* node2 = FindNode (newTree->root, "savelyi");

    Difference (node1, node2);

    treeNode* badAnsNode = nullptr;
    treeNode* newAnsNode = nullptr;
    //GuessLogic (newTree->root, &badAnsNode);
    if (badAnsNode) 
    {
        newAnsNode = NewAnswer (badAnsNode);
        GraphDump (newTree->root, nullptr);

        FILE* newData = fopen (argv [1], "w");
        FDumpTreePrev (newData, newTree->root);
        fclose (newData);
    }

    if (newAnsNode) free (newAnsNode->value);
    DelTextStates (states);
    fclose (file);
}