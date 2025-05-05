#include "minimap.h"

int main(int argc, char* argv[]) {
    // Initialize SDL and SDL_image
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("SDL_image could not initialize! IMG_Error: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }
    
    // Create window
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE);
    if(screen == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    
    SDL_WM_SetCaption("Direct Minimap with Player Icon", NULL);
    
    // Create world and minimap surfaces
    world = SDL_CreateRGBSurface(SDL_SWSURFACE, WORLD_WIDTH, WORLD_HEIGHT, 32, 
                               0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    minimap = SDL_CreateRGBSurface(SDL_SWSURFACE, MINIMAP_WIDTH, MINIMAP_HEIGHT, 32, 
                                0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    
    // Load images and initialize player
    if(!load_images()) {
        free_resources();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    initialize_player();
    
    // Main game loop
    int quit = 0;
    SDL_Event e;
    
    while(!quit) {
        // Handle events
        while(SDL_PollEvent(&e) != 0) {
            if(e.type == SDL_QUIT) {
                quit = 1;
            }
        }
        
        // Handle input
        handle_input();
        
        // Render
        render_world();
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        
        // Draw visible world portion
        SDL_Rect src = {viewport_x, viewport_y, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_Rect dst = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_BlitSurface(world, &src, screen, &dst);
        
        // Draw minimap
        generate_minimap();
        SDL_Rect minimap_pos = {SCREEN_WIDTH - MINIMAP_WIDTH - 10, 10, MINIMAP_WIDTH, MINIMAP_HEIGHT};
        SDL_BlitSurface(minimap, NULL, screen, &minimap_pos);
        
        // Update screen
        SDL_Flip(screen);
        SDL_Delay(16);
    }
    
    // Clean up
    free_resources();
    IMG_Quit();
    SDL_Quit();
    return 0;
}
