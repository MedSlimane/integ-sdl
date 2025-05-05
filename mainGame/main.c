#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include "integrated.h"
#include "menu.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

int main(int argc, char *argv[]) {
    // Initialisation de SDL, TTF, SDL_image et SDL_mixer
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Erreur lors de l'initialisation de SDL : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    
    if (TTF_Init() < 0) {
        fprintf(stderr, "Erreur lors de l'initialisation de TTF : %s\n", TTF_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }
    
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Erreur lors de l'initialisation de SDL_mixer : %s\n", Mix_GetError());
    }

    // Création de la fenêtre
    SDL_Surface *screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!screen) {
        fprintf(stderr, "Erreur lors de la création de la fenêtre : %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }
    
    SDL_WM_SetCaption("Jeu Intégré", NULL);
    
    // Initialisation du menu
    Menu menu;
    initMenu(&menu);
    
    // Affichage du menu et récupération du mode choisi
    int mode = afficherMenu(screen, &menu);
    
    // Lancement du jeu selon le mode choisi
    switch (mode) {
        case MODE_SOLO:
            jouerModeSolo(screen);
            break;
        case MODE_DUO:
            jouerModeDuo(screen);
            break;
        case MODE_QUITTER:
        default:
            // Sortir du jeu
            break;
    }
    
    // Libération des ressources du menu
    libererMenu(&menu);
    
    // Fermeture de SDL, TTF et SDL_mixer
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
    
    return EXIT_SUCCESS;
}