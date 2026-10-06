#include "hub.h"

typedef struct { int rank, suit; } Card;

static void shuffle(Card *d, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = GetRandomValue(0, i);
        Card t = d[i]; d[i] = d[j]; d[j] = t;
    }
}
static int val(Card c) {
    if (c.rank >= 10) return 10;
    if (c.rank == 1) return 11;
    return c.rank;
}
static int total(Card *h, int n) {
    int s = 0, a = 0;
    for (int i = 0; i < n; i++) {
        s += val(h[i]);
        if (h[i].rank == 1) a++;
    }
    while (s > 21 && a--) s -= 10;
    return s;
}

/* Draw a full playing-card face (side-by-side table layout) */
static void draw_card(Card c, int x, int y, int hide) {
    const int W = 78, H = 110;
    /* shadow */
    DrawRectangle(x + 4, y + 4, W, H, (Color){0, 0, 0, 80});
    if (hide) {
        /* blue card back with pattern */
        DrawRectangle(x, y, W, H, (Color){25, 55, 140, 255});
        DrawRectangleLinesEx((Rectangle){(float)x,(float)y,(float)W,(float)H}, 3, (Color){200, 180, 80, 255});
        DrawRectangle(x + 10, y + 12, W - 20, H - 24, (Color){35, 70, 160, 255});
        DrawText("HUB", x + 22, y + 42, 22, (Color){200, 180, 80, 255});
        return;
    }
    int red = (c.suit == 1 || c.suit == 2);
    Color ink = red ? (Color){200, 30, 30, 255} : (Color){20, 20, 20, 255};
    const char *rn[] = {"?","A","2","3","4","5","6","7","8","9","10","J","Q","K"};
    /* suit as letter + shape hint (raylib default font may lack unicode suits) */
    const char *su[] = {"S", "H", "D", "C"}; /* Spades Hearts Diamonds Clubs */

    DrawRectangle(x, y, W, H, RAYWHITE);
    DrawRectangleLinesEx((Rectangle){(float)x,(float)y,(float)W,(float)H}, 2, (Color){40,40,40,255});
    /* top-left rank */
    DrawText(rn[c.rank], x + 8, y + 8, 22, ink);
    DrawText(su[c.suit], x + 8, y + 32, 20, ink);
    /* center suit large */
    DrawText(su[c.suit], x + (c.rank == 10 ? 22 : 28), y + 48, 36, ink);
    /* bottom-right rank */
    DrawText(rn[c.rank], x + W - 28, y + H - 40, 20, ink);
    DrawText(su[c.suit], x + W - 24, y + H - 22, 18, ink);
}

void game_blackjack(Player *p) {
    (void)p;
    Card deck[52];
    int k = 0;
    for (int s = 0; s < 4; s++)
        for (int r = 1; r <= 13; r++) {
            deck[k].suit = s;
            deck[k].rank = r;
            k++;
        }
    shuffle(deck, 52);
    int top = 0;
    Card ph[12], dh[12];
    int pn = 0, dn = 0;
    ph[pn++] = deck[top++]; dh[dn++] = deck[top++];
    ph[pn++] = deck[top++]; dh[dn++] = deck[top++];
    int phase = 0;
    const char *result = "";

    InitWindow(SW, SH, "Blackjack — Table");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) break;
        if (phase == 0) {
            if (IsKeyPressed(KEY_H) && total(ph, pn) < 21) {
                ph[pn++] = deck[top++];
                if (total(ph, pn) > 21) { phase = 1; result = "BUST — dealer wins"; }
            }
            if (IsKeyPressed(KEY_S)) {
                while (total(dh, dn) < 17) dh[dn++] = deck[top++];
                int pt = total(ph, pn), dt = total(dh, dn);
                if (dt > 21 || pt > dt) result = "YOU WIN";
                else if (pt < dt) result = "DEALER WINS";
                else result = "PUSH (draw)";
                phase = 1;
            }
        }
        if (phase && IsKeyPressed(KEY_N)) {
            /* new hand */
            shuffle(deck, 52); top = 0; pn = dn = 0; phase = 0; result = "";
            ph[pn++] = deck[top++]; dh[dn++] = deck[top++];
            ph[pn++] = deck[top++]; dh[dn++] = deck[top++];
        }

        BeginDrawing();
        /* felt table */
        ClearBackground((Color){ 12, 90, 45, 255 });
        DrawRectangle(0, 0, SW, 56, (Color){ 8, 40, 20, 255 });
        DrawText("BLACKJACK", 20, 14, 28, GOLD);
        DrawText("H hit   S stand   N new hand   ESC hub", 220, 20, 20, RAYWHITE);

        /* dealer row */
        DrawText("DEALER", 40, 80, 24, (Color){ 220, 220, 180, 255 });
        for (int i = 0; i < dn; i++)
            draw_card(dh[i], 40 + i * 90, 115, phase == 0 && i == 0);
        if (phase)
            DrawText(TextFormat("total %d", total(dh, dn)), 40 + dn * 90 + 10, 155, 22, YELLOW);

        /* you row */
        DrawText("YOU", 40, 320, 24, (Color){ 220, 220, 180, 255 });
        for (int i = 0; i < pn; i++)
            draw_card(ph[i], 40 + i * 90, 355, 0);
        DrawText(TextFormat("total %d", total(ph, pn)), 40 + pn * 90 + 10, 395, 22, YELLOW);

        if (phase) {
            DrawRectangle(0, SH - 70, SW, 70, (Color){ 0, 0, 0, 160 });
            DrawText(result, 40, SH - 48, 32, GOLD);
            DrawText("press N for new hand", 400, SH - 42, 22, LIGHTGRAY);
        } else {
            DrawText("Your move: H or S", 40, SH - 40, 22, RAYWHITE);
        }
        EndDrawing();
    }
    CloseWindow();
}
