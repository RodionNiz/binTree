#include "akinator.h"
#include "raylib/raylib.h"
#include <cstdio>

void Menu (tree* tree, treeNode** badAnsNode)
{
    float screenWidth = 1000;
    float screenHeight = 600;
    
    InitWindow (screenWidth, screenHeight, "menu");

    int answer = 0;
    while (!WindowShouldClose ())
    {
        BeginDrawing ();
        ClearBackground (RAYWHITE);
        if (!answer) answer = DrawMenuButtons ();
        else 
        {
            EndDrawing ();
            break;
        }
        EndDrawing ();
    }

    if (answer == 'C') 
    {
        CloseWindow ();
        ConsoleMenu (tree, badAnsNode);
        return;
    }
    else
    {
        answer = 0;
        while (!WindowShouldClose ())
        {
            BeginDrawing ();
            ClearBackground (RAYWHITE);
            if (!answer) answer = GraphModeMenu ();
            else
            {
                EndDrawing ();
                break;
            }
            EndDrawing ();
        }
        switch (answer)
        {
            case Guess:
            {
                GraphInt (tree->root, badAnsNode);
                break;
            } 
            case Def:
            {
                GraphDef (tree->root);
                break;
            }
            case Cmp:
            {
                GraphCmp (tree->root);
                break;
            }
            default:
            {
                break;
            }
        }
    }
    CloseWindow ();
}

int DrawMenuButtons ()
{
    float btnWidth = 300.0f;
    float btnHeight = 100.0f;
    float offset = 25.0f;

    float totalHeight = btnHeight * 2 + offset;
    float startX = (GetScreenWidth () - btnWidth) / 2.0f;
    float startY = (GetScreenHeight () - totalHeight) / 2.0f;

    Rectangle visualBtn = {startX, startY, btnWidth, btnHeight};
    Rectangle consoleBtn = {startX, startY + btnHeight + offset, btnWidth, btnHeight};
    button btnVis = 
    {
        .rect = {startX, startY, btnWidth, btnHeight},
        .text = "Visual"
    };
    button btnCns = 
    {
        .rect = {startX, startY + btnHeight + offset, btnWidth, btnHeight},
        .text = "Console"
    };

    DrawButton (btnVis);
    DrawButton (btnCns);
    
    if (IsMouseButtonPressed (MOUSE_LEFT_BUTTON))
    {
        Vector2 pos = GetMousePosition ();
        if (CheckCollisionPointRec (pos, visualBtn)) return 'V';
        else if (CheckCollisionPointRec (pos, consoleBtn)) return 'C';
    }

    return 0;
}

void ConsoleMenu (tree* tree, treeNode** badAnsNode)
{
    printf ("\n\n\nIf you want to guess an object press 1\n"
            "If you want to get an object definition press 2\n"
            "If you want to compare two objects press 3\n\n");

    int answer = 0;
    while (!(answer > 0 && answer < 4))
    {
        if (!scanf ("%d", &answer) || !(answer > 0 && answer < 4)) printf ("Err, unsupported answer\n");
        BufferClean ();
    }

    switch (answer)
    {
        case Guess :
        {
            GuessLogic (tree->root, badAnsNode);
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
        default :
        {
            break;
        }
    }
}

workType GraphModeMenu ()
{
    float btnWidth = 350.0f;
    float btnHeight = 100.0f;
    float offset = 25.0f;

    float totalHeight = btnHeight * 3 + offset * 2;
    
    float startX = (GetScreenWidth () - btnWidth) / 2.0f;
    float startY = (GetScreenHeight () - totalHeight) / 2.0f;

    button buttonAkin = 
    {
        .rect = {startX, startY, btnWidth, btnHeight},
        .text = "akinator"
    };

    button buttonDef = 
    {
        .rect = {startX, startY + btnHeight + offset, btnWidth, btnHeight},
        .text = "definition"
    };

    button buttonCmp = 
    {
        .rect = {startX, startY + 2 * (btnHeight + offset), btnWidth, btnHeight},
        .text = "compare"
    };

    DrawButton (buttonAkin);
    DrawButton (buttonDef);
    DrawButton (buttonCmp);

    if (IsMouseButtonPressed (MOUSE_LEFT_BUTTON))
    {
        Vector2 pos = GetMousePosition ();
        if (CheckCollisionPointRec (pos, buttonAkin.rect)) return Guess;
        else if (CheckCollisionPointRec (pos, buttonDef.rect)) return Def;
        else if (CheckCollisionPointRec (pos, buttonCmp.rect)) return Cmp;
    }

    return No;
}
