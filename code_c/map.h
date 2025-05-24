#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <time.h>
#include <stdlib.h>

#define NR_MAPS 1
#define MAP_WIDTH 27
#define MAP_HEIGHT 15
#define TILE_SIZE 16
#define WIDTH 1920
#define HEIGHT 1080
#define TILE_SIZE_W WIDTH/MAP_WIDTH
#define TILE_SIZE_H HEIGHT/MAP_HEIGHT

extern int tile_map[MAP_HEIGHT][MAP_WIDTH];

SDL_Surface* load_map_image(const char* file_path);

extern SDL_Renderer *renderer;

void reset_map();
void stergere_tiles();
void draw_map(SDL_Texture* spritesheet);
void load_tiles(SDL_Renderer *renderer);
void map_randomtiles();