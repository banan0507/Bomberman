#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#include "map.h"
#include "player.h"

SDL_Window* window;
SDL_Renderer* renderer;

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
    while(!quit)
    {
        while(SDL_PollEvent(&event))
        {
            handle_player_input(&event);
            if (event.type == SDL_EVENT_QUIT) 
            {
                quit = 1;
            }
        }
        draw_map(spritesheet);
        draw_player(renderer, spritesheet);
        
        SDL_RenderPresent(renderer);
    }

    SDL_Log("Game over!");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(spritesheet);

    SDL_Quit();

    return 0;
}