#include "hub.h"
#define GW 28
#define GH 18
#define CS 28

void game_snake(Player *p) {
    Vector2 snake[GW*GH];
    int len=3; snake[0]=(Vector2){8,8}; snake[1]=(Vector2){7,8}; snake[2]=(Vector2){6,8};
    Vector2 dir={1,0}, food={14,8};
    int score=0, dead=0; float acc=0;
    int ox=(SW-GW*CS)/2, oy=(SH-GH*CS)/2+20;
    InitWindow(SW, SH, "Snake");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) break;
        if (IsKeyPressed(KEY_R)) { len=3; score=0; dead=0; dir=(Vector2){1,0}; snake[0]=(Vector2){8,8}; }
        if (!dead) {
            if (IsKeyPressed(KEY_RIGHT)&&dir.x==0) dir=(Vector2){1,0};
            if (IsKeyPressed(KEY_LEFT)&&dir.x==0) dir=(Vector2){-1,0};
            if (IsKeyPressed(KEY_DOWN)&&dir.y==0) dir=(Vector2){0,1};
            if (IsKeyPressed(KEY_UP)&&dir.y==0) dir=(Vector2){0,-1};
            acc += GetFrameTime();
            if (acc>0.11f) {
                acc=0;
                for (int i=len;i>0;i--) snake[i]=snake[i-1];
                snake[0].x+=dir.x; snake[0].y+=dir.y;
                if (snake[0].x<0||snake[0].x>=GW||snake[0].y<0||snake[0].y>=GH) dead=1;
                for (int i=1;i<len;i++) if (snake[i].x==snake[0].x&&snake[i].y==snake[0].y) dead=1;
                if (snake[0].x==food.x&&snake[0].y==food.y) {
                    len++; score+=10; food.x=(float)GetRandomValue(0,GW-1); food.y=(float)GetRandomValue(0,GH-1);
                    if (score > p->best_snake) p->best_snake = score;
                }
            }
        }
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText(TextFormat("SNAKE  score %d best %d  arrows  R  ESC", score, p->best_snake), 20, 16, 20, RAYWHITE);
        DrawRectangle(ox-2,oy-2,GW*CS+4,GH*CS+4, DARKGRAY);
        for (int i=0;i<len;i++)
            DrawRectangle(ox+(int)snake[i].x*CS, oy+(int)snake[i].y*CS, CS-2, CS-2, i==0?LIME:GREEN);
        DrawRectangle(ox+(int)food.x*CS, oy+(int)food.y*CS, CS-2, CS-2, RED);
        if (dead) DrawText("DEAD - R restart", SW/2-100, SH/2, 28, RED);
        EndDrawing();
    }
    CloseWindow();
}
