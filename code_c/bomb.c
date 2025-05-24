#include "bomb.h"
#include "map.h"
#include "player.h"

Bomb bomb = { -1, -1, 0, 0 }; // Inițializare: bomba este inactivă

void place_bomb() {
    if (bomb.active) return; // Nu putem plasa o altă bombă dacă una este deja activă
    int size_w = TILE_SIZE_W;
    int size_h = TILE_SIZE_H;
    bomb.x = (int)((player.x + player.width / 2) / size_w);
    bomb.y = (int)((player.y + player.height / 2)/ size_h);
    bomb.timer = 2000; // Timer-ul pentru explozie (3 secunde)
    bomb.active = 1;   // Activăm bomba
    can_pass_bomb = 1;  // playerul poate trece prin bombă cât timp e pe ea
}

void update_bomb(float delta_time) {
    if (!bomb.active) return; // Dacă bomba nu este activă, nu facem nimic

    bomb.timer -= delta_time * 1000; 
    if (bomb.timer <= 0) {
        explode_bomb(); // Explodăm bomba
        bomb.active = 0; // Dezactivăm bomba
    }
}

int is_bomb_at(float x, float y, float width, float height) 
{
    if (!bomb.active) return 0;
    float size_w = TILE_SIZE_W;
    float size_h = TILE_SIZE_H;

    int corners[4][2] = {
        { (int)(x / size_w), (int)(y / size_h) },
        { (int)((x + width - 1) / size_w), (int)(y / size_h) },
        { (int)(x / size_w), (int)((y + height - 1) / size_h) },
        { (int)((x + width - 1) / size_w), (int)((y + height - 1) / size_h) }
    };

    for (int i = 0; i < 4; i++) {
        if (corners[i][0] == bomb.x && corners[i][1] == bomb.y)
            return 1;
    }
    return 0;
}

int is_in_bomb_range(int bomb_x, int bomb_y, int player_x, int player_y, int range) 
{
    // Verificăm dacă player-ul este în raza bombei
    if ((player_x == bomb_x && abs(player_y - bomb_y) <= range) || // Pe verticală
        (player_y == bomb_y && abs(player_x - bomb_x) <= range)) { // Pe orizontală
        return 1; // Player-ul este în raza bombei
    }
    return 0; // Player-ul nu este în raza bombei
}

void explode_bomb() {
    // Distrugem tile-urile de tip `2` din jurul bombei
    int range = 1;
    for (int i = -range; i <= range; i++) {
        for (int j = -range; j <= range; j++) {
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
    
    int size_w = TILE_SIZE_W;
    int size_h = TILE_SIZE_H;

    int player_x = (int)(player.x / size_w);
    int player_y = (int)(player.y / size_h);
    if (is_in_bomb_range(bomb.x, bomb.y, player_x, player_y, range)) {
        SDL_Log("Player-ul a fost omorât de bombă!");
        game_running = 0;
        player.alive = 0; 
    }

    for (int i = 0; i < MAX_AI; i++) {
        if (!enemies[i].alive) continue;
        int ai_x = (int)((enemies[i].x + enemies[i].width / 2) / size_w);
        int ai_y = (int)((enemies[i].y + enemies[i].height / 2) / size_h);
        if (is_in_bomb_range(bomb.x, bomb.y, ai_x, ai_y, range)) {
            enemies[i].alive = 0;
            SDL_Log("AI %d a fost omorat de bomba!", i);
        }
    }

    int all_dead = 1;
    for (int i = 0; i < MAX_AI; i++) {
        if (enemies[i].alive) {
            all_dead = 0;
            break;
        }
    }
    if (all_dead) {
        SDL_Log("Toți AI-ii au fost omorâți! Ai câștigat!");
        game_running = 0;
    }
}

void draw_bomb(SDL_Renderer* renderer, SDL_Texture* spritesheet) {
    if (!bomb.active) return; // Dacă bomba nu este activă, nu o desenăm

    // Coordonatele imaginii bombei în spritesheet
    SDL_FRect srcRect = { 0, 97, 16, 16 }; // Exemplu: imaginea bombei începe la (32, 0) și are dimensiunea 16x16

    // Poziția bombei pe ecran
    SDL_FRect dstRect = { bomb.x * TILE_SIZE_W, bomb.y * TILE_SIZE_H, TILE_SIZE_W, TILE_SIZE_H };

    // Desenăm bomba
    SDL_RenderTexture(renderer, spritesheet, &srcRect, &dstRect);
}