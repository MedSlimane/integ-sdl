#ifndef ES_H_INCLUDED
#define ES_H_INCLUDED
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <math.h>


typedef struct 
{
	SDL_Rect pos;
	SDL_Surface* enem[17];
	int d,num;
	
}Ennemi;
typedef struct
{
	SDL_Surface *image;
	SDL_Rect pos;
	SDL_Rect rect;

	
}personne;

typedef struct Coeur
{
    SDL_Surface *image;
    SDL_Rect position;
    int visible;
} Coeur;


void initEnnemi(Ennemi*e);
void afficherEnnemi(Ennemi e, SDL_Surface * screen);
void animerEnnemi( Ennemi * e,SDL_Surface *screen);
void deplacer( Ennemi * e);
int collisionBB( personne p, Ennemi e);
void initCoeurs(Coeur coeurs[]);
void afficherCoeurs(Coeur coeurs[], SDL_Surface *screen);
int collisionTri(personne p, Coeur coeur) ;


#endif // ES_H_INCLUDED

