#include "fonctions.h"
#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

SDL_Surface* charger_image(const char* nom_fichier) {
    SDL_Surface* temp = IMG_Load(nom_fichier);
    if (!temp) {
        fprintf(stderr, "Erreur de chargement de l'image %s : %s\n", nom_fichier, IMG_GetError());
        return NULL;
    }
    SDL_Surface* image;
    // Conserver la transparence si l'image possède un canal alpha
    if (temp->format->Amask)
        image = SDL_DisplayFormatAlpha(temp);
    else
        image = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);
    return image;
}

void afficher_image(SDL_Surface* image, SDL_Surface* ecran, int x, int y) {
    SDL_Rect pos = { x, y, 0, 0 };
    SDL_BlitSurface(image, NULL, ecran, &pos);
}

int generer_nombre_aleatoire(int min, int max) {
    return min + rand() % (max - min + 1);
}

void afficher_message(SDL_Surface* ecran, const char* image_message) {
    SDL_Surface* message = charger_image(image_message);
    if (!message)
        return;
    
    double angle = 0.0;
    // Animation : faire varier progressivement zoom et rotation
    for (double zoom = 0.5; zoom <= 1.5; zoom += 0.05) {
        SDL_Surface* image_zoomee = rotozoomSurface(message, angle, zoom, 1);
        if (!image_zoomee)
            break;
        
        // Effacer l'écran
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
        
        // Centrer l'image
        SDL_Rect pos = { (ecran->w - image_zoomee->w) / 2,
                         (ecran->h - image_zoomee->h) / 2, 0, 0 };
        SDL_BlitSurface(image_zoomee, NULL, ecran, &pos);
        SDL_Flip(ecran);
        
        SDL_FreeSurface(image_zoomee);
        SDL_Delay(50);
        angle += 5;
    }
    SDL_FreeSurface(message);
}

Uint32 animer_chrono(Uint32 intervalle, void* param) {
    SDL_Surface* ecran = (SDL_Surface*) param;
    static int largeur = 0;
    largeur = (largeur + 5) % (ecran->w - 20);
    
    SDL_Rect rect = { 10, 10, largeur, 20 };
    SDL_FillRect(ecran, &rect, SDL_MapRGB(ecran->format, 255, 0, 0));
    SDL_Flip(ecran);
    
    return intervalle;
}

