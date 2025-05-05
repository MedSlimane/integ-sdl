#ifndef FONCTION_H
#define FONCTION_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

typedef enum { MENU_SAISIE, MENU_SCORES } EtatJeu;

void afficher_scores(SDL_Surface *ecran, SDL_Surface *image, TTF_Font *font);

#endif
