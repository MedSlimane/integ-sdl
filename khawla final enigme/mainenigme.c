#include "enigmetf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

int main()
{
	srand(time(NULL));
	
    
	SDL_Surface *ecran = NULL;
	ecran = SDL_SetVideoMode(1200, 800, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	int continuer = 1;
	SDL_Event event;

	enigmetf e;
	InitEnigme(&e, "fichier.txt");

	while (continuer)
	{
		 
		afficherEnigme(e, ecran);
		animer_background(&e);
		animer(&e);
		

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
			{
				continuer = 0;
			}
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_a:
					e.pos_selected = 1;
					break;

				case SDLK_b:
					e.pos_selected = 2;
					break;

				case SDLK_c:
					e.pos_selected = 3;
					break;

				case SDLK_RETURN:
					{
						verify_enigme(e, ecran);
						continuer = 0;
					}
					break;
				}
				break;
			case SDL_MOUSEMOTION:
			{
				if ((event.motion.x > e.pos_reponse1.x) && (event.motion.x < e.pos_reponse1.x + 240 ) && (event.motion.y > e.pos_reponse1.y) && (event.motion.y < e.pos_reponse1.y + 90))
				{
					e.pos_selected = 1;
				}
				else if ((event.motion.x > e.pos_reponse2.x) && (event.motion.x < e.pos_reponse2.x + 240) && (event.motion.y > e.pos_reponse2.y ) && (event.motion.y < e.pos_reponse2.y + 90))
				{
					e.pos_selected = 2;

				}
				else if ((event.motion.x > e.pos_reponse3.x) && (event.motion.x < e.pos_reponse3.x + 240) && (event.motion.y > e.pos_reponse3.y ) && (event.motion.y < e.pos_reponse3.y + 90))
				{
					e.pos_selected = 3;
				}
				else
					{
						e.pos_selected = 0;
					}
			}
			break;

			case SDL_MOUSEBUTTONDOWN:
				{
					if( event.button.button == SDL_BUTTON_LEFT )
					{
						if(e.pos_selected == 1 )
						{
							verify_enigme(e,ecran);
							continuer = 0;
						}
						else if(e.pos_selected == 2 )
						{
							verify_enigme(e,ecran);
							continuer = 0;
						}
						else if(e.pos_selected == 3 )
						{
							verify_enigme(e,ecran);
							continuer = 0;
						}

					}
				}
				break;
			}
		}

		SDL_Flip(ecran);
		
	}

	SDL_Delay(2000);
	free_Surface_enigme(&e);
	TTF_CloseFont(e.police1);
	TTF_CloseFont(e.police);
	TTF_Quit();
	SDL_Quit();
}
