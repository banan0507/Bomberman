#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#include "map.h"

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
    while(!quit)
    {
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_EVENT_QUIT:
                    SDL_Log("SDL3 event quit");
                    quit = 1;
                    break;
            }
        }
        draw_map();
        SDL_Delay(1);
    }

    SDL_Log("Game over!");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}