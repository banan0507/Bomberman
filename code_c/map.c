#include "map.h"
#include "player.h"
 
int tile_map[MAP_HEIGHT][MAP_WIDTH] = 
{
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

// SDL_Surface* load_map_image(const char* file_path) 
// {
//     SDL_Surface* image = IMG_Load(file_path);
//     if (image == NULL) 
//     {
//         SDL_Log("Eroare la IMG_Load in map: %s", SDL_GetError());
//         return NULL;
//     }
//     return image;
// }

// void load_tiles(SDL_Renderer *renderer)
// {
//     SDL_Surface* spritesheet = load_map_image("Assets\\spritesheet.png");
//     if (spritesheet == NULL) {
//         SDL_Log("Eroare la încărcarea spritesheet-ului!");
//         return;
//     }
    
//     SDL_Rect tile_1 = { 0, 114, 16, 16 };  
//     SDL_Rect tile_2 = { 119, 114, 16, 16 }; 

//     SDL_Surface* tile_surface_1 = SDL_CreateRGBSurfaceWithFormat(0, tile_1.w, tile_1.h, 32, spritesheet->format);
//     SDL_Surface* tile_surface_2 = SDL_CreateRGBSurfaceWithFormat(0, tile_2.w, tile_2.h, 32, spritesheet->format);

//     SDL_BlitSurface(spritesheet, &tile_1, tile_surface_1, NULL);
//     SDL_BlitSurface(spritesheet, &tile_2, tile_surface_2, NULL);

//     SDL_Texture* tile_texture_1 = SDL_CreateTextureFromSurface(renderer, tile_surface_1);
//     SDL_Texture* tile_texture_2 = SDL_CreateTextureFromSurface(renderer, tile_surface_2);

//     SDL_FreeSurface(tile_surface_1);
//     SDL_FreeSurface(tile_surface_2);
//     SDL_FreeSurface(spritesheet);

//     SDL_DestroyTexture(tile_texture_1);
//     SDL_DestroyTexture(tile_texture_2);   
// }

void map_randomtiles()
{
    srand(time(NULL)); 
    for(int i = 0; i < MAP_HEIGHT; i++)
        for(int j = 0; j < MAP_WIDTH; j++)
        {    
            // Verificăm dacă tile-ul coincide cu poziția oricărui AI
            int is_ai_position = 0;
            for (int k = 0; k < MAX_AI; k++) {
                if (i == (int)(enemies[k].y / TILE_SIZE_SHOW) && j == (int)(enemies[k].x / TILE_SIZE_SHOW)) {
                    is_ai_position = 1;
                    break;
                }
            }

            if (is_ai_position) {
                tile_map[i][j] = 1; // Tile traversabil
                continue;
            }
            if(tile_map[i][j] == 1 && (i != 1 || j != 1))
                tile_map[i][j] = (rand() % 4 == 0) ? 2 : 1;
        }
}

void draw_map(SDL_Texture* spritesheet) 
{
    SDL_FRect tileCoords[] = {
        { 0, 114, 16, 16 },       
        { 68, 114, 16, 16 },
        { 17, 114, 16, 16}
    };

    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) 
        {
            int tileType = tile_map[i][j];

            if (tileType < 0 || tileType >= sizeof(tileCoords) / sizeof(tileCoords[0])) 
            {
                SDL_Log("TileType %d nu are coordonate definite!", tileType);
                continue;
            }

            const SDL_FRect srcRect = tileCoords[tileType];
            const SDL_FRect dstRect = { j * TILE_SIZE_SHOW, i * TILE_SIZE_SHOW, TILE_SIZE_SHOW, TILE_SIZE_SHOW }; 

            SDL_RenderTexture(renderer, spritesheet, &srcRect, &dstRect);
        }
    } 
}