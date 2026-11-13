#include <assert.h>
#include <cstdlib>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct String 
{
    char* strPtr;
    size_t len;
};

struct TextStates 
{
    size_t nOfStr;
    size_t nOfGoodStr;
    size_t textLen;
    char*  text; 
    String* strings;
};

TextStates* ReadText (FILE* readingFile, const char* const fileName);
size_t CountFileSize (const char* const fileName);
char*  ReadFile      (size_t* fileSize, FILE* file);
size_t CountStrings  (char* text, size_t textSize);
size_t FillIndex     (String* originalTextStrings, char* text, size_t textSize);
void   DelTextStates (TextStates* states);

TextStates* ReadText (FILE* readingFile, const char* const fileName)
{
    assert (readingFile != nullptr);
    assert (fileName != nullptr);

    TextStates* states = (TextStates*) calloc (1, sizeof (TextStates));

    states->textLen = CountFileSize (fileName);
    states->text    = ReadFile (&(states->textLen), readingFile);
    states->nOfStr  = CountStrings (states->text, states->textLen);
    states->strings = (String*) calloc (states->nOfStr, sizeof (states->strings [0]));
    if (!states->strings) printf ("strings arr calloc err\n");

    states->nOfGoodStr = FillIndex (states->strings, states->text, states->textLen);
    states->strings    = (String*) realloc (states->strings, states->nOfGoodStr * sizeof (states->strings [0]));
    if (!states->strings) printf ("strings arr realloc err\n");

    return states;
}

size_t CountFileSize (const char* const fileName)
{
    assert (fileName != nullptr);
    struct stat fileStat = {};
    assert (stat (fileName, &fileStat) != -1);

    return  fileStat.st_size;
}

char* ReadFile (size_t* fileSize, FILE* file)
{
    assert (file != nullptr);
    assert (fileSize != nullptr);

    char* text = (char*) calloc (*fileSize + 1, sizeof (text [0]));
    if (!text) 
    {
        printf ("text calloc err\n");
        return nullptr;
    }
    *fileSize = fread (text, sizeof (text [0]), *fileSize, file);

    text = (char*) realloc (text, *fileSize * sizeof (text [0]) + 1);
    if (!text) 
    {
        printf ("text realloc err\n");
        return nullptr;
    }
    text [*fileSize] = 0;

    return text;
}

size_t CountStrings (char* text, size_t textSize)
{
    assert (text != nullptr);
    assert (textSize != 0);

    size_t count = 0;
    size_t strCount = 0;

    char endOfStrMarker = ')';
    while (count < textSize)
    {
        if (text [count] == endOfStrMarker) strCount++;
        count++;
    }

    return strCount;
}

size_t FillIndex (String* originalTextStrings, char* text, size_t textSize)
{
    assert (originalTextStrings != nullptr);
    assert (text != nullptr);
    assert (textSize != 0);

    size_t count = 0;
    size_t strCount = 0;

    while (count < textSize)
    {
        while (text [count] != '(' && count < textSize) count++;
        if (count == textSize) return strCount;

        text [count] = '\0';
        String newStr = {};
        size_t strLen = 0;

        while (text [count] != ')' && text [count] != '(') 
        {
            strLen++;
            count++;
        }

        if (strLen)
        {
            newStr.len = strLen - 1;
            newStr.strPtr = text + count - strLen + 1;
        }

        while (text [count] == ')') text [count++] = '\0';

        originalTextStrings [strCount] = newStr;\
        strCount++;
    }

    return strCount;
}

void DelTextStates (TextStates* states)
{
    free (states->strings);
    free (states->text);
    free (states);
}