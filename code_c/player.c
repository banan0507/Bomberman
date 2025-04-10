#include "player.h"
#include "map.h"

typedef struct 
{
    float x, y; 
    float width, height; 
} Player;

Player player = { 32, 32, 65, 65 }; 

void handle_player_input(SDL_Event* event) 
{
    const float speed = 40.0f; // Viteza de mișcare (în pixeli)

    if (event->type == SDL_EVENT_KEY_DOWN) 
    {
        switch (event->key.key) 
        {
            case SDLK_W: 
                if (can_move_to(player.x, player.y - speed)) {
                    player.y -= speed;
                }
                break;
            case SDLK_S: 
                if (can_move_to(player.x, player.y + speed)) {
                    player.y += speed;
                }
                break;
            case SDLK_A: 
                if (can_move_to(player.x - speed, player.y)) {
                    player.x -= speed;
                }
                break;
            case SDLK_D:
                if (can_move_to(player.x + speed, player.y)) {
                    player.x += speed;
                }
                break;
        }
    }
}

int can_move_to(float x, float y) 
{
    // Calculăm tile-ul pe care încearcă să se mute player-ul
    int tile_x = (int)(x / TILE_SIZE_SHOW);
    int tile_y = (int)(y / TILE_SIZE_SHOW);

    // Verificăm dacă tile-ul este în limitele hărții
    if (tile_x < 0 || tile_x >= MAP_WIDTH || tile_y < 0 || tile_y >= MAP_HEIGHT) {
        return 0; // Nu se poate muta în afara hărții
    }

    // Verificăm dacă tile-ul este traversabil (de exemplu, `0` este traversabil, `1` este perete)
    return tile_map[tile_y][tile_x] == 0;
}

void draw_player(SDL_Renderer* renderer, SDL_Texture* spritesheet) 
{
    const SDL_FRect srcRect = { 16, 0, 15, 24 };
    const SDL_FRect dstRect = { (int)player.x, (int)player.y, (int)player.width, (int)player.height };
    SDL_RenderTexture(renderer, spritesheet, &srcRect, &dstRect);
}