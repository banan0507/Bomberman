#include "player.h"
#include "map.h"

Player player = { 65, 65, 50, 50 };

AI enemies[MAX_AI] = {
    { 200, 200, 50, 50, 100.0f, 2.0f, 0 },
    { 300, 300, 50, 50, 100.0f, 2.0f, 1 },
    { 400, 400, 50, 50, 100.0f, 2.0f, 2 },
    { 500, 500, 50, 50, 100.0f, 2.0f, 3 },
    { 600, 600, 50, 50, 100.0f, 2.0f, 0 }
};

int key_states[4] = {0, 0, 0, 0}; // 0: W, 1: A, 2: S, 3: D

// void handle_player_input(SDL_Event* event) 
// {

//     if (event->type == SDL_EVENT_KEY_DOWN) 
//     {
//         switch (event->key.key) 
//         {
//             case SDLK_W: key_states[0] = 1; break; 
//             case SDLK_A: key_states[1] = 1; break; 
//             case SDLK_S: key_states[2] = 1; break; 
//             case SDLK_D: key_states[3] = 1; break; 
//         }
//     }
//     else if (event->type == SDL_EVENT_KEY_UP) 
//     {
//         switch (event->key.key) 
//         {
//             case SDLK_W: key_states[0] = 0; break; 
//             case SDLK_A: key_states[1] = 0; break; 
//             case SDLK_S: key_states[2] = 0; break; 
//             case SDLK_D: key_states[3] = 0; break; 
//         }
//     }
// }

// static void update(float delta_time) {


//     if (!check_collision(new_x, position_player.y)) {
//         position_player.x = new_x;
//     }

//     if (!check_collision(position_player.x, new_y)) {
//         position_player.y = new_y;
//     }
// }

void update_ai_position(float delta_time) 
{
    for (int i = 0; i < MAX_AI; i++) {
        AI* enemy = &enemies[i];

        // Scădem timer-ul pentru schimbarea direcției
        enemy->direction_timer -= delta_time;

        // Dacă timer-ul a expirat, alegem o nouă direcție aleatorie
        if (enemy->direction_timer <= 0) {
            enemy->direction = rand() % 4; // Alegem o direcție aleatorie
            enemy->direction_timer = 2.0f; // Resetăm timer-ul
        }

        float new_x = enemy->x;
        float new_y = enemy->y;

        // Mișcăm AI-ul în funcție de direcția curentă
        switch (enemy->direction) {
            case 0: // Sus
                new_y -= enemy->speed * delta_time;
                break;
            case 1: // Jos
                new_y += enemy->speed * delta_time;
                break;
            case 2: // Stânga
                new_x -= enemy->speed * delta_time;
                break;
            case 3: // Dreapta
                new_x += enemy->speed * delta_time;
                break;
        }

        // Verificăm coliziunile și actualizăm poziția
        if (can_move_to(new_x, enemy->y)) {
            enemy->x = new_x;
        } else {
            // Inversăm direcția pe axa X
            if (enemy->direction == 2) enemy->direction = 3;
            else if (enemy->direction == 3) enemy->direction = 2;
        }

        if (can_move_to(enemy->x, new_y)) {
            enemy->y = new_y;
        } else {
            // Inversăm direcția pe axa Y
            if (enemy->direction == 0) enemy->direction = 1;
            else if (enemy->direction == 1) enemy->direction = 0;
        }
    }
}

int is_tile_type(float x, float y, int type) 
{
    int tile_x = (int)(x / TILE_SIZE_SHOW);
    int tile_y = (int)(y / TILE_SIZE_SHOW);

    // Verificăm dacă tile-ul este în limitele hărții
    if (tile_x < 0 || tile_x >= MAP_WIDTH || tile_y < 0 || tile_y >= MAP_HEIGHT) {
        return 0; // Tile-ul este în afara hărții
    }

    // Returnăm 1 dacă tile-ul este de tipul specificat, altfel 0
    return tile_map[tile_y][tile_x] == type;
}

int can_move_to(float x, float y) 
{
    int tile_x1 = (int)(x / TILE_SIZE_SHOW); // Colțul stânga-sus
    int tile_y1 = (int)(y / TILE_SIZE_SHOW); // Colțul stânga-sus
    int tile_x2 = (int)((x + player.width - 1) / TILE_SIZE_SHOW); // Colțul dreapta-sus
    int tile_y2 = (int)((y + player.height - 1) / TILE_SIZE_SHOW); // Colțul stânga-jos

    // // Verificăm dacă toate colțurile sunt în limitele hărții
    // if (tile_x1 < 0 || tile_x2 >= MAP_WIDTH || tile_y1 < 0 || tile_y2 >= MAP_HEIGHT) {
    //     return 0; // Nu se poate muta în afara hărții
    // }

    // Verificăm dacă toate colțurile sunt traversabile
    if (tile_map[tile_y1][tile_x1] == 0 || tile_map[tile_y1][tile_x2] == 0 || tile_map[tile_y2][tile_x1] == 0 || tile_map[tile_y2][tile_x2] == 0  
    || tile_map[tile_y1][tile_x1] == 2 || tile_map[tile_y1][tile_x2] == 2 || tile_map[tile_y2][tile_x1] == 2 || tile_map[tile_y2][tile_x2] == 2)
        return 0; // Coliziune cu un tile solid

    return 1; // Mișcarea este permisă
}

void update_player_position(float delta_time) 
{
    const _Bool *keyboard_state = SDL_GetKeyboardState(NULL);

    float new_x = player.x;
    float new_y = player.y;

    // Mișcare în sus
    if (keyboard_state[SDL_SCANCODE_W]) {
        new_y -= 100 * delta_time;
    }
    // Mișcare la stânga
    if (keyboard_state[SDL_SCANCODE_A]) {
        new_x -= 100 * delta_time;
    }
    // Mișcare în jos
    if (keyboard_state[SDL_SCANCODE_S]) {
        new_y += 100 * delta_time;
    }
    // Mișcare la dreapta
    if (keyboard_state[SDL_SCANCODE_D]) {
        new_x += 100 * delta_time;
    }

    // Actualizăm poziția player-ului dacă nu există coliziuni
    if (can_move_to(new_x, player.y)) {
        player.x = new_x;
    }
    if (can_move_to(player.x, new_y)) {
        player.y = new_y;
    }
}

void draw_player(SDL_Renderer* renderer, SDL_Texture* spritesheet) 
{
    const SDL_FRect srcRect = { 16, 0, 15, 25 };
    const SDL_FRect dstRect = { (int)player.x, (int)player.y, (int)player.width, (int)player.height };
    SDL_RenderTexture(renderer, spritesheet, &srcRect, &dstRect);
}

void draw_ai(SDL_Renderer* renderer, SDL_Texture* spritesheet) 
{
    for (int i = 0; i < MAX_AI; i++) {
        AI* enemy = &enemies[i];
        SDL_FRect srcRect = { 16, 24, 15, 25 };
        SDL_FRect dstRect = { enemy->x, enemy->y, enemy->width, enemy->height };
        SDL_RenderTexture(renderer, spritesheet, &srcRect, &dstRect);
    }
}