#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "menu.h"
#include "perso.h"

int main() {
   int done=1;
    Uint32 dt = 0, t_prev;
    int jump_start_x1 = 0, jump_start_y1 = 700;
    int jump_start_x2 = 0, jump_start_y2 = 700;
    int x;
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface *screen;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("unable to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
        if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
        printf("Erreur SDL_mixer: %s\n", Mix_GetError());
        SDL_Quit();
        return 1;
    }
    screen = SDL_SetVideoMode(1920,1100, 32,
                              SDL_HWSURFACE | SDL_DOUBLEBUF);

    if (screen == NULL) {
        printf("unable to set video mode:%s\n", SDL_GetError());
        return 1;
    }
 
   

    // creation background
    SDL_Surface *backg;
    SDL_Rect backg_pos;
    backg = IMG_Load("background final.png");
    backg_pos.x = 0;
    backg_pos.y = 0;

    // Load player choice image
   
   
 

   

    SDL_WM_SetCaption("JEUX", NULL);

    // initialisation
   x=afficher_menu(screen);
   
   printf("la valeur de x est %d ",x);
    int etat_up = 0, etat_down = 0, etat_up2 = 0, etat_down2 = 0;

    Personne p;
    Personne p2;

    initPerso(&p);
    initPerso2(&p2);

    while (done) {
        t_prev = SDL_GetTicks();  // au début de la boucle de jeu//1000
  
  
    SDL_BlitSurface(backg, NULL, screen, &backg_pos);

        afficherPerso(p, screen);
        if (x == 2) {
            afficherPerso2(p2, screen);
        }
 
        SDL_PollEvent(&event);
        switch (event.type)

        {
            case SDL_QUIT:
                done = 0;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_s:
                        dt = 0;
                        deplacerPerso(&p, dt);
                        break;

                    case SDLK_d:
                        if (p.position.x < 1750) {
                            p.Vscore += 1;
                            p.direction = 1;
                            deplacerPerso(&p, dt);
                            animerPerso(&p);
                        }
                        break;

                    case SDLK_q:
                        if (p.position.x > 0) {
                            p.Vscore += 1;
                            p.direction = 2;
                            deplacerPerso(&p, dt);
                            animerPerso(&p);
                        }
                        break;
                        
                    case SDLK_z: // Jump key for player 1
                        if (p.up == 0) { // Only start a jump if not already jumping
                            jump_start_x1 = p.position.x;
                            jump_start_y1 = p.position.y;
                            p.up = 1;
                            // If no direction is currently set, default to right direction
                            if (p.direction != 1 && p.direction != 2) {
                                p.direction = 1; // Default to right direction for jump
                            }
                            animerPerso(&p); // Animate according to direction
                            p.nbcoeur = 2;

                        }
                        break;

                    case SDLK_i:
                        dt = 0;
                        deplacerPerso(&p2, dt);
                        break;

                    case SDLK_m:
                        if (p2.position.x < 1750) {
                            p2.Vscore += 1;
                            p2.direction = 1;
                            deplacerPerso(&p2, dt);
                            animerPerso(&p2);
                        }
                        break;

                    case SDLK_k:
                        if (p2.position.x > 0) {
                            p2.Vscore += 1;
                            p2.direction = 2;
                            deplacerPerso(&p2, dt);
                            animerPerso(&p2);
                        }
                        break;
                    case SDLK_o:
                        if (p2.up == 0) { // Jump key for player 2
                            jump_start_x2 = p2.position.x;
                            jump_start_y2 = p2.position.y;
                            p2.up = 1;
                            // If no direction is currently set, default to right direction
                            if (p2.direction != 1 && p2.direction != 2) {
                                p2.direction = 1; // Default to right direction for jump
                            }
                            animerPerso(&p2); // Animate according to direction
                        }
                        break;

                    case SDLK_SPACE:
                        p.direction = 3;
                        deplacerPerso(&p, dt);
                        p.nbcoeur = 3;

                        break;

                    case SDLK_UP:
                        etat_up = 1;
                        break;

                    case SDLK_u:
                        etat_up2 = 1;
                        break;
                    case SDLK_c:
                        p.nbcoeur = 2;
                        break;
                    case SDLK_t:
                        p.nbcoeur = 1;
                        break;

                    case SDLK_DOWN:
                        etat_down = 1;

                        break;
                    case SDLK_w:
                        etat_down2 = 1;

                        break;

                    case SDLK_a:
                        p.direction = 4; // Left attack
                        animerPerso(&p);
                        break;

                    case SDLK_e:
                        p.direction = 3; // Right attack
                        animerPerso(&p);
                        break;

                    case SDLK_ESCAPE:

                        done = 0;
                        break;
                }
                break;

            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_UP:

                        etat_up = 0;

                        break;

                    case SDLK_DOWN:
                        etat_down = 0;
                        break;

                    case SDLK_u:

                        etat_up2 = 0;

                        break;

                    case SDLK_w:

                        etat_down2 = 0;

                        break;
                }
                break;
        }
        if (etat_up == 1) {
            p.acceleration += 0.005;
            p.vitesse += p.acceleration;
        } else if (etat_down == 1) {
            p.acceleration -= 0.01;
            p.vitesse -= p.acceleration;
        }
        if (p.vitesse >= 30) {
            p.vitesse = 30;
        }
        if (p.acceleration < 0) {
            p.vitesse = 0;
            p.acceleration = 0;
        }
        if (x == 2) {
            if (etat_up2 == 1) {
                p2.acceleration += 0.005;
                p2.vitesse += p2.acceleration;
            } else if (etat_down2 == 1) {
                p2.acceleration -= 0.01;
                p2.vitesse -= p2.acceleration;
            }
            if (p2.vitesse >= 30) {
                p2.vitesse = 30;
            }
            if (p2.acceleration < 0) {
                p2.vitesse = 0;
                p2.acceleration = 0;
            }
        }
             if (p.up == 1) {
            saut_Personnage(&p, dt, jump_start_x1, jump_start_y1);
        }
        if (x == 2) {
            if (p2.up == 1) {
                saut_Personnage(&p2, dt, jump_start_x2, jump_start_y2);
            }
        }

        printf("acceleration  %f\n", p.acceleration);
        printf("vitesse  %f\n", p.vitesse);

        dt = (SDL_GetTicks() - t_prev);  // à la fin de la boucle de jeu 1000
        SDL_Flip(screen);
    }


  Mix_CloseAudio();
    SDL_FreeSurface(backg);
    SDL_Quit();

    return 0;
}
