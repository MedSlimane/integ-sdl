#ifndef FONCTIONS_H
#define FONCTIONS_H

#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>
#include <stdbool.h>

// Charge une image (PNG, JPG, etc.) via SDL_image et retourne la surface optimisée
SDL_Surface* charger_image(const char* nom_fichier);

// Affiche une image sur l'écran à la position (x, y)
void afficher_image(SDL_Surface* image, SDL_Surface* ecran, int x, int y);

// Génère un nombre aléatoire entre min et max (inclus)
int generer_nombre_aleatoire(int min, int max);

// Affiche un message (image) avec effet rotozoom (animation de rotation/zoom)
void afficher_message(SDL_Surface* ecran, const char* image_message);

// Animation du chronomètre (exemple d'un rectangle animé)
Uint32 animer_chrono(Uint32 intervalle, void* param);

#endif

