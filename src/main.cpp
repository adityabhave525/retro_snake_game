#include <raylib.h>

Color green = {173, 204, 96, 255};
Color dark_green = {43, 51, 24, 255};

int cell_size = 30;
int cell_count = 25;

class Food
{
public:
    Vector2 position;
    Texture2D texture;

    Food()
    {
        Image image = LoadImage("Graphics/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPos();
    }

    ~Food()
    {
        UnloadTexture(texture);
    }

    void Draw()
    {
        DrawTexture(texture, position.x * cell_size, position.y * cell_size, WHITE);
    }

    Vector2 GenerateRandomPos()
    {
        float x = GetRandomValue(0, cell_count - 1);
        float y = GetRandomValue(0, cell_count - 1);

        return Vector2{x, y};
    }
};

int main()
{

    InitWindow(cell_size * cell_count, cell_size * cell_count, "Retro Snake");

    SetTargetFPS(60);

    Food food = Food();

    while (WindowShouldClose() == false)
    {
        BeginDrawing();

        ClearBackground(green);
        food.Draw();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}