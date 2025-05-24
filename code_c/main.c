#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#include "map.h"
#include "player.h"
#include "bomb.h"

#define INIT_GAME reset_map();spawn_ai_random();map_randomtiles();reinitializare_player();

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

void afisareStart()
{
    SDL_Texture * image = IMG_LoadTexture(renderer, "Assets\\StartScreen.png");
    if (!image)
    {
        SDL_Log("Failed to load image: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    const SDL_FRect srcRect = { 0, 0 , 1536, 1024};  
    const SDL_FRect dstRect = { 0, 0 , WIDTH, HEIGHT };

    SDL_RenderTexture(renderer, image, &srcRect, &dstRect);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(image); // Eliberăm textura după utilizare
}

void afisareWIN()
{
    SDL_Texture * image = IMG_LoadTexture(renderer, "Assets\\WinScreen.png");
    if (!image)
    {
        SDL_Log("Failed to load image: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    const SDL_FRect srcRect = { 0, 0 , 1536, 1024};  
    const SDL_FRect dstRect = { 0, 0 , WIDTH, HEIGHT };

    SDL_RenderTexture(renderer, image, &srcRect, &dstRect);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(image); // Eliberăm textura după utilizare
}

void afisareLOSE()
{
    SDL_Texture * image = IMG_LoadTexture(renderer, "Assets\\LoseScreen.png");
    if (!image)
    {
        SDL_Log("Failed to load image: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    const SDL_FRect srcRect = { 0, 0 , 1536, 1024};  
    const SDL_FRect dstRect = { 0, 0 , WIDTH, HEIGHT };

    SDL_RenderTexture(renderer, image, &srcRect, &dstRect);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(image); // Eliberăm textura după utilizare
}

int startGame(float x, float y)
{
    return (x >= 0.15*WIDTH && x <= 0.4*WIDTH && y >= 0.68*HEIGHT && y <= 0.871*HEIGHT);
}

int retry(float x, float y)
{
    if(y > 0.715 * HEIGHT && y < 0.877 * HEIGHT){
        if(x > 0.016*WIDTH && x < 0.321*WIDTH) 
        {
            return 1; // Returnează 1 pentru a indica că utilizatorul a apăsat pe butonul "Retry"
        }
    }
    if(y > 0.685 * HEIGHT && y < 0.894 * HEIGHT){
        if(x > 0.56*WIDTH && x < 0.917*WIDTH) 
        {
            return 2; // Returnează 2 pentru a indica că utilizatorul a apăsat pe butonul "Quit"
        }
    }
}

int main(int argc, char* argv[])
 {
    int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    if(result < 0)
    {
        SDL_Log("SDL_Init error:  %s", SDL_GetError());
        return -1;
    } 

    window = SDL_CreateWindow("Bomberman", WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);
    
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

    afisareStart();

    do{
        SDL_Event event;
        SDL_PollEvent(&event);
        if (event.type == SDL_EVENT_QUIT)
        {
            SDL_Log("Quit event received");
            break;
        }
        if(SDL_EVENT_MOUSE_BUTTON_DOWN == event.type)
        {
        //SDL_Log("Mouse button pressed at (%f, %f)", event.button.x, event.button.y);
            float x = event.button.x;
            float y = event.button.y;
            if (startGame(x, y) == 1)
            {
                break;
            }
        }
    }while(1);

    SDL_Log("Game start!");

    SDL_Event event;
    int quit = 0;
    SDL_Texture* spritesheet = IMG_LoadTexture(renderer, "Assets\\spritesheet.png");
    if (spritesheet == NULL) {
        SDL_Log("Eroare la încărcarea spritesheet-ului: %s", SDL_GetError());
        return -1;
    }

    INIT_GAME

    uint32_t last_time = SDL_GetTicks();    // Timpul inițial

    int running = 1;

    while (running && !quit)
    {
        Uint32 frameStart = SDL_GetTicks();
        float delta_time = (frameStart - last_time) / 1000.0f; // Calculăm delta_time
        last_time = frameStart;


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
        }

        update_player_position(delta_time);
        update_ai_position(delta_time);
        update_bomb(delta_time);

        if (!game_running) {
            if(player.alive == 1){
                afisareWIN();
                bool retrying = true;
                while (retrying) {
                    while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_EVENT_QUIT) {
                            running = false;
                        retrying = false;
                        break;
                        }
                        if(SDL_EVENT_MOUSE_BUTTON_DOWN == event.type) {
                            float x = event.button.x;
                            float y = event.button.y;
                            switch(retry(x, y)) {
                            case 1: // Dacă utilizatorul apasă pe retry
                                INIT_GAME
                                game_running = 1;
                                retrying = false; // Ieși din bucla de retry
                                break;
                            case 2: // Dacă utilizatorul apasă pe quit
                                running = false;
                                retrying = false;
                                break;
                            }
                        }
                    }
                }
            }
            else{
                afisareLOSE();
                bool retrying = true;
                while (retrying) {
                    while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_EVENT_QUIT) {
                            running = false;
                        retrying = false;
                        break;
                        }
                        if(SDL_EVENT_MOUSE_BUTTON_DOWN == event.type) {
                            float x = event.button.x;
                            float y = event.button.y;
                            switch(retry(x, y)) {
                            case 1: // Dacă utilizatorul apasă pe retry
                                INIT_GAME
                                game_running = 1;
                                retrying = false; // Ieși din bucla de retry
                                break;
                            case 2: // Dacă utilizatorul apasă pe quit
                                running = false;
                                retrying = false;
                                break;
                            }
                        }
                    }
                }
            }
        }

        SDL_RenderClear(renderer);

        draw_map(spritesheet);
        draw_bomb(renderer, spritesheet); 
        draw_player(renderer, spritesheet); 
        draw_ai(renderer, spritesheet);

        SDL_RenderPresent(renderer); 
    }

    SDL_Log("Game over!");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(spritesheet);

    SDL_Quit();

    return 0;
}