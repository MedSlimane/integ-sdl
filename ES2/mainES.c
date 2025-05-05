#include "headerES.h"

int main(char argc, char argv[])
{
	Ennemi e;
	int continuer = 1,i;
	int c, c1;
	int largeur_fenetre = 1280;
	int hauteur_fenetre = 640;
	SDL_Surface *screen = NULL;
	SDL_Surface *background = NULL;
	SDL_Rect pos;
	SDL_Event event;
	personne p;
	Coeur coeurs[8];
	int collision = 0;
SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
	
	screen = SDL_SetVideoMode(largeur_fenetre, hauteur_fenetre, 32, SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF);
	SDL_WM_SetCaption("ENNEMI", NULL);
	
	background = IMG_Load("resources/image/background2.jpg");
	pos.x = 0;
	pos.y = 0;

   p.image = IMG_Load("resources/image/ligne.png");
p.pos.x = 100;
p.pos.y = 280; 
p.pos.w = p.image->w;
p.pos.h = p.image->h;

     
     
     
     
    initEnnemi(&e);
    initCoeurs(coeurs);

	while(continuer)
	{ 
	SDL_BlitSurface(background,NULL,screen,&pos);
             SDL_BlitSurface(p.image,NULL,screen,&p.pos);


		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
				continuer = 0;
				break;
				case SDL_KEYDOWN:
				    switch (event.key.keysym.sym) 
                    {
                    	case SDLK_q:
                    	continuer = 0;
                    	break;
                    	case SDLK_d:
                    	p.pos.x+=7;
                    	break;
                    	case SDLK_g:
                    	p.pos.x-=7;
                    	break;
                    }
				break;
			}
		}

                 afficherCoeurs(coeurs, screen);
                 afficherEnnemi(e, screen);
                 deplacer(&e);

                 animerEnnemi(&e,screen);
		 
	  c = collisionBB(p,e);

    if(c == 1) 
    {
        printf("COLLISION DETECTED\n");
        
           
    }
    else {
    printf(" PAS COLLISION \n");}
    

    for (int i = 0; i < 8; i++) 
    {
        c1 = collisionTri(p, coeurs[i]);
        if(c1 == 1) {
            printf(" bonus\n");
        }
        else {
       printf(" PAS BONUS \n");}
    
    }
  }
	

	
	SDL_FreeSurface(background);
	SDL_FreeSurface(screen);
	SDL_FreeSurface(p.image);
for(i=0; i<17 ;i++)
	{
		
			SDL_FreeSurface(e.enem[i]);
		
	}

    SDL_Quit();
	return 0;
}
