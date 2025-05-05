#include <SDL/SDL.h> 
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include "fonction.h"

int main(int argc, char *argv[]) {
    SDL_Surface *screen = NULL;
    SDL_Surface *textSurface1 = NULL, *textSurface2 = NULL;
    TTF_Font *font = NULL;
    SDL_Color textColor = {255, 255, 255};

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("Erreur TTF_Init : %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    screen = SDL_SetVideoMode(1920, 1080, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!screen) {
        printf("Erreur SDL_SetVideoMode : %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    Background bg;
    Platform platforms[20];
    int taille = 0;
    initBackgroundAndPlatforms(&bg, platforms, &taille);

    font = TTF_OpenFont("arial.ttf", 24);
    if (!font) {
        printf("Erreur TTF_OpenFont : %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    int timeLeft = 1000;
    Uint32 lastTime = SDL_GetTicks();

    int scrollSpeed = 20;
    int bgX1 = 0, bgY1 = 0;
    int bgX2 = 0, bgY2 = 0;

    int partageEcran = 0;

    SDL_Event event;
    int running = 1;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = 0;

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p) {
                partageEcran = !partageEcran; 
            }

            gererGuideEtClic(event, &bg.guide, &bg.commentJouer, &bg.afficherCommentJouer);
            gererScrollingDeuxJoueurs(event, &bgX1, &bgY1, &bgX2, &bgY2, scrollSpeed);
        }

        gererTemps(&timeLeft, &lastTime);

        if (partageEcran) {
            SDL_Rect rectGauche = {0, 0, screen->w / 2, screen->h}; 
            SDL_Rect rectDroite = {screen->w / 2, 0, screen->w / 2, screen->h};

            SDL_SetClipRect(screen, &rectGauche);
            afficherBackgroundEtElements(screen, &bg, platforms, taille, font, textColor, timeLeft, bgX1, bgY1);

            SDL_SetClipRect(screen, &rectDroite);
            afficherBackgroundEtElements(screen, &bg, platforms, taille, font, textColor, timeLeft, bgX2, bgY2);
        } else {
            afficherBackgroundEtElements(screen, &bg, platforms, taille, font, textColor, timeLeft, bgX1, bgY1);
        }

        SDL_SetClipRect(screen, NULL);
        SDL_Flip(screen);
        SDL_Delay(16);
    }

    SDL_FreeSurface(bg.img[0]);
    for (int p = 0; p < taille; p++) {
        for (int i = 0; i < MAX_FRAMES; i++) {
            if (platforms[p].frames[i] != NULL) {
                SDL_FreeSurface(platforms[p].frames[i]);
            }
        }
    }

    SDL_FreeSurface(bg.guide.image);
    SDL_FreeSurface(bg.commentJouer.image);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return 0;
} 
