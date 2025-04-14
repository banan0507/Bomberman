#include "player.h"
#include "map.h"

Player player = { 65, 65, 57, 57 };

int key_states[4] = {0, 0, 0, 0}; // 0: W, 1: A, 2: S, 3: D

void handle_player_input(SDL_Event* event) 
{
    const float speed = 10.0f; // Viteza de mișcare (în pixeli)

    if (event->type == SDL_EVENT_KEY_DOWN) 
    {
        switch (event->key.key) 
        {
            case SDLK_W: key_states[0] = 1; break; 
            case SDLK_A: key_states[1] = 1; break; 
            case SDLK_S: key_states[2] = 1; break; 
            case SDLK_D: key_states[3] = 1; break; 
        }
    }
    else if (event->type == SDL_EVENT_KEY_UP) 
    {
        switch (event->key.key) 
        {
            case SDLK_W: key_states[0] = 0; break; 
            case SDLK_A: key_states[1] = 0; break; 
            case SDLK_S: key_states[2] = 0; break; 
            case SDLK_D: key_states[3] = 0; break; 
        }
    }
}

void update_player_position() 
{
    const float speed = 10.0f; // Viteza de mișcare (în pixeli)

    if (key_states[0] && can_move_to(player.x, player.y - speed)) { // W
        player.y -= speed;
    }
    if (key_states[1] && can_move_to(player.x - speed, player.y)) { // A
        player.x -= speed;
    }
    if (key_states[2] && can_move_to(player.x, player.y + speed)) { // S
        player.y += speed;
    }
    if (key_states[3] && can_move_to(player.x + speed, player.y)) { // D
        player.x += speed;
    }
}

int can_move_to(float x, float y) 
{
    int tile_x1 = (int)(x / TILE_SIZE_SHOW); // Colțul stânga-sus
    int tile_y1 = (int)(y / TILE_SIZE_SHOW); // Colțul stânga-sus
    int tile_x2 = (int)((x + player.width - 1) / TILE_SIZE_SHOW); // Colțul dreapta-sus
    int tile_y2 = (int)((y + player.height - 1) / TILE_SIZE_SHOW); // Colțul stânga-jos

    // Verificăm dacă toate colțurile sunt în limitele hărții
    if (tile_x1 < 0 || tile_x2 >= MAP_WIDTH || tile_y1 < 0 || tile_y2 >= MAP_HEIGHT) {
        return 0; // Nu se poate muta în afara hărții
    }

    // Verificăm dacă toate colțurile sunt traversabile
    if (tile_map[tile_y1][tile_x1] == 0 || tile_map[tile_y1][tile_x2] == 0 || tile_map[tile_y2][tile_x1] == 0 || tile_map[tile_y2][tile_x2] == 0  
    || tile_map[tile_y1][tile_x1] == 2 || tile_map[tile_y1][tile_x2] == 2 || tile_map[tile_y2][tile_x1] == 2 || tile_map[tile_y2][tile_x2] == 2)
        return 0; // Coliziune cu un tile solid

    return 1; // Mișcarea este permisă
}

void draw_player(SDL_Renderer* renderer, SDL_Texture* spritesheet) 
{
    const SDL_FRect srcRect = { 16, 0, 15, 24 };
    const SDL_FRect dstRect = { (int)player.x, (int)player.y, (int)player.width, (int)player.height };
    SDL_RenderTexture(renderer, spritesheet, &srcRect, &dstRect);
}