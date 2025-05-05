#include "fonction.h"
#include <SDL/SDL_image.h>
#include <stdio.h>

void afficher_scores(SDL_Surface *ecran, SDL_Surface *image, TTF_Font *font) {
    SDL_Surface *texte;
    SDL_Color textColor = {0, 0, 0};
    SDL_Rect position;
    position.x = 700;
    position.y = 60;

    SDL_BlitSurface(image, NULL, ecran, NULL);

    texte = TTF_RenderText_Solid(font, "Meilleurs Scores", textColor);
    SDL_BlitSurface(texte, NULL, ecran, &position);
    SDL_FreeSurface(texte);

    char score1[100] = "Player1: 1500";
    char score2[100] = "Player2: 1200";
    char score3[100] = "Player3: 1000";

    position.y += 50;
    texte = TTF_RenderText_Solid(font, score1, textColor);
    SDL_BlitSurface(texte, NULL, ecran, &position);
    SDL_FreeSurface(texte);

    position.y += 65;
    texte = TTF_RenderText_Solid(font, score2, textColor);
    SDL_BlitSurface(texte, NULL, ecran, &position);
    SDL_FreeSurface(texte);

    position.y += 70;
    texte = TTF_RenderText_Solid(font, score3, textColor);
    SDL_BlitSurface(texte, NULL, ecran, &position);
    SDL_FreeSurface(texte);

    SDL_Surface *etoile = IMG_Load("img ela/etoilefin2.png");
    if (etoile) {
        SDL_Rect positionEtoile1 = {600, 100};
        SDL_Rect positionEtoile2 = {600, 150};
        SDL_Rect positionEtoile3 = {600, 200};
        SDL_BlitSurface(etoile, NULL, ecran, &positionEtoile1);
        SDL_BlitSurface(etoile, NULL, ecran, &positionEtoile2);
        SDL_BlitSurface(etoile, NULL, ecran, &positionEtoile3);
        SDL_FreeSurface(etoile);
    }
}
