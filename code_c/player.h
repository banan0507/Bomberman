#pragma once

#include <SDL3/SDL.h>

#define MAX_AI 6

typedef struct 
{
    float x, y; 
    float width, height; 
    int alive; // 1: viu, 0: mort
} Player;

typedef struct {
    float x, y;         
    float width, height; 
    float speed;
    float direction_timer; 
    int direction;    
    int alive; // 1: viu, 0: mort 
    int last_direction;     
} AI;

extern Player player; 
extern AI enemies[MAX_AI];
extern int game_running; // 1: jocul este activ, 0: jocul s-a terminat

int player_on_bomb_tile(); 
void reinitializare_player();
int is_tile_type(float x, float y, int type);
void draw_player(SDL_Renderer* renderer, SDL_Texture* spritesheet);
void draw_ai(SDL_Renderer* renderer, SDL_Texture* spritesheet);
int can_move_to(float x, float y);
void update_player_position(float delta_time);
void update_ai_position(float delta_time);
void spawn_ai_random();