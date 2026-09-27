#include <raylib.h>

Color green = {173, 204, 96, 255};
Color dark_green = {43, 51, 24, 255};

int main() 
{
    
    InitWindow(750, 750, "Retro Snake");
    
    SetTargetFPS(60);

    while (WindowShouldClose() == false)
    {
        BeginDrawing();


        ClearBackground(green);

        EndDrawing();
    }
    
    CloseWindow();

    return 0;    
}