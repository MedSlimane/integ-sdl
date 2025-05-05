#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <string.h>
#include <stdio.h>
#include "fonction.h"

int main(int argc, char** argv) {
    SDL_Surface *ecran, *image, *surfaceTexte = NULL, *boutonImage = NULL, *boutonRetourImage = NULL, *boutonQuitterImage = NULL;
    SDL_Event event;
    Mix_Chunk *sonBref = NULL;
    Mix_Music *musique = NULL;
    int quitter = 0, saisir_nom = 1;
    int survolBoutonValider = 0, survolBoutonRetour = 0, survolBoutonQuitter = 0;
    SDL_Rect positiontext, zoneSaisie, positionNom, positionBouton, positionBoutonRetour, positionBoutonQuitter;
    EtatJeu etat = MENU_SAISIE;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("Echec d'initialisation de SDL : %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() == -1) {
        printf("Echec d'initialisation de TTF : %s\n", TTF_GetError());
        return 1;
    }

    ecran = SDL_SetVideoMode(1500, 800, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!ecran) {
        fprintf(stderr, "Echec de la création de la fenêtre : %s\n", SDL_GetError());
        return 1;
    }

    image = IMG_Load("img ela/background fr3(4).jpg");
    if (!image) {
        fprintf(stderr, "Echec du chargement de l'image : %s.\n", SDL_GetError());
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
        printf("Echec d'initialisation de SDL_mixer : %s\n", Mix_GetError());
        return 1;
    }

    boutonImage = IMG_Load("img ela/VALIDER (11).jpg");
    if (!boutonImage) {
        fprintf(stderr, "Echec du chargement du bouton valider : %s.\n", SDL_GetError());
        return 1;
    }
    boutonRetourImage = IMG_Load("img ela/VALIDER (33).jpg");
    if (!boutonRetourImage) {
        printf("Erreur chargement bouton Retour : %s\n", SDL_GetError());
        return 1;
    }
    boutonQuitterImage = IMG_Load("img ela/VALIDER (333).jpg");
    if (!boutonQuitterImage) {
        printf("Erreur chargement bouton Quitter : %s\n", SDL_GetError());
        return 1;
    }

    positionBouton.x = 700; positionBouton.y = 700;
    positionBoutonRetour.x = 500; positionBoutonRetour.y = 700;
    positionBoutonQuitter.x = 800; positionBoutonQuitter.y = 700;

    TTF_Font *font = TTF_OpenFont("img ela/Arbutus-Regular.ttf", 24);
    if (!font) {
        fprintf(stderr, "Echec du chargement de la police : %s\n", TTF_GetError());
        return 1;
    }

    SDL_Color textColor = {0, 0, 0};
    positiontext.x = 580;
    positiontext.y = 50;

    zoneSaisie.x = (ecran->w - 400) / 2;
    zoneSaisie.y = positiontext.y + 50;
    zoneSaisie.w = 400;
    zoneSaisie.h = 50;

    char nom[100] = "";

    sonBref = Mix_LoadWAV("img ela/button_click_fixed.wav");
    if (!sonBref) {
        printf("Erreur lors du chargement du son : %s\n", Mix_GetError());
        return 1;
    }
    musique = Mix_LoadMUS("img ela/victoir f.wav");
    if (!musique) {
        printf("Erreur lors du chargement de la musique : %s\n", Mix_GetError());
    } else {
        printf("Musique chargée avec succès !\n");
    }

    while (!quitter) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quitter = 1;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        quitter = 1;
                    } else if (event.key.keysym.sym == SDLK_RETURN && etat == MENU_SAISIE) {
                        etat = MENU_SCORES;
                    } else if (saisir_nom && event.key.keysym.sym == SDLK_BACKSPACE && strlen(nom) > 0) {
                        nom[strlen(nom) - 1] = '\0';
                    } else if (saisir_nom && event.key.keysym.sym >= SDLK_SPACE && event.key.keysym.sym <= SDLK_z) {
                        if (strlen(nom) < 99) {
                            int len = strlen(nom);
                            nom[len] = (char)event.key.keysym.sym;
                            nom[len + 1] = '\0';
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        if (etat == MENU_SAISIE) {
                            if (event.button.x >= positionBouton.x && event.button.x <= positionBouton.x + boutonImage->w &&
                                event.button.y >= positionBouton.y && event.button.y <= positionBouton.y + boutonImage->h) {
                                Mix_PlayChannel(-1, sonBref, 0);
                                if (Mix_PlayMusic(musique, 0) == -1) {
                                    printf("Erreur musique : %s\n", Mix_GetError());
                                }
                                etat = MENU_SCORES;
                            }
                        } else if (etat == MENU_SCORES) {
                            if (event.button.x >= positionBoutonRetour.x && event.button.x <= positionBoutonRetour.x + boutonRetourImage->w &&
                                event.button.y >= positionBoutonRetour.y && event.button.y <= positionBoutonRetour.y + boutonRetourImage->h) {
                                printf("Bouton Retour cliqué\n");
                                etat = MENU_SAISIE;
                            } else if (event.button.x >= positionBoutonQuitter.x && event.button.x <= positionBoutonQuitter.x + boutonQuitterImage->w &&
                                       event.button.y >= positionBoutonQuitter.y && event.button.y <= positionBoutonQuitter.y + boutonQuitterImage->h) {
                                printf("Bouton Quitter cliqué\n");
                                quitter = 1;
                            }
                        }
                    }
                    break;
                case SDL_MOUSEMOTION:
                    if (event.motion.x >= positionBouton.x && event.motion.x <= positionBouton.x + boutonImage->w &&
                        event.motion.y >= positionBouton.y && event.motion.y <= positionBouton.y + boutonImage->h) {
                        if (!survolBoutonValider) {
                            survolBoutonValider = 1;
                            Mix_PlayChannel(-1, sonBref, 0);
                        }
                    } else {
                        survolBoutonValider = 0;
                    }
                    if (event.motion.x >= positionBoutonRetour.x && event.motion.x <= positionBoutonRetour.x + boutonRetourImage->w &&
                        event.motion.y >= positionBoutonRetour.y && event.motion.y <= positionBoutonRetour.y + boutonRetourImage->h) {
                        if (!survolBoutonRetour) {
                            survolBoutonRetour = 1;
                            Mix_PlayChannel(-1, sonBref, 0);
                        }
                    } else {
                        survolBoutonRetour = 0;
                    }
                    if (event.motion.x >= positionBoutonQuitter.x && event.motion.x <= positionBoutonQuitter.x + boutonQuitterImage->w &&
                        event.motion.y >= positionBoutonQuitter.y && event.motion.y <= positionBoutonQuitter.y + boutonQuitterImage->h) {
                        if (!survolBoutonQuitter) {
                            survolBoutonQuitter = 1;
                            Mix_PlayChannel(-1, sonBref, 0);
                        }
                    } else {
                        survolBoutonQuitter = 0;
                    }
                    break;
            }
        }

        SDL_BlitSurface(image, NULL, ecran, NULL);

        if (etat == MENU_SAISIE) {
            surfaceTexte = TTF_RenderText_Solid(font, "SAISIR VOTRE NOM/PSEUDO", textColor);
            SDL_BlitSurface(surfaceTexte, NULL, ecran, &positiontext);
            SDL_FreeSurface(surfaceTexte);

            SDL_FillRect(ecran, &zoneSaisie, SDL_MapRGB(ecran->format, 0, 0, 0));
            SDL_Rect contour = {zoneSaisie.x - 2, zoneSaisie.y - 2, zoneSaisie.w + 4, zoneSaisie.h + 4};
            SDL_FillRect(ecran, &contour, SDL_MapRGB(ecran->format, 255, 255, 255));

            if (strlen(nom) > 0) {
                surfaceTexte = TTF_RenderText_Solid(font, nom, textColor);
                positionNom.x = zoneSaisie.x + 10;
                positionNom.y = zoneSaisie.y + 10;
                SDL_BlitSurface(surfaceTexte, NULL, ecran, &positionNom);
                SDL_FreeSurface(surfaceTexte);
            }

            SDL_BlitSurface(boutonImage, NULL, ecran, &positionBouton);
        } else if (etat == MENU_SCORES) {
            afficher_scores(ecran, image, font);
            SDL_BlitSurface(boutonRetourImage, NULL, ecran, &positionBoutonRetour);
            SDL_BlitSurface(boutonQuitterImage, NULL, ecran, &positionBoutonQuitter);
        }

        SDL_Flip(ecran);
        SDL_Delay(16);
    }

    // Libération des ressources à la fin
}
