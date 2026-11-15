#include "akinator.h"
#include "raylib/raylib.h"
#include <cstdio>

void Menu (tree* tree)
{
    float screenWidth = 1000;
    float screenHeight = 600;
    
    InitWindow (screenWidth, screenHeight, "menu");

    int answer = 0;
    while (!WindowShouldClose ())
    {
        BeginDrawing ();
        ClearBackground (RAYWHITE);

        if (!answer)
        {
            answer = DrawMenuButtons ();
            EndDrawing ();
            if (answer == 'C') break;
        }
        else
        {
            GraphModeMenu ();
        }
        
    }
    CloseWindow ();
    if (answer == 'C') ConsoleMenu (tree);
}

int DrawMenuButtons ()
{
    float btnWidth = 300.0f;
    float btnHeight = 100.0f;
    float offset = 25.0f;
    float round = 0.2f;
    int segments = 10;
    float thick = 3.0f;
    int textSize = 25;

    float totalHeight = btnHeight * 2 + offset;
    float startX = (GetScreenWidth () - btnWidth) / 2.0f;
    float startY = (GetScreenHeight () - totalHeight) / 2.0f;

    Rectangle visualBtn = { startX, startY, btnWidth, btnHeight };
    Rectangle consoleBtn = { startX, startY + btnHeight + offset, btnWidth, btnHeight };

    DrawRectangleRoundedLinesEx (visualBtn, round, segments, thick, BLACK);
    int textWidthVis = MeasureText("visual", textSize);
    DrawText("Visual", visualBtn.x + (btnWidth - textWidthVis) / 2.0f, visualBtn.y + (btnHeight - textSize) / 2.0f, textSize, BLACK);

    DrawRectangleRoundedLinesEx (consoleBtn, round, segments, thick, BLACK);
    int textWidthCon = MeasureText("console", textSize);
    DrawText("Console", consoleBtn.x + (btnWidth - textWidthCon) / 2.0f, consoleBtn.y + (btnHeight - textSize) / 2.0f, textSize, BLACK);

    Vector2 pos = GetMousePosition ();
    if (IsMouseButtonPressed (MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec (pos, visualBtn)) return 'V';
        else if (CheckCollisionPointRec (pos, consoleBtn)) return 'C';
    }

    return 0;
}

enum workType
{
    Guess = 1,
    Def = 2,
    Cmp = 3,
};

void ConsoleMenu (tree* tree)
{
    printf ("If you want to guess an object press 1\n"
            "If you want to get an object definition press 2"
            "If you want to compare two objects press 3");

    int answer = 0;
    while (!(answer > 0 && answer < 4))
    {
        if (!scanf ("%d", &answer) || !(answer > 0 && answer < 4)) printf ("Err, unsupported answer\n");
    }

    switch (answer)
    {
        case Guess :
        {
            GuessLogic (tree->root, badNode);
            break;
        }
        case Def :
        {
            Definition (tree->root);
            break;
        }
        case Cmp :
        {
            Compare (tree->root);
            break;
        }
    }
}

void GraphModeMenu (tree* tree)
{
    
}