#ifndef FONCTION_H
#define FONCTION_H

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

// Declare the global variables as extern
extern SDL_Surface *screen;
extern SDL_Surface *button_diminuer, *button_diminuer_hovered;
extern SDL_Surface *button_augmenter, *button_augmenter_hovered;
extern SDL_Surface *button_plein_ecran, *button_plein_ecran_hovered;
extern SDL_Surface *button_normal, *button_normal_hovered;
extern SDL_Surface *button_retour, *button_retour_hovered;
extern SDL_Surface *background, *fullscreen_background;
extern Mix_Music *music;
extern Mix_Chunk *click_sound;
extern TTF_Font *font;
extern SDL_Color textColor; // Color of the text (white)
extern int volume;  // Initial volume
extern int is_fullscreen; // Fullscreen state

// Function declarations
int init();
SDL_Surface* draw_text(const char *text);
void handle_mouse_over(SDL_Rect button_rect, SDL_Surface *button_normal, SDL_Surface *button_hovered, SDL_Surface **current_button);
void increase_volume();
void decrease_volume();
void toggle_fullscreen();
void main_loop();
void reload_images(); // Déclaration de la fonction

#endif

