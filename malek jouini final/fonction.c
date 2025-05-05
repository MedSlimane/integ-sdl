#include "fonction.h"
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>

// Global variables definitions
SDL_Surface *screen = NULL;
SDL_Surface *button_diminuer, *button_diminuer_hovered;
SDL_Surface *button_augmenter, *button_augmenter_hovered;
SDL_Surface *button_plein_ecran, *button_plein_ecran_hovered;
SDL_Surface *button_normal, *button_normal_hovered;
SDL_Surface *button_retour, *button_retour_hovered;
SDL_Surface *background, *fullscreen_background;
Mix_Music *music = NULL;
Mix_Chunk *click_sound = NULL;
TTF_Font *font = NULL;
SDL_Color textColor = {255, 255, 255}; // Color of the text (white)
int volume = 50;  // Initial volume
int is_fullscreen = 0;

// Function to initialize SDL and resources
int init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return -1;
    }

    screen = SDL_SetVideoMode(1500, 800, 32, SDL_SWSURFACE);
    if (!screen) {
        printf("Video mode setting error: %s\n", SDL_GetError());
        return -1;
    }

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        printf("Mix_OpenAudio Error: %s\n", Mix_GetError());
        return -1;
    }

    music = Mix_LoadMUS("musique.mp3");
    if (!music) {
        printf("Music loading error: %s\n", Mix_GetError());
        return -1;
    }

    click_sound = Mix_LoadWAV("son_bref.wav");
    if (!click_sound) {
        printf("Sound loading error: %s\n", Mix_GetError());
        return -1;
    }

    button_diminuer = IMG_Load("diminuer.png");
    button_diminuer_hovered = IMG_Load("diminuer2.png");
    button_augmenter = IMG_Load("augmenter.png");
    button_augmenter_hovered = IMG_Load("augmenter2.png");
    button_plein_ecran = IMG_Load("plein_ecran.png");
    button_plein_ecran_hovered = IMG_Load("plein_ecran2.png");
    button_normal = IMG_Load("normal.png");
    button_normal_hovered = IMG_Load("normal2.png");
    button_retour = IMG_Load("retour.png");
    button_retour_hovered = IMG_Load("retour2.png");

    background = IMG_Load("background.jpeg");
    fullscreen_background = IMG_Load("fullscreen_background.jpeg");

    // Initialize TTF
    if (TTF_Init() == -1) {
        printf("TTF_Init Error: %s\n", TTF_GetError());
        return -1;
    }

    font = TTF_OpenFont("arial.ttf", 38);  // Load the font
    if (!font) {
        printf("Font loading error: %s\n", TTF_GetError());
        return -1;
    }

    return 0;
}

// Function to draw text
SDL_Surface* draw_text(const char *text) {
    return TTF_RenderText_Solid(font, text, textColor);
}

// Function to handle mouse hover events on buttons
void handle_mouse_over(SDL_Rect button_rect, SDL_Surface *button_normal, SDL_Surface *button_hovered, SDL_Surface **current_button) {
    int x, y;
    SDL_GetMouseState(&x, &y);

    if (x > button_rect.x && x < button_rect.x + button_rect.w &&
        y > button_rect.y && y < button_rect.y + button_rect.h) {
        *current_button = button_hovered;  // Change to hovered image
    } else {
        *current_button = button_normal;  // Normal image
    }
}

// Function to increase the volume
void increase_volume() {
    volume += 10;
    if (volume > 128) volume = 128;  // Maximum volume for SDL_mixer
    Mix_VolumeMusic(volume);
}

// Function to decrease the volume
void decrease_volume() {
    volume -= 10;
    if (volume < 0) volume = 0;  // Minimum volume for SDL_mixer
    Mix_VolumeMusic(volume);
}

// Function to toggle fullscreen mode
void toggle_fullscreen() {
    if (is_fullscreen) {
        screen = SDL_SetVideoMode(1280, 720, 32, SDL_SWSURFACE);
        is_fullscreen = 0;
    } else {
        screen = SDL_SetVideoMode(0, 0, 32, SDL_FULLSCREEN);
        is_fullscreen = 1;
    }
}

// Main game loop
void main_loop() {
    int running = 1;
    SDL_Event event;

    // Define button positions
    SDL_Rect volume_label_rect = { 200, 120, 0, 0 }; // "Volume" text
    SDL_Rect button_diminuer_rect = { 200, 200, 100, 50 }; // Decrease button
    SDL_Rect button_augmenter_rect = { 500, 200, 100, 50 }; // Increase button
    SDL_Rect display_label_rect = { 200, 270, 0, 0 }; // "Display Mode" text
    SDL_Rect button_plein_ecran_rect = { 200, 450, 100, 50 }; // Fullscreen button
    SDL_Rect button_normal_rect = { 500, 450, 100, 50 }; // Normal mode button
    SDL_Rect button_retour_rect = { 980, 650, 250, 50 }; // Back button

    SDL_Surface *current_button_diminuer = button_diminuer;
    SDL_Surface *current_button_augmenter = button_augmenter;
    SDL_Surface *current_button_plein_ecran = button_plein_ecran;
    SDL_Surface *current_button_normal = button_normal;
    SDL_Surface *current_button_retour = button_retour;

    // Play music in loop
    Mix_PlayMusic(music, -1);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }

            // Handle mouse hover events
            handle_mouse_over(button_diminuer_rect, button_diminuer, button_diminuer_hovered, &current_button_diminuer);
            handle_mouse_over(button_augmenter_rect, button_augmenter, button_augmenter_hovered, &current_button_augmenter);
            handle_mouse_over(button_plein_ecran_rect, button_plein_ecran, button_plein_ecran_hovered, &current_button_plein_ecran);
            handle_mouse_over(button_normal_rect, button_normal, button_normal_hovered, &current_button_normal);
            handle_mouse_over(button_retour_rect, button_retour, button_retour_hovered, &current_button_retour);

            // Handle mouse click events on buttons
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (event.button.x > button_diminuer_rect.x && event.button.x < button_diminuer_rect.x + button_diminuer_rect.w &&
                        event.button.y > button_diminuer_rect.y && event.button.y < button_diminuer_rect.y + button_diminuer_rect.h) {
                        decrease_volume();  // Decrease volume
                        Mix_PlayChannel(-1, click_sound, 0);  // Play click sound
                    }
                    if (event.button.x > button_augmenter_rect.x && event.button.x < button_augmenter_rect.x + button_augmenter_rect.w &&
                        event.button.y > button_augmenter_rect.y && event.button.y < button_augmenter_rect.y + button_augmenter_rect.h) {
                        increase_volume();  // Increase volume
                        Mix_PlayChannel(-1, click_sound, 0);  // Play click sound
                    }
                    if (event.button.x > button_plein_ecran_rect.x && event.button.x < button_plein_ecran_rect.x + button_plein_ecran_rect.w &&
                        event.button.y > button_plein_ecran_rect.y && event.button.y < button_plein_ecran_rect.y + button_plein_ecran_rect.h) {
                        toggle_fullscreen();  // Toggle fullscreen
                        Mix_PlayChannel(-1, click_sound, 0);  // Play click sound
                    }
                    if (event.button.x > button_normal_rect.x && event.button.x < button_normal_rect.x + button_normal_rect.w &&
                        event.button.y > button_normal_rect.y && event.button.y < button_normal_rect.y + button_normal_rect.h) {
                        screen = SDL_SetVideoMode(1280, 720, 32, SDL_SWSURFACE);  // Switch to normal mode
                        is_fullscreen = 0;
                        Mix_PlayChannel(-1, click_sound, 0);  // Play click sound
                    }
                    if (event.button.x > button_retour_rect.x && event.button.x < button_retour_rect.x + button_retour_rect.w &&
                        event.button.y > button_retour_rect.y && event.button.y < button_retour_rect.y + button_retour_rect.h) {
                        running = 0;  // Go back to main menu
                        Mix_PlayChannel(-1, click_sound, 0);  // Play click sound
                    }
                }
            }

            // Handle keyboard input
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = 0;  // Exit game
                }
                if (event.key.keysym.sym == SDLK_PLUS || event.key.keysym.sym == SDLK_KP_PLUS) {
                    increase_volume();
                    Mix_PlayChannel(-1, click_sound, 0);  // Play click sound
                }
                if (event.key.keysym.sym == SDLK_MINUS || event.key.keysym.sym == SDLK_KP_MINUS) {
                    decrease_volume();
                    Mix_PlayChannel(-1, click_sound, 0);  // Play click sound
                }
                if (event.key.keysym.sym == SDLK_F11) {
                    toggle_fullscreen();
                }
                if (event.key.keysym.sym == SDLK_r) {  // Press 'R' to return to 1280x720
                    screen = SDL_SetVideoMode(1280, 720, 32, SDL_SWSURFACE);
                    is_fullscreen = 0;
                }
            }
        }

        // Draw background and elements on screen
        if (is_fullscreen) {
            SDL_BlitSurface(fullscreen_background, NULL, screen, NULL);
        } else {
            SDL_BlitSurface(background, NULL, screen, NULL);
        }

        // Draw text labels "Volume" and "Display Mode"
        SDL_Surface *volume_label = draw_text("Volume");
        SDL_Surface *display_label = draw_text("Display Mode");

        // Position the text "Volume"
        SDL_Rect volume_label_position = { 200, 120, 0, 0 };
        // Position the text "Display Mode" (moved further down with more space)
        SDL_Rect display_label_position = { 200, 340, 0, 0 };

        SDL_BlitSurface(volume_label, NULL, screen, &volume_label_position);
        SDL_BlitSurface(display_label, NULL, screen, &display_label_position);

        // Draw buttons
        SDL_BlitSurface(current_button_diminuer, NULL, screen, &button_diminuer_rect);
        SDL_BlitSurface(current_button_augmenter, NULL, screen, &button_augmenter_rect);
        SDL_BlitSurface(current_button_plein_ecran, NULL, screen, &button_plein_ecran_rect);
        SDL_BlitSurface(current_button_normal, NULL, screen, &button_normal_rect);
        SDL_BlitSurface(current_button_retour, NULL, screen, &button_retour_rect);

        // Update the screen
        SDL_Flip(screen);

        // Free surfaces
        SDL_FreeSurface(volume_label);
        SDL_FreeSurface(display_label);
    }
}

