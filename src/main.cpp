#include <raylib.h>
#include <deque>
#include <raymath.h>

Color green = {173, 204, 96, 255};
Color dark_green = {43, 51, 24, 255};

int cell_size = 30;
int cell_count = 25;

double last_update_time = 0;

bool eventTriggered(double interval)
{
    double current_time = GetTime();
    if (current_time - last_update_time >= interval)
    {
        last_update_time = current_time;
        return true;
    }
    return false;
}

class Snake
{
public:
    std::deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    Vector2 direction = {1, 0};

    void Draw()
    {
        for (unsigned int i = 0; i < body.size(); i++)
        {
            float x = body[i].x;
            float y = body[i].y;

            Rectangle segment = Rectangle{x * cell_size, y * cell_size, (float)cell_size, (float)cell_size};

            DrawRectangleRounded(segment, 0.5, 6, dark_green);
        }
    }

    void Update()
    {
        body.pop_back();
        body.push_front(Vector2Add(body[0], direction));
    }
};

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

class Game
{
    public:
    Snake snake = Snake();
    Food food = Food();

    void Draw()
    {
        food.Draw();
        snake.Draw();
    }

    void Update()
    {
        snake.Update();
    }
};

int main()
{

    InitWindow(cell_size * cell_count, cell_size * cell_count, "Retro Snake");

    SetTargetFPS(60);

    Game game = Game();

    while (WindowShouldClose() == false)
    {
        BeginDrawing();

        if (eventTriggered(0.2))
        {
            game.Update();
        }

        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1)
        {
            game.snake.direction = {0, -1};
        }

        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
        {
            game.snake.direction = {0, 1};
        }

        if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
        {
            game.snake.direction = {-1, 0};
        }

        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
        {
            game.snake.direction = {1, 0};
        }

        ClearBackground(green);

        game.Draw();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}