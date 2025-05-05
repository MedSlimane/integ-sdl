#ifndef ENIGMETF_H_INCLUDED
#define ENIGMETF_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <time.h>

typedef struct
{ 

	SDL_Surface *background, *background1,*background2;  
	SDL_Surface *question;	  
	SDL_Surface *reponses[4]; 
	SDL_Surface *button,*button_s;
	char  chquestion[50];
	char chrep[2][50];
	int rc,pos_selected;	   
	SDL_Rect pos_question,pos_reponse1,pos_reponse2,pos_reponse3; 
	int num_question;
	SDL_Rect pos_reponse1txt, pos_reponse2txt, pos_reponse3txt; 		  
	TTF_Font *police,*police1;
	SDL_Surface *image_clock;
	SDL_Rect pos_image_clock;

	SDL_Rect single_Clock;

	int clock_num,currentbackg;

} enigmetf;

void InitEnigme(enigmetf *e, char nomfichier[]);
void afficherEnigme(enigmetf e, SDL_Surface *ecran);
void displayClock(enigmetf e, SDL_Surface *ecran);
void verify_enigme(enigmetf e, SDL_Surface *ecran);
void animer(enigmetf *e);
void free_Surface_enigme(enigmetf *e);
void animer_background(enigmetf *e);

#endif
