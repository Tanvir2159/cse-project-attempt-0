#include "hub.h"

/* Mini Golf — 8 levels (walls, sand, ponds like golf-replica) */

#define BALL_R  12.0f
#define HOLE_R  16.0f
#define FRICTION 120.0f
#define MAX_LEVELS 8

typedef struct {
    Vector2 start, hole;
    Rectangle walls[8];
    int wallN;
    Rectangle sand[4];
    int sandN;
    Vector2 ponds[4];
    float pondR[4];
    int pondN;
    const char *name;
    Color grass;
} GolfLevel;

static void load_level(GolfLevel *L, int id) {
    memset(L, 0, sizeof *L);
    L->grass = (Color){ 42, 140, 62, 255 };
    switch (id) {
    case 0:
        L->name = "1  Open Fairway";
        L->start = (Vector2){ 100, SH/2.f };
        L->hole  = (Vector2){ 900, SH/2.f };
        break;
    case 1:
        L->name = "2  Pillars";
        L->start = (Vector2){ 90, SH/2.f };
        L->hole  = (Vector2){ 910, SH/2.f };
        L->walls[0] = (Rectangle){ 250, 40, 32, 360 };
        L->walls[1] = (Rectangle){ 480, 220, 32, 360 };
        L->walls[2] = (Rectangle){ 710, 40, 32, 360 };
        L->wallN = 3;
        break;
    case 2:
        L->name = "3  Sand Channels";
        L->start = (Vector2){ 80, SH/2.f };
        L->hole  = (Vector2){ 920, SH/2.f };
        L->sand[0] = (Rectangle){ 220, 50, 90, SH-60 };
        L->sand[1] = (Rectangle){ 700, 50, 90, SH-60 };
        L->sandN = 2;
        break;
    case 3:
        L->name = "4  Twin Ponds";
        L->start = (Vector2){ 80, 520 };
        L->hole  = (Vector2){ 920, 100 };
        L->ponds[0] = (Vector2){ 350, 300 }; L->pondR[0] = 70;
        L->ponds[1] = (Vector2){ 650, 350 }; L->pondR[1] = 55;
        L->pondN = 2;
        L->walls[0] = (Rectangle){ 500, 0, 28, 250 };
        L->wallN = 1;
        break;
    case 4:
        L->name = "5  Dogleg";
        L->start = (Vector2){ 80, 520 };
        L->hole  = (Vector2){ 900, 100 };
        L->walls[0] = (Rectangle){ 300, 200, 28, 450 };
        L->walls[1] = (Rectangle){ 550, 0, 28, 400 };
        L->walls[2] = (Rectangle){ 750, 250, 28, 400 };
        L->wallN = 3;
        break;
    case 5:
        L->name = "6  Corridor";
        L->start = (Vector2){ 70, SH/2.f };
        L->hole  = (Vector2){ 930, SH/2.f };
        L->walls[0] = (Rectangle){ 250, 0, 24, 240 };
        L->walls[1] = (Rectangle){ 250, 400, 24, 250 };
        L->walls[2] = (Rectangle){ 500, 150, 24, 340 };
        L->walls[3] = (Rectangle){ 720, 0, 24, 260 };
        L->walls[4] = (Rectangle){ 720, 380, 24, 270 };
        L->wallN = 5;
        break;
    case 6:
        L->name = "7  Maze";
        L->start = (Vector2){ 70, 100 };
        L->hole  = (Vector2){ 920, 550 };
        L->walls[0] = (Rectangle){ 200, 0, 24, 400 };
        L->walls[1] = (Rectangle){ 400, 200, 24, 450 };
        L->walls[2] = (Rectangle){ 600, 0, 24, 380 };
        L->walls[3] = (Rectangle){ 800, 180, 24, 470 };
        L->sand[0] = (Rectangle){ 450, 300, 120, 80 };
        L->sandN = 1;
        L->ponds[0] = (Vector2){ 300, 500 }; L->pondR[0] = 45;
        L->pondN = 1;
        L->wallN = 4;
        break;
    default:
        L->name = "8  Final Gauntlet";
        L->start = (Vector2){ 60, SH/2.f };
        L->hole  = (Vector2){ 940, SH/2.f };
        L->walls[0] = (Rectangle){ 180, 50, 26, 280 };
        L->walls[1] = (Rectangle){ 180, 380, 26, 250 };
        L->walls[2] = (Rectangle){ 360, 0, 26, 320 };
        L->walls[3] = (Rectangle){ 540, 280, 26, 360 };
        L->walls[4] = (Rectangle){ 720, 0, 26, 300 };
        L->walls[5] = (Rectangle){ 720, 400, 26, 250 };
        L->wallN = 6;
        L->ponds[0] = (Vector2){ 450, 200 }; L->pondR[0] = 40;
        L->ponds[1] = (Vector2){ 620, 480 }; L->pondR[1] = 50;
        L->pondN = 2;
        L->sand[0] = (Rectangle){ 300, 300, 80, SH-350 };
        L->sandN = 1;
        break;
    }
}

static void bounce(Vector2 *b, Vector2 *v, Rectangle w) {
    if (!CheckCollisionCircleRec(*b, BALL_R, w)) return;
    if (b->x < w.x) { b->x = w.x - BALL_R; v->x = -fabsf(v->x)*0.72f; }
    else if (b->x > w.x+w.width) { b->x = w.x+w.width+BALL_R; v->x = fabsf(v->x)*0.72f; }
    if (b->y < w.y) { b->y = w.y - BALL_R; v->y = -fabsf(v->y)*0.72f; }
    else if (b->y > w.y+w.height) { b->y = w.y+w.height+BALL_R; v->y = fabsf(v->y)*0.72f; }
}

void game_golf(Player *p) {
    int lid = 0, strokes = 0, sunk = 0, aiming = 0, done = 0;
    GolfLevel L;
    load_level(&L, lid);
    Vector2 ball = L.start, vel = {0,0};

    InitWindow(SW, SH, "Mini Golf - 8 Levels");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) break;
        if (IsKeyPressed(KEY_R) && !done) {
            ball = L.start; vel = (Vector2){0,0}; sunk = 0;
        }

        float dt = GetFrameTime();
        if (!done && !sunk) {
            float fric = FRICTION;
            for (int i = 0; i < L.sandN; i++)
                if (CheckCollisionCircleRec(ball, BALL_R, L.sand[i]))
                    fric = FRICTION * 2.8f;

            ball.x += vel.x * dt; ball.y += vel.y * dt;
            float sp = sqrtf(vel.x*vel.x + vel.y*vel.y);
            if (sp > 0) {
                float ns = sp - fric * dt;
                if (ns < 0) ns = 0;
                vel.x = vel.x/sp*ns; vel.y = vel.y/sp*ns;
                if (ns < 12) vel = (Vector2){0,0};
            }

            if (ball.x < BALL_R) { ball.x = BALL_R; vel.x *= -0.7f; }
            if (ball.x > SW-BALL_R) { ball.x = SW-BALL_R; vel.x *= -0.7f; }
            if (ball.y < 56) { ball.y = 56; vel.y *= -0.7f; }
            if (ball.y > SH-BALL_R) { ball.y = SH-BALL_R; vel.y *= -0.7f; }

            for (int i = 0; i < L.wallN; i++) bounce(&ball, &vel, L.walls[i]);

            for (int i = 0; i < L.pondN; i++) {
                if (CheckCollisionCircles(ball, BALL_R*0.7f, L.ponds[i], L.pondR[i])) {
                    ball = L.start; vel = (Vector2){0,0}; strokes++;
                }
            }

            if (CheckCollisionCircles(ball, 7, L.hole, HOLE_R) && sp < 80) {
                sunk = 1;
                if (lid >= MAX_LEVELS - 1) {
                    done = 1;
                    if (p->best_golf == 0 || strokes < p->best_golf)
                        p->best_golf = strokes;
                }
            }

            if (sp < 12 && !sunk) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) aiming = 1;
                if (aiming && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                    Vector2 m = GetMousePosition();
                    Vector2 d = { ball.x - m.x, ball.y - m.y };
                    float pwr = sqrtf(d.x*d.x+d.y*d.y) * 3.4f;
                    if (pwr > 900) pwr = 900;
                    if (pwr > 30) {
                        float len = sqrtf(d.x*d.x+d.y*d.y);
                        vel.x = d.x/len*pwr; vel.y = d.y/len*pwr;
                        strokes++;
                    }
                    aiming = 0;
                }
            }
        }

        if (sunk && !done && (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))) {
            lid++;
            load_level(&L, lid);
            ball = L.start; vel = (Vector2){0,0}; sunk = 0;
        }

        BeginDrawing();
        DrawRectangle(0, 0, SW, SH, L.grass);
        DrawRectangle(0, 0, SW, 52, (Color){ 25, 55, 90, 255 });
        for (int i = 0; i < 8; i++)
            DrawCircle(80 + i*120, 120 + (i%3)*80, 40, (Color){ 35, 120, 50, 40 });

        for (int i = 0; i < L.sandN; i++)
            DrawRectangleRec(L.sand[i], (Color){ 210, 190, 120, 255 });
        for (int i = 0; i < L.pondN; i++) {
            DrawCircleV(L.ponds[i], L.pondR[i], (Color){ 40, 100, 180, 255 });
            DrawCircleV(L.ponds[i], L.pondR[i]-8, (Color){ 50, 130, 200, 200 });
        }
        for (int i = 0; i < L.wallN; i++) {
            DrawRectangleRec(L.walls[i], (Color){ 95, 65, 35, 255 });
            DrawRectangleLinesEx(L.walls[i], 2, (Color){ 60, 40, 20, 255 });
        }

        DrawCircleV(L.hole, HOLE_R + 4, (Color){ 20, 20, 20, 255 });
        DrawCircleV(L.hole, HOLE_R, BLACK);
        DrawCircleV(ball, BALL_R, RAYWHITE);
        DrawCircleLines((int)ball.x, (int)ball.y, (int)BALL_R, GRAY);
        if (aiming) {
            DrawLineEx(ball, GetMousePosition(), 2, YELLOW);
            Vector2 m = GetMousePosition();
            Vector2 tip = { ball.x + (ball.x - m.x), ball.y + (ball.y - m.y) };
            DrawLineEx(ball, tip, 3, ORANGE);
        }

        DrawText(TextFormat("%s", L.name), 16, 10, 24, RAYWHITE);
        DrawText(TextFormat("Strokes %d   Best %d   Level %d/8", strokes, p->best_golf, lid+1),
                 16, 36, 18, (Color){ 200, 220, 255, 255 });
        DrawText("drag aim  |  R reset  |  ESC hub", SW - 300, 16, 18, LIGHTGRAY);

        for (int i = 0; i < MAX_LEVELS; i++) {
            Color c = i < lid ? LIME : (i == lid ? GOLD : GRAY);
            DrawCircle(SW/2 - 70 + i*20, SH - 18, 6, c);
        }

        if (sunk && !done) {
            DrawRectangle(0, SH/2-45, SW, 90, (Color){0,0,0,170});
            DrawText("HOLE!", SW/2-50, SH/2-35, 36, GOLD);
            DrawText("ENTER / SPACE  ->  next level", SW/2-150, SH/2+10, 22, RAYWHITE);
        }
        if (done) {
            DrawRectangle(0,0,SW,SH,(Color){0,0,0,180});
            DrawText("COURSE COMPLETE", SW/2-170, SH/2-50, 36, GOLD);
            DrawText(TextFormat("Total strokes: %d", strokes), SW/2-110, SH/2, 26, RAYWHITE);
            DrawText("ESC return to hub", SW/2-100, SH/2+40, 20, LIGHTGRAY);
        }
        EndDrawing();
    }
    CloseWindow();
}
