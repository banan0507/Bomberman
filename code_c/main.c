#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#include "map.h"
#include "player.h"
#include "bomb.h"

SDL_Window* window;
SDL_Renderer* renderer;

Uint64 last_tick = 0;
Uint64 current_tick = 0;
float delta_time;

void update() 
{
    last_tick = current_tick;
    current_tick = SDL_GetTicks();
    delta_time = (current_tick - last_tick) / 1000.0f;
}

int main(int argc, char* argv[])
 {
    int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    if(result < 0)
    {
        SDL_Log("SDL_Init error:  %s", SDL_GetError());
        return -1;
    }

    window = SDL_CreateWindow("Bomberman", 975, 975, SDL_WINDOW_RESIZABLE);
    
    if(window == NULL)
    {
        SDL_Log("SDL_CreateWindow: %s", SDL_GetError());
        return -2;
    }

    SDL_SetWindowIcon(window, IMG_Load("Assets\\Yellowicon-Game-Stars-Bomberman.256.png"));

    renderer = SDL_CreateRenderer(window, NULL);
    SDL_SetRenderVSync(renderer, 1);
    if(renderer == NULL)
    {
        SDL_Log("SDL_CreateRenderer: %s", SDL_GetError());
        return -3;
    }

    SDL_Log("Game start!");

    SDL_Event event;
    int quit = 0;
    SDL_Texture* spritesheet = IMG_LoadTexture(renderer, "Assets\\spritesheet.png");
    if (spritesheet == NULL) {
        SDL_Log("Eroare la încărcarea spritesheet-ului: %s", SDL_GetError());
        return -1;
    }

    map_randomtiles();

    uint32_t last_time = SDL_GetTicks();    // Timpul inițial

    while (!quit)
    {
        Uint32 frameStart = SDL_GetTicks();
        float delta_time = (frameStart - last_time) / 1000.0f; // Calculăm delta_time
        last_time = frameStart;

        // Procesăm evenimentele
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT) 
            {
                quit = 1;
            }
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_B) 
            {
                place_bomb();
            }
            //update_player_position(delta_time);
            //handle_player_input(&event); // Gestionăm intrările de la tastatură
        }

        // Actualizăm logica jocului
        update_player_position(delta_time);
        update_ai_position(delta_time);
        update_bomb(delta_time);

        SDL_RenderClear(renderer);

        draw_map(spritesheet);
        draw_bomb(renderer, spritesheet); 
        draw_player(renderer, spritesheet); 
        draw_ai(renderer, spritesheet);

        SDL_RenderPresent(renderer); // Prezentăm frame-ul
    }

    SDL_Log("Game over!");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(spritesheet);

    SDL_Quit();

    return 0;
}