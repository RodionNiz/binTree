#include "akinator.cpp"


int main (int argc, char* argv [])
{
    FILE* file = fopen (argv [1], "r");

    TextStates states = ParseText (file, argv [1]);

    printf ("endparse\n");
    tree* newTree = BuildPrev (argv [1], states.strings);

    GraphDump (newTree->root);

    for (int i = 0; i < states.nOfGoodStr; i++)
    {
        printf ("\'%s\'\n", states.strings [i].strPtr);
    }
}