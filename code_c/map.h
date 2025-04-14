#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <time.h>
#include <stdlib.h>

#define NR_MAPS 1
#define MAP_WIDTH 15
#define MAP_HEIGHT 15
#define TILE_SIZE 16
#define TILE_SIZE_SHOW 65

extern int tile_map[MAP_HEIGHT][MAP_WIDTH];

SDL_Surface* load_map_image(const char* file_path);

extern SDL_Renderer *renderer;

void draw_map(SDL_Texture* spritesheet);
void load_tiles(SDL_Renderer *renderer);
void map_randomtiles();