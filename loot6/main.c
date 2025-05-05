#include <SDL/SDL.h>
#include <SDL/SDL_timer.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include <X11/Xlib.h>                   
#include "fonctions.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

int main(int argc, char *argv[]) {
    // Initialisation multi-thread pour X11
    if (!XInitThreads()) {
         fprintf(stderr, "XInitThreads a échoué\n");
         return EXIT_FAILURE;
    }
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
         fprintf(stderr, "Erreur d'initialisation de SDL : %s\n", SDL_GetError());
         return EXIT_FAILURE;
    }
    
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
         fprintf(stderr, "Erreur d'initialisation de SDL_image : %s\n", IMG_GetError());
         SDL_Quit();
         return EXIT_FAILURE;
    }
    
    SDL_Surface* screen = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);
    if (!screen) {
         fprintf(stderr, "Erreur de création de la fenêtre : %s\n", SDL_GetError());
         IMG_Quit();
         SDL_Quit();
         return EXIT_FAILURE;
    }
    SDL_WM_SetCaption("Puzzle Fantasy & Myths (Corrigé)", NULL);
    
    srand(time(NULL));
    
    // Charger et redimensionner le puzzle (600 px de large)
    SDL_Surface* puzzleOrig = charger_image("puzzel.jpeg");
    if (!puzzleOrig) {
         fprintf(stderr, "Erreur lors du chargement de puzzle.png\n");
         IMG_Quit();
         SDL_Quit();
         return EXIT_FAILURE;
    }
    double scalePuzzle = 600.0 / puzzleOrig->w;
    SDL_Surface* puzzleResized = rotozoomSurface(puzzleOrig, 0, scalePuzzle, 1);
    SDL_FreeSurface(puzzleOrig);
    
    // Charger et redimensionner les propositions (150 px de large)
    SDL_Surface* prop1Orig = charger_image("prp.jpeg");
    SDL_Surface* prop2Orig = charger_image("bg.png");
    SDL_Surface* prop3Orig = charger_image("lg.png");
    if (!prop1Orig || !prop2Orig || !prop3Orig) {
         fprintf(stderr, "Erreur lors du chargement des propositions\n");
         IMG_Quit();
         SDL_Quit();
         return EXIT_FAILURE;
    }
    double scaleProp = 50.0 / prop1Orig->w;
 // Suppose que prop1, prop2, prop3 ont tailles similaires
    SDL_Surface* prop1 = rotozoomSurface(prop1Orig, 0, scaleProp, 1);
    SDL_Surface* prop2 = rotozoomSurface(prop2Orig, 0, scaleProp, 1);
    SDL_Surface* prop3 = rotozoomSurface(prop3Orig, 0, scaleProp, 1);
    SDL_FreeSurface(prop1Orig);
    SDL_FreeSurface(prop2Orig);
    SDL_FreeSurface(prop3Orig);
    
    SDL_Surface* proposals[3] = { prop1, prop2, prop3 };
    
    // Position du puzzle en haut (centré)
    int puzzle_x = (screen->w - puzzleResized->w) / 2;
    int puzzle_y = 50;
    SDL_Rect pos_cible = { puzzle_x, puzzle_y, puzzleResized->w, puzzleResized->h };
    
    // Position des propositions en bas, centrées
    int spacing = 20;
    int totalWidth = proposals[0]->w + proposals[1]->w + proposals[2]->w + 2 * spacing;
    int start_x = (screen->w - totalWidth) / 2;
    int proposal_y = screen->h - proposals[0]->h - 50;
    
    // Débogage : afficher les zones de chaque proposition
    int xDebug = start_x;
    for (int i = 0; i < 3; i++) {
         SDL_Rect rect = { xDebug, proposal_y, proposals[i]->w, proposals[i]->h };
         printf("Proposition %d -> x: [%d..%d], y: [%d..%d]\n",
                i+1, rect.x, rect.x + rect.w, rect.y, rect.y + rect.h);
         xDebug += proposals[i]->w + spacing;
    }
    
    // Variables pour drag & drop
    bool pieceSelected = false;
    bool dragging = false;
    SDL_Rect pos_piece;  // position de la pièce déplacée
    int offset_x = 0, offset_y = 0;
    
    // Choix aléatoire de la bonne proposition
    int correctIndex = generer_nombre_aleatoire(0, 2);
    printf("La bonne proposition est la %d\n", correctIndex + 1);
    
    bool running = true;
    bool solved = false;
    SDL_Event event;
    
    // Démarrer un timer pour le chrono (optionnel)
    SDL_AddTimer(100, animer_chrono, screen);
    
    while (running) {
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        
        // Afficher puzzle
        afficher_image(puzzleResized, screen, puzzle_x, puzzle_y);
        
        // Afficher propositions
        int x = start_x;
        for (int i = 0; i < 3; i++) {
            afficher_image(proposals[i], screen, x, proposal_y);
            x += proposals[i]->w + spacing;
        }
        
        // Si la pièce correcte est sélectionnée, l'afficher en cours de drag
        if (pieceSelected) {
            afficher_image(proposals[correctIndex], screen, pos_piece.x, pos_piece.y);
        }
        
        SDL_Flip(screen);
        
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mx = event.button.x;
                int my = event.button.y;
                printf("Clic à (%d, %d)\n", mx, my);
                
                // Vérifier clic sur proposition seulement si pas encore sélectionnée
                if (!pieceSelected) {
                    int xPos = start_x;
                    for (int i = 0; i < 3; i++) {
                        SDL_Rect rect = { xPos, proposal_y, proposals[i]->w, proposals[i]->h };
                        if (mx >= rect.x && mx <= rect.x + rect.w &&
                            my >= rect.y && my <= rect.y + rect.h) {
                            
                            printf("Clic détecté sur la proposition %d\n", i + 1);
                            
                            // Vérifier si c'est la bonne
                            if (i == correctIndex) {
                                pieceSelected = true;
                                dragging = true;
                                pos_piece.x = rect.x;
                                pos_piece.y = rect.y;
                                pos_piece.w = proposals[i]->w;
                                pos_piece.h = proposals[i]->h;
                                offset_x = mx - rect.x;
                                offset_y = my - rect.y;
                            } else {
                                // Au lieu de "DEFEAT", on affiche un message et on continue
                                printf("Mauvaise proposition, réessayez !\n");
                            }
                            
                            // IMPORTANT : break pour ne pas tester les autres propositions
                            break;
                        }
                        xPos += proposals[i]->w + spacing;
                    }
                }
            } else if (event.type == SDL_MOUSEMOTION) {
                if (dragging) {
                    int mx = event.motion.x;
                    int my = event.motion.y;
                    pos_piece.x = mx - offset_x;
                    pos_piece.y = my - offset_y;
                }
            } else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
                if (dragging) {
                    dragging = false;
                    // Vérifier si la pièce est proche de la zone cible
                    int marge = 20;
                    if (fabs(pos_piece.x - pos_cible.x) < marge &&
                        fabs(pos_piece.y - pos_cible.y) < marge) {
                        solved = true;
                    }
                    running = false;
                }
            }
        }
        
        SDL_Delay(16);
    }
    
    // Affichage final du message
    if (pieceSelected) {
        if (solved)
            afficher_message(screen, "v.jpeg");
        else
            afficher_message(screen, "d.jpeg");
    }
    
    // Boucle d'attente pour voir le message
    bool wait = true;
    while (wait) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                wait = false;
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
                wait = false;
        }
        SDL_Delay(16);
    }
    
    // Libération
    SDL_FreeSurface(puzzleResized);
    SDL_FreeSurface(prop1);
    SDL_FreeSurface(prop2);
    SDL_FreeSurface(prop3);
    IMG_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}

