#ifndef MINIMAP_H
#define MINIMAP_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

// Screen dimensions
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define WORLD_WIDTH 2000
#define WORLD_HEIGHT 2000
#define MINIMAP_WIDTH 200
#define MINIMAP_HEIGHT 150

// Player settings
#define PLAYER_SPEED 5

// Player structure
typedef struct {
    int x, y;       // Position in world coordinates
    SDL_Surface *image; // Player image (for main view)
    SDL_Surface *minimap_icon; // Player icon for minimap
} Player;

// Global variables
extern SDL_Surface *screen;
extern SDL_Surface *world;
extern SDL_Surface *minimap;
extern SDL_Surface *minimap_bg;
extern Player player;
extern int viewport_x;
extern int viewport_y;
extern SDL_Surface *viewport_icon; 
// Function declarations
int load_images();
void initialize_player();
void handle_input();
void render_world();
void generate_minimap();
void free_resources();

#endif // MINIMAP_H
