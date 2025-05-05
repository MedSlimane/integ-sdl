#ifndef perso_H_INCLUDED  
#define perso_H_INCLUDED

#include <stdio.h>  
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h> 

typedef struct struct_text{
 TTF_Font *font;
 SDL_Color color;
 SDL_Surface* text;
 SDL_Rect postext;
}Text;
typedef struct {  

	SDL_Surface *imagePers;//image
	SDL_Rect position;

	SDL_Rect image; 
	int nbimage;
        SDL_Surface *imagec;
	SDL_Rect positionc;
        int nbcoeur;
        Text vie;
	int direction; 
	Text score; 
	double acceleration; 
	double vitesse;   
	double vx_saut;
	double v_grav ;
	double v_saut ;
	double v_y ;  
        int Vscore;
        int up;  // Flag to indicate if the character is jumping

	
}Personne;


void initPerso(Personne *p);  
void initPerso2(Personne *p);  
void afficherPerso (Personne p,SDL_Surface *screen);
void afficherPerso2(Personne p,SDL_Surface *screen);
void deplacerPerso(Personne *p ,Uint32 dt);
void animerPerso(Personne *p);
void saut_Personnage(Personne *P, Uint32 dt, int posx_absolu, int posy_absolu);

#endif
