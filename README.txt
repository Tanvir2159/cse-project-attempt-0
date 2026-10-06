FULL GAME HUB — Linux-style CLI + raylib game windows

FLOW
  1. Terminal opens: login, ASCII banner, player@hub:~$
  2. Type a room name -> graphics WINDOW opens for that game
  3. ESC closes window -> back to shell prompt

GAMES
  connect4  blackjack  snakes  ludo  snake  word  candy  golf

TOPICS
  macros, pointers (Player*), structs, union (type "union"),
  files (data/scores.txt), multi-file, CLI shell

SETUP (CRITICAL — use WIN64)
  Your explorer already had:
    include\raylib-6.0_win64_mingw-w64\include\raylib.h
    include\raylib-6.0_win64_mingw-w64\lib\libraylib.a

  Copy that whole raylib-6.0_win64_mingw-w64 folder into:
    rayhub\include\raylib-6.0_win64_mingw-w64\

BUILD
  Double-click build.bat

  OR PowerShell (must say win64 NOT win32):

cd C:\path\to\rayhub

gcc -Wall -Wextra -std=c99 -Iinclude -I"include/raylib-6.0_win64_mingw-w64/include" -L"include/raylib-6.0_win64_mingw-w64/lib" -o rayhub.exe src/main.c src/util.c src/g_connect4.c src/g_blackjack.c src/g_snakes.c src/g_ludo.c src/g_snake.c src/g_word.c src/g_candy.c src/g_golf.c -lraylib -lopengl32 -lgdi32 -lwinmm

.\rayhub.exe

WHY YOUR LAST COMPILE FAILED
  Folder on disk:  raylib-6.0_win64_mingw-w64   (CORRECT)
  Command you ran: raylib-6.0_win32_mingw-w64   (WRONG — still said win32)
  Fix: type win64 in both -I and -L paths.
