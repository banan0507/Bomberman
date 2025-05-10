#include "bomb.h"
#include "map.h"
#include "player.h"

Bomb bomb = { -1, -1, 0, 0 }; // Inițializare: bomba este inactivă

void place_bomb() {
    if (bomb.active) return; // Nu putem plasa o altă bombă dacă una este deja activă

    // Plasăm bomba la poziția player-ului (rotunjită la coordonate de tile)
    bomb.x = (int)((player.x + 25) / TILE_SIZE_SHOW);
    bomb.y = (int)((player.y + 25)/ TILE_SIZE_SHOW);
    bomb.timer = 2000; // Timer-ul pentru explozie (3 secunde)
    bomb.active = 1;   // Activăm bomba
}

void update_bomb(float delta_time) {
    if (!bomb.active) return; // Dacă bomba nu este activă, nu facem nimic

    bomb.timer -= delta_time * 1000; 
    if (bomb.timer <= 0) {
        explode_bomb(); // Explodăm bomba
        bomb.active = 0; // Dezactivăm bomba
    }
}

void explode_bomb() {
    // Distrugem tile-urile de tip `2` din jurul bombei
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int tile_x = bomb.x + j;
            int tile_y = bomb.y + i;

            // Verificăm dacă tile-ul este în limitele hărții
            if (tile_x >= 0 && tile_x < MAP_WIDTH && tile_y >= 0 && tile_y < MAP_HEIGHT) {
                if (tile_map[tile_y][tile_x] == 2) {
                    tile_map[tile_y][tile_x] = 1; // Tile-ul devine traversabil
                }
            }
        }
    }
}

void draw_bomb(SDL_Renderer* renderer, SDL_Texture* spritesheet) {
    if (!bomb.active) return; // Dacă bomba nu este activă, nu o desenăm

    // Coordonatele imaginii bombei în spritesheet
    SDL_FRect srcRect = { 0, 97, 16, 16 }; // Exemplu: imaginea bombei începe la (32, 0) și are dimensiunea 16x16

    // Poziția bombei pe ecran
    SDL_FRect dstRect = { bomb.x * TILE_SIZE_SHOW, bomb.y * TILE_SIZE_SHOW, TILE_SIZE_SHOW, TILE_SIZE_SHOW };

    // Desenăm bomba
    SDL_RenderTexture(renderer, spritesheet, &srcRect, &dstRect);
}