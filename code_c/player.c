#include "player.h"
#include "map.h"
#include "bomb.h"

Player player;
AI enemies[MAX_AI];

int key_states[4] = {0, 0, 0, 0}; // 0: W, 1: A, 2: S, 3: D
int game_running = 1; // Jocul începe activ
int can_pass_bomb = 0;
int player_stanga = 0;
int player_dreapta = 0;
int player_sus = 0;
int player_jos = 0;
int ai_stanga = 0;
int ai_dreapta = 0;
int ai_sus = 0;
int ai_jos = 0;

void reinitializare_player() 
{
    player.x = TILE_SIZE_W;
    player.y = TILE_SIZE_H;
    player.width = 0.8 * TILE_SIZE_W;  
    player.height = 0.8 * TILE_SIZE_H;
    player.alive = 1; // Player-ul este viu
}

int player_on_bomb_tile()
{
    float size_w = TILE_SIZE_W;
    float size_h = TILE_SIZE_H;
    if (!bomb.active) return 0;
    
    int corners[4][2] = {
        { (int)(player.x / size_w), (int)(player.y / size_h) },
        { (int)((player.x + player.width - 1) / size_w), (int)(player.y / size_h) },
        { (int)(player.x / size_w), (int)((player.y + player.height - 1) / size_h) },
        { (int)((player.x + player.width - 1) / size_w), (int)((player.y + player.height - 1) / size_h) }
    };

    for (int i = 0; i < 4; i++) {
        if (corners[i][0] == bomb.x && corners[i][1] == bomb.y)
            return 1;
    }
    return 0;
}

void spawn_ai_random() 
{
    int tile_w = TILE_SIZE_W;
    int tile_h = TILE_SIZE_H;

    srand(time(NULL)); // Inițializăm generatorul de numere aleatorii

    int player_tile_x = (int)(player.x / tile_w);
    int player_tile_y = (int)(player.y / tile_h);

    for (int i = 0; i < MAX_AI; i++) {
        int valid_position = 0;

        while (!valid_position) {
            // Generăm coordonate aleatorii pentru AI
            int random_tile_x = rand() % MAP_WIDTH;
            int random_tile_y = rand() % MAP_HEIGHT;

            // Verificăm dacă tile-ul este traversabil (1)
            if (tile_map[random_tile_y][random_tile_x] != 1) {
                continue;
            }

            // NU permite spawn pe player sau în vecinătatea jos/dreapta
        int is_near_player = 0;
        if (
            (random_tile_x == player_tile_x     && random_tile_y == player_tile_y)     || // pe player
            (random_tile_x == player_tile_x + 1 && random_tile_y == player_tile_y)     || // dreapta
            (random_tile_x == player_tile_x     && random_tile_y == player_tile_y + 1) || // jos
            (random_tile_x == player_tile_x + 1 && random_tile_y == player_tile_y + 1)    // dreapta-jos
        ) {
            continue;
        }

            // Verificăm dacă nu se suprapune cu alte AI-uri
            valid_position = 1;
            for (int j = 0; j < i; j++) {
                int ai_tile_x = (int)(enemies[j].x / tile_w);
                int ai_tile_y = (int)(enemies[j].y / tile_h);
                if (random_tile_x == ai_tile_x && random_tile_y == ai_tile_y) {
                    valid_position = 0;
                    break;
                }
            }

            // Dacă poziția este validă, setăm coordonatele AI-ului
            if (valid_position) {
                enemies[i].x = random_tile_x * tile_w;
                enemies[i].y = random_tile_y * tile_h;
                enemies[i].width = 0.8 * tile_w;  
                enemies[i].height = 0.8 * tile_h;
                enemies[i].speed = 100;       
                enemies[i].direction = rand() % 4;
                enemies[i].direction_timer = 2;
                enemies[i].alive = 1;
            }
        }
    }
}

void update_ai_position(float delta_time) 
{
    for (int i = 0; i < MAX_AI; i++) {
        AI* enemy = &enemies[i];

        if (!enemy->alive) continue;

        enemy->direction_timer -= delta_time;

        if (enemy->direction_timer <= 0) {
            enemy->direction = rand() % 4; 
            enemy->direction_timer = 2; 
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
        if (can_move_to(new_x, enemy->y) && !is_bomb_at(new_x, enemy->y, enemy->width, enemy->height)) {
            enemy->x = new_x;
        } else {
            // Inversăm direcția pe axa X
            if (enemy->direction == 2) enemy->direction = 3;
            else if (enemy->direction == 3) enemy->direction = 2;
        }

        if (can_move_to(enemy->x, new_y) && !is_bomb_at(enemy->x, new_y, enemy->width, enemy->height)) {
            enemy->y = new_y;
        } else {
            // Inversăm direcția pe axa Y
            if (enemy->direction == 0) enemy->direction = 1;
            else if (enemy->direction == 1) enemy->direction = 0;
        }
        if (!(player.x + player.width <= enemy->x || player.x >= enemy->x + enemy->width ||
              player.y + player.height <= enemy->y || player.y >= enemy->y + enemy->height)) {
            game_running = 0; // Player-ul a fost omorât
            player.alive = 0; // Player-ul este mort
        }
        enemy->last_direction = enemy->direction;
    }
}

int is_tile_type(float x, float y, int type) 
{
    int tile_w = TILE_SIZE_W;
    int tile_h = TILE_SIZE_H;

    int tile_x = (int)(x / tile_w);
    int tile_y = (int)(y / tile_h);

    // Verificăm dacă tile-ul este în limitele hărții
    if (tile_x < 0 || tile_x >= MAP_WIDTH || tile_y < 0 || tile_y >= MAP_HEIGHT) {
        return 0; // Tile-ul este în afara hărții
    }

    // Returnăm 1 dacă tile-ul este de tipul specificat, altfel 0
    return tile_map[tile_y][tile_x] == type;
}

int can_move_to(float x, float y) 
{
    int tile_w = TILE_SIZE_W;
    int tile_h = TILE_SIZE_H;
    
    int tile_x1 = (int)(x / tile_w); // Colțul stânga-sus
    int tile_y1 = (int)(y / tile_h); // Colțul stânga-sus
    int tile_x2 = (int)((x + player.width - 1) / tile_w); // Colțul dreapta-sus
    int tile_y2 = (int)((y + player.height - 1) / tile_h); // Colțul stânga-jos

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
        new_y -= 150 * delta_time;
        player_sus = 1;
    }
    // Mișcare la stânga
    if (keyboard_state[SDL_SCANCODE_A]) {
        new_x -= 150 * delta_time;
        player_stanga = 1;
    }
    // Mișcare în jos
    if (keyboard_state[SDL_SCANCODE_S]) {
        new_y += 150 * delta_time;
        player_jos = 1;
    }
    // Mișcare la dreapta
    if (keyboard_state[SDL_SCANCODE_D]) {
        new_x += 150 * delta_time;
        player_dreapta = 1;
    }

    // Mutare pe axa X
    if (can_move_to(new_x, player.y) && 
        (!is_bomb_at(new_x, player.y, player.width, player.height) || (can_pass_bomb && player_on_bomb_tile()))) {
        player.x = new_x;
    }

    // Mutare pe axa Y
    if (can_move_to(player.x, new_y) && 
        (!is_bomb_at(player.x, new_y, player.width, player.height) || (can_pass_bomb && player_on_bomb_tile()))) {
        player.y = new_y;
    }

    // *** Setează can_pass_bomb = 0 doar dacă playerul nu mai e pe tile-ul bombei după ambele mutări ***
    if (!player_on_bomb_tile()) {
        can_pass_bomb = 0;
    }
}

void draw_player(SDL_Renderer* renderer, SDL_Texture* spritesheet) 
{
    const SDL_FRect srcRect = { 16, 0, 15, 24 };
    const SDL_FRect dstRect = { (int)player.x, (int)player.y, (int)player.width, (int)player.height };
    const SDL_FRect srcRect_stanga = { 64, 0, 14.9, 23.6 };
    const SDL_FRect srcRect_dreapta = { 112, 0, 16, 23.6 };
    const SDL_FRect srcRect_sus = { 160, 0, 15, 24 };
    const SDL_FRect srcRect_jos = { 16, 0, 15, 24 };
    if (player_sus) {
        SDL_RenderTexture(renderer, spritesheet, &srcRect_sus, &dstRect);
        player_sus = 0;
        return;
    }
    if (player_jos) {
        SDL_RenderTexture(renderer, spritesheet, &srcRect_jos, &dstRect);
        player_jos = 0;
        return;
    }
    if (player_dreapta) {
        SDL_RenderTexture(renderer, spritesheet, &srcRect_dreapta, &dstRect);
        player_dreapta = 0;
        return;
    }
    if (player_stanga) {
        SDL_RenderTexture(renderer, spritesheet, &srcRect_stanga, &dstRect);
        player_stanga = 0;
        return;
    }
    SDL_RenderTexture(renderer, spritesheet, &srcRect, &dstRect);
}

void draw_ai(SDL_Renderer* renderer, SDL_Texture* spritesheet) 
{
    for (int i = 0; i < MAX_AI; i++) {
        AI* enemy = &enemies[i];
        if (!enemy->alive) continue;

        SDL_FRect srcRect = { 16, 24, 15, 24 };
        SDL_FRect dstRect = { enemy->x, enemy->y, enemy->width, enemy->height };
        SDL_FRect srcRect_stanga = { 64, 24.5, 14.9, 23 };
        SDL_FRect srcRect_dreapta = { 112, 24.5, 16, 23 };
        SDL_FRect srcRect_sus = { 160, 24.5, 15, 23.5 };
        SDL_FRect srcRect_jos = { 16, 24.5, 15, 23.5 };

        switch (enemy->last_direction) {
            case 0: // sus
                SDL_RenderTexture(renderer, spritesheet, &srcRect_sus, &dstRect);
                break;
            case 1: // jos
                SDL_RenderTexture(renderer, spritesheet, &srcRect_jos, &dstRect);
                break;
            case 2: // stanga
                SDL_RenderTexture(renderer, spritesheet, &srcRect_stanga, &dstRect);
                break;
            case 3: // dreapta
                SDL_RenderTexture(renderer, spritesheet, &srcRect_dreapta, &dstRect);
                break;
            default:
                SDL_RenderTexture(renderer, spritesheet, &srcRect, &dstRect);
        }
    }
}