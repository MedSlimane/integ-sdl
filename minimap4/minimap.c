#include "minimap.h"

// Global variables
SDL_Surface *screen = NULL;
SDL_Surface *world = NULL;
SDL_Surface *minimap = NULL;
SDL_Surface *minimap_bg = NULL;
Player player;
int viewport_x = 0;
int viewport_y = 0;

int load_images() {
    // Load minimap background (should be same size as minimap)
    minimap_bg = IMG_Load("minimap.jpg");
    if(!minimap_bg) {
        printf("Failed to load minimap.png: %s\n", IMG_GetError());
        return 0;
    }
    
    // Load player image (for main view)
    player.image = IMG_Load("player.png");
    if(!player.image) {
        printf("Failed to load player.png: %s\n", IMG_GetError());
        return 0;
    }
    
    // Create minimap player icon (scaled down version)
    player.minimap_icon = IMG_Load("player.png");
    if(!player.minimap_icon) {
        printf("Failed to load player.png for minimap: %s\n", IMG_GetError());
        return 0;
    }
    
    SDL_SoftStretch(player.minimap_icon, NULL, player.image, NULL);
    SDL_FreeSurface(player.minimap_icon);
    player.minimap_icon = player.image;
    return 1;
}

void initialize_player() {
    player.x = WORLD_WIDTH / 2;
    player.y = WORLD_HEIGHT / 2;
}

void free_resources() {
    if(minimap_bg) SDL_FreeSurface(minimap_bg);
    if(player.image) SDL_FreeSurface(player.image);
    if(player.minimap_icon) SDL_FreeSurface(player.minimap_icon);
    if(world) SDL_FreeSurface(world);
    if(minimap) SDL_FreeSurface(minimap);
}


void handle_input() {
    const Uint8 *keystate = SDL_GetKeyState(NULL);
    
    // Move player
    if(keystate[SDLK_LEFT]) player.x -= PLAYER_SPEED;
    if(keystate[SDLK_RIGHT]) player.x += PLAYER_SPEED;
    if(keystate[SDLK_UP]) player.y -= PLAYER_SPEED;
    if(keystate[SDLK_DOWN]) player.y += PLAYER_SPEED;
    
    // Keep player in bounds
    if(player.x < 0) player.x = 0;
    if(player.y < 0) player.y = 0;
    if(player.x > WORLD_WIDTH - player.image->w) player.x = WORLD_WIDTH - player.image->w;
    if(player.y > WORLD_HEIGHT - player.image->h) player.y = WORLD_HEIGHT - player.image->h;
    
    // Update viewport to follow player
    viewport_x = player.x - SCREEN_WIDTH/2 + player.image->w/2;
    viewport_y = player.y - SCREEN_HEIGHT/2 + player.image->h/2;
    
    // Clamp viewport to world boundaries
    if(viewport_x < 0) viewport_x = 0;
    if(viewport_y < 0) viewport_y = 0;
    if(viewport_x > WORLD_WIDTH - SCREEN_WIDTH) viewport_x = WORLD_WIDTH - SCREEN_WIDTH;
    if(viewport_y > WORLD_HEIGHT - SCREEN_HEIGHT) viewport_y = WORLD_HEIGHT - SCREEN_HEIGHT;
}

void render_world() {
    // Clear world with dark gray
    SDL_FillRect(world, NULL, SDL_MapRGB(world->format, 30, 30, 30));
    
    // Draw player on world
    SDL_Rect player_rect = {player.x, player.y, 0, 0};
    SDL_BlitSurface(player.image, NULL, world, &player_rect);
}

void generate_minimap() {
    // Draw complete minimap background (not fog of war)
    SDL_BlitSurface(minimap_bg, NULL, minimap, NULL);
    
    // Calculate player position on minimap
    float scale_x = (float)MINIMAP_WIDTH / WORLD_WIDTH;
    float scale_y = (float)MINIMAP_HEIGHT / WORLD_HEIGHT;
    
    SDL_Rect player_pos = {
        (int)(player.x * scale_x) - player.minimap_icon->w/2,
        (int)(player.y * scale_y) - player.minimap_icon->h/2,
        player.minimap_icon->w,
        player.minimap_icon->h
    };
    
    // Draw player icon on minimap
    SDL_BlitSurface(player.minimap_icon, NULL, minimap, &player_pos);
    

}
