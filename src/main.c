#include "raylib.h"
#include <stdio.h>
#include <time.h>
#include <math.h>

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 672
#define TARGET_FPS 60
#define SQUARE_SIZE 32
#define TITLE "snake.c"

Texture2D tileTexture;
Texture2D appleTexture;
Texture2D bodyTexture;
Texture2D headTexture;
Sound eatingSound;

Color fieldColor1 = {173, 214, 68, 255};
Color fieldColor2 = {166, 209, 61, 255};

typedef struct {
    int posX, posY;
} Apple;

typedef struct {
    int length;
    Vector2 body[SCREEN_WIDTH / SQUARE_SIZE * SCREEN_HEIGHT / SQUARE_SIZE];
    Color color;
    Vector2 speed;
} Snake;

void DrawField() {
    for (int i = 0; i < SCREEN_WIDTH / SQUARE_SIZE; i ++) {
        for (int j = 0; j < SCREEN_HEIGHT / SQUARE_SIZE; j ++) {
            int x = i * SQUARE_SIZE;
            int y = j * SQUARE_SIZE;
            DrawTexture(tileTexture, x, y, WHITE);
        }
    }
}

void DrawScore(int score) {
    DrawTexture(appleTexture, 20, SCREEN_HEIGHT + 20, WHITE);
    char text[32];
    sprintf(text, "%d", score);
    DrawText(text, 60, SCREEN_HEIGHT + 25, 32, BLACK);
}

void randomizeApple(Apple* apple) {
    int posX = GetRandomValue(0, (SCREEN_WIDTH / SQUARE_SIZE) - 1);
    int posY = GetRandomValue(0, (SCREEN_HEIGHT / SQUARE_SIZE) - 1);

    apple->posX = posX * SQUARE_SIZE;
    apple->posY = posY * SQUARE_SIZE;
}

void moveSnake(Snake* snake) {
    for (int i = snake->length - 1; i > 0; i--) {
        snake->body[i] = snake->body[i - 1];
    }

    snake->body[0].x += snake->speed.x;
    snake->body[0].y += snake->speed.y;
}

void DrawSnake(Snake* snake) {
    for (int i=0; i<snake->length; i++) {
        if (i == 0) DrawTexture(headTexture, snake->body[i].x, snake->body[i].y, WHITE);
        else DrawTexture(bodyTexture, snake->body[i].x, snake->body[i].y, WHITE);
    }
}

void LoadTextures() {
    tileTexture = LoadTexture("resources/tile.png");
    headTexture = LoadTexture("resources/head.png");
    bodyTexture = LoadTexture("resources/body.png");
    appleTexture = LoadTexture("resources/apple.png");
    appleTexture.height = SQUARE_SIZE;
    appleTexture.width = SQUARE_SIZE;
    tileTexture.height = SQUARE_SIZE;
    tileTexture.width = SQUARE_SIZE;
    headTexture.height = SQUARE_SIZE;
    headTexture.width = SQUARE_SIZE;
    bodyTexture.height = SQUARE_SIZE;
    bodyTexture.width = SQUARE_SIZE;
}

int main(void) {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT + 100, TITLE);
    InitAudioDevice();
    SetTargetFPS(TARGET_FPS);
    SetRandomSeed(time(NULL));

    LoadTextures();

    eatingSound = LoadSound("resources/eat.wav");

    Snake snake = { 0 };
    snake.length = 1;
    snake.body[0].x = SQUARE_SIZE;
    snake.body[0].y = SQUARE_SIZE;
    snake.speed.x = 0;
    snake.speed.y = 0;

    float moveTimer = 0.0f;
    float moveInterval = 0.1f;

    Apple apple;
    randomizeApple(&apple);

    int score = 0;

    while (!WindowShouldClose()) {

        // --- INPUT (Fuori dal timer per essere reattivi) ---
        if ((IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) && snake.speed.x == 0) {
            snake.speed = (Vector2){ SQUARE_SIZE, 0 };
        }
        if ((IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) && snake.speed.x == 0) {
            snake.speed = (Vector2){ -SQUARE_SIZE, 0 };
        }
        if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && snake.speed.y == 0) {
            snake.speed = (Vector2){ 0, -SQUARE_SIZE };
        }
        if ((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) && snake.speed.y == 0) {
            snake.speed = (Vector2){ 0, SQUARE_SIZE };
        }

        // --- UPDATE ---
        moveTimer += GetFrameTime();
        if (moveTimer >= moveInterval) {
            moveTimer = 0.0f;

            if (snake.speed.x != 0 || snake.speed.y != 0) {
                moveSnake(&snake);
            }

            if (snake.body[0].x == apple.posX && snake.body[0].y == apple.posY) {
                snake.body[snake.length] = snake.body[snake.length - 1];
                snake.length++;
                score++;
                PlaySound(eatingSound);
                randomizeApple(&apple);
            }
            if (snake.body[0].x < 0 || snake.body[0].x > SCREEN_WIDTH || snake.body[0].y < 0 || snake.body[0].y > SCREEN_HEIGHT) {
                break;
            }
        }

        // --- DRAW ---
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawField();
            // drawGrid();

            DrawTexture(appleTexture, apple.posX, apple.posY, WHITE);
            // DrawRectangle(apple.posX, apple.posY, SQUARE_SIZE, SQUARE_SIZE, apple.color);
            DrawSnake(&snake);
            DrawScore(score);

        EndDrawing();
    }

    UnloadTexture(appleTexture);
    UnloadTexture(bodyTexture);
    UnloadTexture(headTexture);

    CloseAudioDevice();
    CloseWindow();
    return 0;
}