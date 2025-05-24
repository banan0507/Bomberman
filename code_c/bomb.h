#pragma once

#include <SDL3/SDL.h>

typedef struct {
    int x, y;           // Poziția bombei pe hartă (în coordonate de tile)
    int timer;          // Timer-ul până la explozie (în milisecunde)
    int active;         // 1 dacă bomba este activă, 0 dacă nu
} Bomb;

extern Bomb bomb;
extern int can_pass_bomb;

void place_bomb();
void update_bomb(float delta_time);
void explode_bomb();
void draw_bomb(SDL_Renderer* renderer, SDL_Texture* spritesheet);
int is_in_bomb_range(int bomb_x, int bomb_y, int player_x, int player_y, int range);
int is_bomb_at(float x, float y, float width, float height);