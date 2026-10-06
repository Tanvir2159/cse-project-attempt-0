#ifndef HUB_H
#define HUB_H

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

/* ---------- macros ---------- */
#define SW              1000
#define SH              650
#define MAX_NAME        32
#define SCORE_FILE      "data/scores.txt"
#define ARRAY_LEN(a)    ((int)(sizeof(a)/sizeof((a)[0])))
#define CLAMP(x,a,b)    ((x)<(a)?(a):((x)>(b)?(b):(x)))

#ifdef _WIN32
  #define CLEAR_CMD "cls"
#else
  #define CLEAR_CMD "clear"
#endif

/* ---------- structs / union (course topics) ---------- */
typedef struct {
    char name[MAX_NAME];
    int  wins[8];   /* per-game win counters */
    int  best_golf;
    int  best_candy;
    int  best_snake;
} Player;

/* union: same memory as int or as 4 chars (demo for viva) */
typedef union {
    int   as_int;
    char  as_bytes[4];
} IntBytes;

typedef enum {
    G_CONNECT4 = 0,
    G_BLACKJACK,
    G_SNAKES,
    G_LUDO,
    G_SNAKE,
    G_WORD,
    G_CANDY,
    G_GOLF,
    G_COUNT
} GameId;

void enable_console(void);
void shell_banner(const Player *p);
void shell_help(void);
void score_load(Player *p, const char *name);
void score_save(const Player *p);

void game_connect4(Player *p);
void game_blackjack(Player *p);
void game_snakes(Player *p);
void game_ludo(Player *p);
void game_snake(Player *p);
void game_word(Player *p);
void game_candy(Player *p);
void game_golf(Player *p);

#endif
