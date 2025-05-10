#pragma once

#include <SDL3/SDL.h>

#define MAX_AI 5

typedef struct 
{
    float x, y; 
    float width, height; 
} Player;

typedef struct {
    float x, y;         
    float width, height; 
    float speed;
    float direction_timer; 
    int direction;          
} AI;

extern Player player; 
extern AI enemies[MAX_AI];

//void handle_player_input(SDL_Event* event);
int is_tile_type(float x, float y, int type);
void draw_player(SDL_Renderer* renderer, SDL_Texture* spritesheet);
void draw_ai(SDL_Renderer* renderer, SDL_Texture* spritesheet);
int can_move_to(float x, float y);
void update_player_position(float delta_time);
void update_ai_position(float delta_time);