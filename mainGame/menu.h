#ifndef MENU_H
#define MENU_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include "integrated.h"

// Constantes pour les modes de jeu
#define MODE_SOLO 1
#define MODE_DUO 2
#define MODE_QUITTER 0

// Structure pour le menu
typedef struct {
    SDL_Surface *background;
    SDL_Surface *logo;
    SDL_Surface *btn_solo;
    SDL_Surface *btn_duo;
    SDL_Surface *btn_quitter;
    SDL_Surface *btn_zqsd;
    SDL_Surface *btn_arrows;
    SDL_Surface *btn_avatar1;
    SDL_Surface *btn_avatar2;
    SDL_Surface *btn_back;
    Mix_Music *musique;
    Mix_Chunk *son_clic;
} Menu;

// Fonctions du menu
void initMenu(Menu *menu);
void libererMenu(Menu *menu);
int afficherMenu(SDL_Surface *screen, Menu *menu);
int afficherMenuOptions(SDL_Surface *screen, Menu *menu);

#endif