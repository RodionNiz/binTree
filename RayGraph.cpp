#include "akinator.h"
#include "raylib/raylib.h"
#include <cstdio>
#include <cstdlib>

void GraphInt (treeNode* node, treeNode** badAnsNode)
{
    float screenWidth = GetScreenWidth ();
    float screenHeight = GetScreenHeight ();
    float buttonWidth = 450;
    float buttonHeigh = 150 ;
    float offset = 25;

    float btnPosY = screenHeight - buttonHeigh - offset;

    button btnY = 
    {
        .rect = {offset, btnPosY, buttonWidth, buttonHeigh},
        .text = "yes"
    };

    button btnN = 
    {
        .rect = {screenWidth - buttonWidth - offset, btnPosY, buttonWidth, buttonHeigh},
        .text = "no"
    };

    BeginDrawing ();
    DrawFrame (node, btnY, btnN);
    EndDrawing ();
    int guessed = 0;

    while (!WindowShouldClose ())
    {
        BeginDrawing ();
        char answer = GetYorNGraph (btnY, btnN);
        
        if (answer == 'Y')
        {
            if (node->left) node = node->left;
            else guessed = 1;
        }
        else if (answer == 'N')
        {
            if (node->right) node = node->right;
            else 
            {
                *badAnsNode = node;
                EndDrawing ();
                SplitGraph (*badAnsNode);
                break;
            }
        }

        if (!guessed) DrawFrame (node, btnY, btnN);
        else DrawCorAns ();
        EndDrawing ();
    }
}

void DrawFrame (treeNode* node, button btnY, button btnN)
{
    ClearBackground (RAYWHITE);
    DrawButton (btnY);
    DrawButton (btnN);
    DrawQuest (node);
}

void DrawButton (button btn) 
{
    float round = 0.2f;
    int segments = 10;
    float thick = 3.0f;
    DrawRectangleRoundedLinesEx (btn.rect, round, segments, thick, BLACK);
    if (!btn.text)
    {
        printf ("no text\n");
        return;
    }

    int textSize = btn.rect.height / 2;
    int offset = 20;
    int maxTextWidth = btn.rect.width - offset;
    int textWidth = MeasureText (btn.text, textSize);

    while (textWidth > maxTextWidth) 
    {
        textSize--;
        textWidth = MeasureText (btn.text, textWidth);
    }

    int posTextX = btn.rect.x + (btn.rect.width - textWidth) / 2;
    int posTextY = btn.rect.y + (btn.rect.height - textSize) / 2;

    DrawText(btn.text, posTextX, posTextY, textSize, BLACK);
}


void DrawQuest (treeNode* node)
{
    ClearBackground (RAYWHITE);
    int textSize = 75;
    int offset = MeasureText (node->value, textSize);
    DrawText (node->value, (GetScreenWidth () - offset) / 2, GetScreenHeight () / 2 - textSize, textSize, BLACK);
    DrawText ("?", (GetScreenWidth () + offset) / 2 + 10, GetScreenHeight () / 2 - textSize, textSize, BLACK);
}

void DrawCorAns ()
{
    ClearBackground (RAYWHITE);
    int textSize = 75;
    int offset = MeasureText ("Lox cvetochnyi", textSize);
    DrawText ("Lox cvetochnyi", (GetScreenWidth () - offset) / 2, GetScreenHeight () / 2 - textSize, textSize, BLACK);
}

char GetYorNGraph (button btnY, button btnN)
{
    Vector2 pos = GetMousePosition ();
    if (IsMouseButtonPressed (MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec (pos, btnY.rect)) return 'Y';
        else if (CheckCollisionPointRec (pos, btnN.rect)) return 'N';
    }
    return 0;
}

void GetInputString (const char* prompt, char* buffer, int maxLen)
{
    int letterCount = 0;
    buffer [0] = '\0';
    while (!WindowShouldClose ())
    {
        int curChar = GetCharPressed ();
        while (curChar > 0)
        {
            if (((curChar >= 'A' && curChar <= 'z') || curChar == ' ') && letterCount < maxLen - 1)
            {
                buffer [letterCount] = (char) curChar;
                buffer [letterCount + 1] = '\0';
                letterCount++;
            }
            curChar = GetCharPressed ();
        }
        if (IsKeyPressed (KEY_BACKSPACE))
        {
            if (letterCount > 0)
            {
                letterCount--;
                buffer [letterCount] = '\0';
            }
        }

        if (IsKeyPressed (KEY_ENTER) && letterCount > 0) return;
        
        BeginDrawing ();
        ClearBackground (RAYWHITE);

        int textSize = 40;
        int inputWidth = MeasureText (prompt, textSize);
        DrawText (prompt, (GetScreenWidth () - inputWidth) / 2, 150, textSize, BLACK);
        Rectangle inputBox = {150, 250, 700, 80};
        
        DrawRectangleLinesEx (inputBox, 3, BLACK);
        DrawText (buffer, inputBox.x + 15, (int) inputBox.y + textSize / 2, textSize, BLACK);
        if (letterCount < maxLen - 1)
        {
            int textWidth = MeasureText (buffer, textSize);
            DrawText ("_", inputBox.x + 15 + textWidth, (int) inputBox.y + textSize / 2, textSize, RED);
        }
        EndDrawing ();
    }
}

#define ANSLEN 64
void SplitGraph (treeNode* nodeToSplit)
{
    char* newAns = (char*) calloc (ANSLEN, sizeof (char));
    if (!newAns) printf ("new ans buffer allocation err\n");
    GetInputString ("Vot yrod. Kakoy otvet?", newAns, ANSLEN);
    treeNode* newAnsNode = NewTreeNode (newAns);
    
    char* newTag = (char*) calloc (ANSLEN, sizeof (char));
    if (!newTag) printf ("new tag buffer allocation err\n");

    char* prompt = (char*) calloc (100, sizeof (char));  
    sprintf (prompt,"Chem %s otlichaetsa ot %s?\n V otlichii ot %s on(a):\n", newAnsNode->value, nodeToSplit->value, nodeToSplit->value);
    GetInputString ((const char*) prompt, newTag, ANSLEN);
    free (prompt);

    treeNode* oldAnsNode = NewTreeNode (nodeToSplit->value);
    nodeToSplit->value = newTag;

    LinkLeft (newAnsNode, nodeToSplit);
    LinkRight (oldAnsNode, nodeToSplit);
}

void GraphDef (treeNode* node)
{
    treeNode* desiredNode = GetNodeByStrGraph (node, "Input the object");
    if (!desiredNode) return;

    int deep = 0;
    wayCtx way = {.way = GetWay (desiredNode, &deep), .deep = deep};
    if (!way.way) return;

    Vector2 printPos = {.x = 10, .y = 10};

    char header [ANSLEN + 8] = {};
    sprintf (header, "%s on(a):", desiredNode->value);

    while (!WindowShouldClose ())
    {   
        BeginDrawing ();
        ClearBackground (RAYWHITE);
        PrintWayGraph (way, &printPos, desiredNode, header);
        EndDrawing ();
    }

    free (way.way);
}

treeNode* GetNodeByStrGraph (treeNode* node, const char* prompt)
{
    char obj [ANSLEN] = {};
    GetInputString (prompt, obj, ANSLEN);
    treeNode* desiredNode = FindNode (node, obj);

    while (!desiredNode && !WindowShouldClose())
    {
        const char* message = "No such object";
        GetInputString (message, obj, ANSLEN);
        desiredNode = FindNode (node, obj);
    }
    return desiredNode;
}

#define WAYTEXTSIZE 40

int PrintWayGraph (wayCtx way, Vector2* printPos, treeNode* node, const char* header)
{
    int strOffset = WAYTEXTSIZE / 4;
    char curStr [ANSLEN] = {};
    int printedStr = 0;

    DrawText (header, printPos->x, printPos->y, WAYTEXTSIZE, BLACK);
    printPos->y += strOffset + WAYTEXTSIZE;
    printedStr++;

    for (int i = way.deep - 1; i > 0; i--) 
    {
        if (way.way [i]->left == way.way [i - 1]) sprintf (curStr, "%s", way.way [i]->value);   
        else sprintf (curStr, "ne %s", way.way [i]->value);
        
        DrawText (curStr, printPos->x, printPos->y, WAYTEXTSIZE, BLACK);
        printPos->y += strOffset + WAYTEXTSIZE;
        printedStr++;
    }

    if (way.way [0]->left == node) sprintf (curStr, "%s", way.way [0]->value);
    else sprintf (curStr, "ne %s", way.way [0]->value);

    DrawText (curStr, printPos->x, printPos->y, WAYTEXTSIZE, BLACK);
    printPos->y += strOffset + WAYTEXTSIZE;
    printedStr++;

    return printedStr;
}

int PrintSameWayGraph (wayCtx way0, wayCtx way1, Vector2* printPos, const char* header)
{
    int strOffset = WAYTEXTSIZE / 4;

    char curStr [ANSLEN] = {};
    int printedStr = 0;

    DrawText (header, printPos->x, printPos->y, WAYTEXTSIZE, BLACK);
    printPos->y += strOffset + WAYTEXTSIZE;
    printedStr++;

    while (way0.deep > 1 && way1.deep > 1 && way0.way [way0.deep - 2] == way1.way [way1.deep - 2])
    {
        if ( way0.way [way0.deep - 1]->left ==  way0.way [way0.deep - 2]) sprintf (curStr, "%s\n",  way0.way [way0.deep - 1]->value);
        else sprintf (curStr ,"ne %s\n",  way0.way [way0.deep - 1]->value);
        way0.deep--;
        way1.deep--;

        DrawText (curStr, printPos->x, printPos->y, WAYTEXTSIZE, BLACK);
        printPos->y += strOffset + WAYTEXTSIZE;
        printedStr++;
    }

    return printedStr;
}
#undef WAYTEXTSIZE

void GraphCmp (treeNode* node)
{
    treeNode* node0 = GetNodeByStrGraph (node, "Input first object");
    if (!node0) return;
    treeNode* node1 = GetNodeByStrGraph (node, "Input second object");
    if (!node1) return;
    
    int deep0 = 0;
    int deep1 = 0;
    wayCtx way0Ctx = {.way = GetWay (node0, &deep0), .deep = deep0};
    wayCtx way1Ctx = {.way = GetWay (node1, &deep1), .deep = deep1};
    if (!way0Ctx.way) return;
    if (!way1Ctx.way) return;
    
    while (!WindowShouldClose ())
    {
        wayCtx curCtx0 = way0Ctx;
        wayCtx curCtx1 = way1Ctx;

        char buffer [2 * ANSLEN + 11] = {};
        Vector2 printPos = {.x = 10, .y = 10};

        ClearBackground (RAYWHITE);
        BeginDrawing ();

        sprintf (buffer ,"I %s i %s oba:", node0->value, node1->value);
        int sameDeep = PrintSameWayGraph (curCtx0, curCtx1, &printPos, buffer) - 1;
        curCtx0.deep -= sameDeep;
        curCtx1.deep -= sameDeep;

        if (curCtx0.deep)
        {
            sprintf (buffer, "%s on(a)\n", node0->value);
            PrintWayGraph (curCtx0, &printPos, node0, buffer);
        }
        if (curCtx1.deep)
        {
            sprintf (buffer, "%s on(a)\n", node1->value);
            PrintWayGraph (curCtx1, &printPos, node1, buffer);
        }

        EndDrawing ();
    }
    
    free (way0Ctx.way);
    free (way1Ctx.way);
}

#undef ANSLEN