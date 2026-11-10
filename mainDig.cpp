#include "tree.cpp"

int main (int argc, char* argv [])
{
    FILE* file = fopen (argv [1], "r");
    tree* newTree = BuildPrev (argv [1]);
    GraphDump (newTree->root);
    DupmTreeIn (newTree->root);
}