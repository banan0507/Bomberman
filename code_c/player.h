#pragma once

#include <SDL3/SDL.h>

void handle_player_input(SDL_Event* event);
void draw_player(SDL_Renderer* renderer, SDL_Texture* spritesheet);
int can_move_to(float x, float y);
void update_player_position();