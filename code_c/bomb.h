#pragma once

#include <SDL3/SDL.h>

typedef struct {
    int x, y;           // Poziția bombei pe hartă (în coordonate de tile)
    int timer;          // Timer-ul până la explozie (în milisecunde)
    int active;         // 1 dacă bomba este activă, 0 dacă nu
} Bomb;

extern Bomb bomb;

void place_bomb();
void update_bomb();
void explode_bomb();
void draw_bomb(SDL_Renderer* renderer, SDL_Texture* spritesheet);
