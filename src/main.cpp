#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

Color green = {173, 204, 96, 255};
Color dark_green = {43, 51, 24, 255};

int cell_size = 30;
int cell_count = 25;
int offset = 75;

double last_update_time = 0;

bool ElementInDeque(Vector2 element, std::deque<Vector2> deque)
{
    for (unsigned int i = 0; i < deque.size(); i++)
    {
        if (Vector2Equals(deque[i], element))
        {
            return true;
        }
    }
    return false;
}

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
    bool add_segment = false;

    void Draw()
    {
        for (unsigned int i = 0; i < body.size(); i++)
        {
            float x = body[i].x;
            float y = body[i].y;

            Rectangle segment = Rectangle{offset + x * cell_size, offset + y * cell_size, (float)cell_size, (float)cell_size};

            DrawRectangleRounded(segment, 0.5, 6, dark_green);
        }
    }

    void Update()
    {
        body.push_front(Vector2Add(body[0], direction));

        if (add_segment == true)
        {
            add_segment = false;
        }
        else
        {
            body.pop_back();
        }
    }

    void Reset()
    {
        body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
        direction = {1, 0};
    }
};

class Food
{
public:
    Vector2 position;
    Texture2D texture;

    Food(std::deque<Vector2> snake_body)
    {
        Image image = LoadImage("Graphics/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPos(snake_body);
    }

    ~Food()
    {
        UnloadTexture(texture);
    }

    void Draw()
    {
        DrawTexture(texture, offset + position.x * cell_size, offset + position.y * cell_size, WHITE);
    }

    Vector2 GenerateRandomCell()
    {
        float x = GetRandomValue(0, cell_count - 1);
        float y = GetRandomValue(0, cell_count - 1);

        return Vector2{x, y};
    }

    Vector2 GenerateRandomPos(std::deque<Vector2> snake_body)
    {

        Vector2 position = GenerateRandomCell();

        while (ElementInDeque(position, snake_body))
        {
            position = GenerateRandomCell();
        }

        return position;
    }
};

class Game
{
public:
    Snake snake = Snake();
    Food food = Food(snake.body);

    bool running = true;
    int score = 0;

    Sound eat_sound;
    Sound wall_sound;

    Game()
    {
        InitAudioDevice();
        eat_sound = LoadSound("Sounds/eat.mp3");
        wall_sound = LoadSound("Sounds/wall.mp3");
    }

    ~Game()
    {
        UnloadSound(eat_sound);
        UnloadSound(wall_sound);
        CloseAudioDevice();
    }

    void Draw()
    {
        food.Draw();
        snake.Draw();
    }

    void Update()
    {
        if (running == true)
        {
            snake.Update();
            CheckCollisionWithFood();
            CheckCollisionWithEdges();
            CheckCollisionWithTail();
        }
    }

    void CheckCollisionWithFood()
    {
        if (Vector2Equals(snake.body[0], food.position))
        {
            food.position = food.GenerateRandomPos(snake.body);
            snake.add_segment = true;
            score++;
            PlaySound(eat_sound);
        }
    }

    void CheckCollisionWithEdges()
    {
        if (snake.body[0].x == cell_count || snake.body[0].x == -1)
        {
            GameOver();
        }
        if (snake.body[0].y == cell_count || snake.body[0].y == -1)
        {
            GameOver();
        }
    }

    void GameOver()
    {
        snake.Reset();
        food.position = food.GenerateRandomPos(snake.body);
        running = false;
        score = 0;
        PlaySound(wall_sound);
    }

    void CheckCollisionWithTail()
    {
        std::deque<Vector2> headless_body = snake.body;
        headless_body.pop_front();
        if (ElementInDeque(snake.body[0], headless_body))
        {
            GameOver();
        }
    }
};

int main()
{

    InitWindow(2 * offset + cell_size * cell_count, 2 * offset + cell_size * cell_count, "Retro Snake");

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
            game.running = true;
        }

        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
        {
            game.snake.direction = {0, 1};
            game.running = true;
        }

        if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
        {
            game.snake.direction = {-1, 0};
            game.running = true;
        }

        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
        {
            game.snake.direction = {1, 0};
            game.running = true;
        }

        ClearBackground(green);
        DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, (float)cell_size * cell_count + 10, (float)cell_size * cell_count + 10}, 5, dark_green);

        DrawText("Retro Snake", offset - 5, 20, 40, dark_green);
        DrawText(TextFormat("%i", game.score), offset - 5, offset + cell_size * cell_count + 10, 40, dark_green);

        game.Draw();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}