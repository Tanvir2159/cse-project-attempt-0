#include "hub.h"
#define R 6
#define C 7
static int drop(int b[R][C], int col, int p) {
    if (col<0||col>=C||b[0][col]) return 0;
    for (int r=R-1;r>=0;r--) if(!b[r][col]){b[r][col]=p;return 1;}
    return 0;
}
static int four(int b[R][C], int p) {
    int r,c,k;
    for(r=0;r<R;r++)for(c=0;c<=C-4;c++){int o=1;for(k=0;k<4;k++)if(b[r][c+k]!=p)o=0;if(o)return 1;}
    for(c=0;c<C;c++)for(r=0;r<=R-4;r++){int o=1;for(k=0;k<4;k++)if(b[r+k][c]!=p)o=0;if(o)return 1;}
    for(r=0;r<=R-4;r++)for(c=0;c<=C-4;c++){int o=1;for(k=0;k<4;k++)if(b[r+k][c+k]!=p)o=0;if(o)return 1;}
    for(r=0;r<=R-4;r++)for(c=3;c<C;c++){int o=1;for(k=0;k<4;k++)if(b[r+k][c-k]!=p)o=0;if(o)return 1;}
    return 0;
}
static void cpu(int b[R][C]) {
    for(int c=0;c<C;c++){if(!drop(b,c,2))continue;if(four(b,2))return;for(int r=0;r<R;r++)if(b[r][c]==2){b[r][c]=0;break;}}
    for(int c=0;c<C;c++){if(!drop(b,c,1))continue;int n=four(b,1);for(int r=0;r<R;r++)if(b[r][c]==1){b[r][c]=0;break;}if(n){drop(b,c,2);return;}}
    if(drop(b,3,2))return;
    for(int i=0;i<30;i++)if(drop(b,GetRandomValue(0,C-1),2))return;
}
void game_connect4(Player *p) {
    (void)p;
    int b[R][C]={0}, over=0, msg=0;
    float cell=70, ox=(SW-C*cell)/2, oy=90;
    InitWindow(SW, SH, "Connect Four");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) break;
        if (IsKeyPressed(KEY_R)) { memset(b,0,sizeof b); over=msg=0; }
        if (!over && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int col = (int)((GetMouseX()-ox)/cell);
            if (drop(b,col,1)) {
                if (four(b,1)) { over=1; msg=1; }
                else { cpu(b); if (four(b,2)) { over=1; msg=2; } }
            }
        }
        BeginDrawing();
        ClearBackground((Color){20,50,100,255});
        DrawText("CONNECT FOUR  ESC=hub  R=restart  click column", 20, 20, 22, RAYWHITE);
        DrawRectangle((int)(ox-12),(int)(oy-12),(int)(C*cell+24),(int)(R*cell+24),(Color){25,70,160,255});
        for (int r=0;r<R;r++) for (int c=0;c<C;c++) {
            Color col = (Color){15,35,70,255};
            if (b[r][c]==1) col=RED; if (b[r][c]==2) col=GOLD;
            DrawCircle((int)(ox+c*cell+cell/2),(int)(oy+r*cell+cell/2),(int)(cell/2-6), col);
        }
        if (msg==1) DrawText("YOU WIN", SW/2-70, SH-40, 30, GREEN);
        if (msg==2) DrawText("CPU WINS", SW/2-80, SH-40, 30, ORANGE);
        EndDrawing();
    }
    CloseWindow();
}
