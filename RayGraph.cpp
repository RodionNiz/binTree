#include "akinator.h"

void GraphInt (treeNode* node, treeNode** badAnsNode)
{
    float screenWidth = 1000;
    float screenHeight = 600;
    float buttonWidth = 450;
    float buttonHeigh = 150 ;
    float offset = 25;
    Rectangle buttonY = {.x = offset, .y = screenHeight - offset - buttonHeigh, .width = buttonWidth, .height = buttonHeigh};
    Rectangle buttonN = {.x = screenWidth - offset - buttonWidth, .y = screenHeight - offset - buttonHeigh, .width = buttonWidth, .height = buttonHeigh};

    windowCtx ctx = {.screenWidth = screenWidth, .screenHeight = screenHeight, .buttonY = buttonY, .buttonN = buttonN};

    InitWindow (screenWidth, screenHeight, "akinator");

    BeginDrawing ();
    DrawFrame (ctx, node);
    EndDrawing ();
    int guessed = 0;

    while (!WindowShouldClose ())
    {
        BeginDrawing ();
        char answer = GetYorNGraph (ctx);
        
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

        if (!guessed) DrawFrame (ctx, node);
        else DrawCorAns (ctx);
        EndDrawing ();
    }
    CloseWindow();
}

void DrawFrame (windowCtx ctx, treeNode* node)
{
    ClearBackground (RAYWHITE);
    DrawButtons (ctx);
    DrawQuest (ctx, node);
}

void DrawButtons (windowCtx ctx)
{
    float round = 0.2f;
    int segments = 10;
    float thick = 3.0f;
    DrawRectangleRoundedLinesEx (ctx.buttonY, round, segments, thick, BLACK);
    DrawRectangleRoundedLinesEx (ctx.buttonN, round, segments, thick, BLACK);
}

void DrawQuest (windowCtx ctx, treeNode* node)
{
    ClearBackground (RAYWHITE);
    int textSize = 75;
    int offset = MeasureText (node->value, textSize);
    DrawText (node->value, (GetScreenWidth () - offset) / 2, GetScreenHeight () / 2 - textSize, textSize, BLACK);
    DrawText ("?", (GetScreenWidth () + offset) / 2 + 10, GetScreenHeight () / 2 - textSize, textSize, BLACK);
}

void DrawCorAns (windowCtx ctx)
{
    ClearBackground (RAYWHITE);
    int textSize = 75;
    int offset = MeasureText ("Lox cvetochnyi", textSize) / 2;
    DrawText ("Lox cvetochnyi", GetScreenWidth () - offset, GetScreenHeight () / 2 - textSize, textSize, BLACK);
}

char GetYorNGraph (windowCtx ctx)
{
    Vector2 pos = GetMousePosition ();
    if (IsMouseButtonPressed (MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec (pos, ctx.buttonY)) return 'Y';
        else if (CheckCollisionPointRec (pos, ctx.buttonN)) return 'N';
    }
    return 0;
}
//TODO - висящие константы
void GetInputString (const char* prompt, char* buffer, int maxLen)
{
    int letterCount = 0;
    buffer [0] = '\0';
    while (!WindowShouldClose ())
    {
        int key = GetCharPressed ();
        while (key > 0)
        {
            if (((key >= 'A' && key <= 'z') || key == ' ') && letterCount < maxLen - 1)
            {
                buffer [letterCount] = (char) key;
                buffer [letterCount + 1] = '\0';
                letterCount++;
            }
            key = GetCharPressed ();
        }

        if (IsKeyPressed (KEY_BACKSPACE))
        {
            if (letterCount > 0)
            {
                letterCount--;
                buffer [letterCount] = '\0';
            }
        }

        if (IsKeyPressed (KEY_ENTER) && letterCount > 0) break;
        
        BeginDrawing ();
        ClearBackground (RAYWHITE);
        
        int inputWidth = MeasureText (prompt, 40);
        DrawText (prompt, (GetScreenWidth () - inputWidth) / 2, 150, 75, DARKGRAY);

        Rectangle inputBox = { 150, 250, 700, 80 };
        DrawRectangleLinesEx (inputBox, 3, BLACK);

        DrawText(buffer, (int) inputBox.x + 15, (int) inputBox.y + 20, 40, BLACK);
        if (((int) (GetTime () * 2) % 2) == 0 && letterCount < maxLen - 1)
        {
            int textWidth = MeasureText (buffer, 40);
            DrawText ("_", (int) inputBox.x + 15 + textWidth, (int) inputBox.y + 20, 40, RED);
        }

        DrawText ("Press ENTER to confirm", 330, 360, 25, GRAY);

        EndDrawing ();
    }
}

#define ANSLEN 20
void SplitGraph (treeNode* nodeToSplit)
{
    char* newAns = (char*) calloc (ANSLEN, sizeof (char));
    if (!newAns) printf ("new ans buffer allocation err\n");
    GetInputString ("Vot yrod. Kakoy otvet?", newAns, ANSLEN);
    treeNode* newAnsNode = NewTreeNode (newAns);

    
    char* newTag = (char*) calloc (ANSLEN, sizeof (char));
    if (!newTag) printf ("new tag buffer allocation err\n");
    char* prompt = (char*) calloc (100, sizeof (char));  
    sprintf (prompt,"Chem %s otlichaetsa ot %s?\n V otlichii ot %s on(a):\n", nodeToSplit->value, newAnsNode->value, nodeToSplit->value);
    GetInputString ((const char*) prompt, newAns, ANSLEN);

    treeNode* oldAnsNode = NewTreeNode (nodeToSplit->value);
    nodeToSplit->value = prompt;
}
#undef ANSLEN