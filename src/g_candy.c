#include "hub.h"
#define N 8
#define T 5

static Color COLS[] = { RED, GREEN, BLUE, GOLD, PURPLE };

void game_candy(Player *p) {
    int g[N][N];
    for (int r=0;r<N;r++) for (int c=0;c<N;c++) g[r][c]=GetRandomValue(0,T-1);
    int score=0, moves=15, sel=-1;
    float cell=55, ox=(SW-N*cell)/2, oy=100;

    InitWindow(SW, SH, "Candy");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) break;
        if (moves>0 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int c=(int)((GetMouseX()-ox)/cell), r=(int)((GetMouseY()-oy)/cell);
            if (r>=0&&r<N&&c>=0&&c<N) {
                int id=r*N+c;
                if (sel<0) sel=id;
                else {
                    int r1=sel/N,c1=sel%N;
                    if (abs(r1-r)+abs(c1-c)==1) {
                        int t=g[r1][c1]; g[r1][c1]=g[r][c]; g[r][c]=t;
                        /* simple clear horizontal matches */
                        int cleared=0;
                        for (int rr=0;rr<N;rr++) for (int cc=0;cc<N-2;cc++)
                            if (g[rr][cc]==g[rr][cc+1]&&g[rr][cc]==g[rr][cc+2]&&g[rr][cc]>=0) {
                                g[rr][cc]=g[rr][cc+1]=g[rr][cc+2]=-1; cleared+=3;
                            }
                        for (int cc=0;cc<N;cc++) for (int rr=0;rr<N-2;rr++)
                            if (g[rr][cc]==g[rr+1][cc]&&g[rr][cc]==g[rr+2][cc]&&g[rr][cc]>=0) {
                                g[rr][cc]=g[rr+1][cc]=g[rr+2][cc]=-1; cleared+=3;
                            }
                        if (!cleared) { t=g[r1][c1]; g[r1][c1]=g[r][c]; g[r][c]=t; }
                        else {
                            score+=cleared*10; moves--;
                            for (int cc=0;cc<N;cc++) {
                                int w=N-1;
                                for (int rr=N-1;rr>=0;rr--)
                                    if (g[rr][cc]>=0) { g[w][cc]=g[rr][cc]; if(w!=rr)g[rr][cc]=-1; w--; }
                                for (int rr=w;rr>=0;rr--) g[rr][cc]=GetRandomValue(0,T-1);
                            }
                            if (score>p->best_candy) p->best_candy=score;
                        }
                    }
                    sel=-1;
                }
            }
        }
        BeginDrawing();
        ClearBackground((Color){40,20,50,255});
        DrawText(TextFormat("CANDY  score %d  moves %d  click swap adjacent  ESC", score, moves), 20, 20, 22, RAYWHITE);
        for (int r=0;r<N;r++) for (int c=0;c<N;c++) {
            int x=(int)(ox+c*cell), y=(int)(oy+r*cell);
            Color col = g[r][c]<0 ? DARKGRAY : COLS[g[r][c]%T];
            DrawRectangle(x+2,y+2,(int)cell-4,(int)cell-4, col);
            if (sel==r*N+c) DrawRectangleLines(x,y,(int)cell,(int)cell, WHITE);
        }
        if (moves<=0) DrawText("OUT OF MOVES", SW/2-100, SH-40, 28, GOLD);
        EndDrawing();
    }
    CloseWindow();
}
