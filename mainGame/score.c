#include "integrated.h"
#include <stdio.h>
#include <string.h>

// Structure pour stocker un score
typedef struct {
    char name[100];
    int score;
} PlayerScore;

// Initialisation du menu des scores
void initScoreMenu(ScoreMenu *menu) {
    // Chargement de l'image de fond
    menu->background = IMG_Load("img/background fr3(4).jpg");
    if (!menu->background) {
        printf("Erreur lors du chargement de l'image de fond : %s\n", IMG_GetError());
        menu->background = SDL_CreateRGBSurface(SDL_HWSURFACE, 1500, 800, 32, 0, 0, 0, 0);
        SDL_FillRect(menu->background, NULL, SDL_MapRGB(menu->background->format, 0, 0, 128));
    }
    
    // Chargement des boutons
    menu->button_validate = IMG_Load("img/VALIDER (11).jpg");
    if (!menu->button_validate) {
        printf("Erreur lors du chargement du bouton valider : %s\n", IMG_GetError());
        menu->button_validate = SDL_CreateRGBSurface(SDL_HWSURFACE, 150, 50, 32, 0, 0, 0, 0);
        SDL_FillRect(menu->button_validate, NULL, SDL_MapRGB(menu->button_validate->format, 0, 255, 0));
    }
    
    menu->button_return = IMG_Load("img/VALIDER (33).jpg");
    if (!menu->button_return) {
        printf("Erreur lors du chargement du bouton retour : %s\n", IMG_GetError());
        menu->button_return = SDL_CreateRGBSurface(SDL_HWSURFACE, 150, 50, 32, 0, 0, 0, 0);
        SDL_FillRect(menu->button_return, NULL, SDL_MapRGB(menu->button_return->format, 255, 255, 0));
    }
    
    menu->button_quit = IMG_Load("img/VALIDER (333).jpg");
    if (!menu->button_quit) {
        printf("Erreur lors du chargement du bouton quitter : %s\n", IMG_GetError());
        menu->button_quit = SDL_CreateRGBSurface(SDL_HWSURFACE, 150, 50, 32, 0, 0, 0, 0);
        SDL_FillRect(menu->button_quit, NULL, SDL_MapRGB(menu->button_quit->format, 255, 0, 0));
    }
    
    // Position des boutons
    menu->pos_button_validate.x = 700;
    menu->pos_button_validate.y = 700;
    
    menu->pos_button_return.x = 500;
    menu->pos_button_return.y = 700;
    
    menu->pos_button_quit.x = 900;
    menu->pos_button_quit.y = 700;
    
    // Zone de saisie du nom
    menu->zone_input.x = 550;
    menu->zone_input.y = 100;
    menu->zone_input.w = 400;
    menu->zone_input.h = 50;
    
    // Initialisation des états de survol
    menu->is_hovering_validate = 0;
    menu->is_hovering_return = 0;
    menu->is_hovering_quit = 0;
    
    // Chargement de la police
    menu->font = TTF_OpenFont("img/Arbutus-Regular.ttf", 24);
    if (!menu->font) {
        printf("Erreur lors du chargement de la police : %s\n", TTF_GetError());
        menu->font = TTF_OpenFont("arial.ttf", 24); // Fallback sur arial
    }
    
    // Couleur du texte (noir)
    menu->textColor.r = 0;
    menu->textColor.g = 0;
    menu->textColor.b = 0;
    
    // Initialisation du nom du joueur
    strcpy(menu->player_name, "");
    
    // Chargement des sons
    menu->click_sound = Mix_LoadWAV("img/button_click_fixed.wav");
    if (!menu->click_sound) {
        printf("Erreur lors du chargement du son : %s\n", Mix_GetError());
    }
    
    menu->validation_music = Mix_LoadMUS("img/victoir f.wav");
    if (!menu->validation_music) {
        printf("Erreur lors du chargement de la musique : %s\n", Mix_GetError());
    }
}

// Libération des ressources du menu des scores
void cleanupScoreMenu(ScoreMenu *menu) {
    if (menu->background) SDL_FreeSurface(menu->background);
    if (menu->button_validate) SDL_FreeSurface(menu->button_validate);
    if (menu->button_return) SDL_FreeSurface(menu->button_return);
    if (menu->button_quit) SDL_FreeSurface(menu->button_quit);
    if (menu->font) TTF_CloseFont(menu->font);
    if (menu->click_sound) Mix_FreeChunk(menu->click_sound);
    if (menu->validation_music) Mix_FreeMusic(menu->validation_music);
}

// Gestion des événements du menu des scores
void handleScoreMenuEvents(SDL_Event event, ScoreMenu *menu, ScoreMenuState *state, int *quit, int score) {
    switch (event.type) {
        case SDL_QUIT:
            *quit = 1;
            break;
            
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                *quit = 1;
            } 
            else if (*state == MENU_INPUT) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    // Enregistrement du score et passage à l'affichage
                    saveScore(menu->player_name, score);
                    if (menu->validation_music) {
                        Mix_PlayMusic(menu->validation_music, 0);
                    }
                    *state = MENU_SCORES_DISPLAY;
                } 
                else if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(menu->player_name) > 0) {
                    // Suppression du dernier caractère
                    menu->player_name[strlen(menu->player_name) - 1] = '\0';
                } 
                else if (event.key.keysym.sym >= SDLK_SPACE && event.key.keysym.sym <= SDLK_z) {
                    // Ajout d'un caractère si le nom n'est pas trop long
                    if (strlen(menu->player_name) < 99) {
                        int len = strlen(menu->player_name);
                        menu->player_name[len] = (char)event.key.keysym.sym;
                        menu->player_name[len + 1] = '\0';
                    }
                }
            }
            break;
            
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                if (*state == MENU_INPUT) {
                    // Clic sur le bouton Valider
                    if (event.button.x >= menu->pos_button_validate.x && 
                        event.button.x <= menu->pos_button_validate.x + menu->button_validate->w &&
                        event.button.y >= menu->pos_button_validate.y && 
                        event.button.y <= menu->pos_button_validate.y + menu->button_validate->h) {
                        
                        if (menu->click_sound) {
                            Mix_PlayChannel(-1, menu->click_sound, 0);
                        }
                        
                        // Enregistrement du score et passage à l'affichage
                        saveScore(menu->player_name, score);
                        if (menu->validation_music) {
                            Mix_PlayMusic(menu->validation_music, 0);
                        }
                        *state = MENU_SCORES_DISPLAY;
                    }
                } 
                else if (*state == MENU_SCORES_DISPLAY) {
                    // Clic sur le bouton Retour
                    if (event.button.x >= menu->pos_button_return.x && 
                        event.button.x <= menu->pos_button_return.x + menu->button_return->w &&
                        event.button.y >= menu->pos_button_return.y && 
                        event.button.y <= menu->pos_button_return.y + menu->button_return->h) {
                        
                        if (menu->click_sound) {
                            Mix_PlayChannel(-1, menu->click_sound, 0);
                        }
                        *state = MENU_INPUT;
                    }
                    // Clic sur le bouton Quitter
                    else if (event.button.x >= menu->pos_button_quit.x && 
                             event.button.x <= menu->pos_button_quit.x + menu->button_quit->w &&
                             event.button.y >= menu->pos_button_quit.y && 
                             event.button.y <= menu->pos_button_quit.y + menu->button_quit->h) {
                        
                        if (menu->click_sound) {
                            Mix_PlayChannel(-1, menu->click_sound, 0);
                        }
                        *quit = 1;
                    }
                }
            }
            break;
            
        case SDL_MOUSEMOTION:
            // Gestion du survol des boutons
            if (event.motion.x >= menu->pos_button_validate.x && 
                event.motion.x <= menu->pos_button_validate.x + menu->button_validate->w &&
                event.motion.y >= menu->pos_button_validate.y && 
                event.motion.y <= menu->pos_button_validate.y + menu->button_validate->h) {
                
                if (!menu->is_hovering_validate) {
                    menu->is_hovering_validate = 1;
                    if (menu->click_sound) {
                        Mix_PlayChannel(-1, menu->click_sound, 0);
                    }
                }
            } else {
                menu->is_hovering_validate = 0;
            }
            
            if (event.motion.x >= menu->pos_button_return.x && 
                event.motion.x <= menu->pos_button_return.x + menu->button_return->w &&
                event.motion.y >= menu->pos_button_return.y && 
                event.motion.y <= menu->pos_button_return.y + menu->button_return->h) {
                
                if (!menu->is_hovering_return) {
                    menu->is_hovering_return = 1;
                    if (menu->click_sound) {
                        Mix_PlayChannel(-1, menu->click_sound, 0);
                    }
                }
            } else {
                menu->is_hovering_return = 0;
            }
            
            if (event.motion.x >= menu->pos_button_quit.x && 
                event.motion.x <= menu->pos_button_quit.x + menu->button_quit->w &&
                event.motion.y >= menu->pos_button_quit.y && 
                event.motion.y <= menu->pos_button_quit.y + menu->button_quit->h) {
                
                if (!menu->is_hovering_quit) {
                    menu->is_hovering_quit = 1;
                    if (menu->click_sound) {
                        Mix_PlayChannel(-1, menu->click_sound, 0);
                    }
                }
            } else {
                menu->is_hovering_quit = 0;
            }
            break;
    }
}

// Affichage du menu des scores
void displayScoreMenu(SDL_Surface *screen, ScoreMenu *menu, ScoreMenuState state, int score) {
    // Affichage du fond
    SDL_BlitSurface(menu->background, NULL, screen, NULL);
    
    if (state == MENU_INPUT) {
        // Affichage du titre
        SDL_Surface *title_text = TTF_RenderText_Solid(menu->font, "SAISIR VOTRE NOM/PSEUDO", menu->textColor);
        SDL_Rect title_pos = {580, 50, 0, 0};
        SDL_BlitSurface(title_text, NULL, screen, &title_pos);
        SDL_FreeSurface(title_text);
        
        // Affichage du score
        char score_text[50];
        snprintf(score_text, 50, "Votre score: %d", score);
        SDL_Surface *score_surface = TTF_RenderText_Solid(menu->font, score_text, menu->textColor);
        SDL_Rect score_pos = {580, 200, 0, 0};
        SDL_BlitSurface(score_surface, NULL, screen, &score_pos);
        SDL_FreeSurface(score_surface);
        
        // Affichage de la zone de saisie
        SDL_FillRect(screen, &menu->zone_input, SDL_MapRGB(screen->format, 0, 0, 0));
        SDL_Rect contour = {menu->zone_input.x - 2, menu->zone_input.y - 2, menu->zone_input.w + 4, menu->zone_input.h + 4};
        SDL_FillRect(screen, &contour, SDL_MapRGB(screen->format, 255, 255, 255));
        
        // Affichage du nom en cours de saisie
        if (strlen(menu->player_name) > 0) {
            SDL_Surface *name_text = TTF_RenderText_Solid(menu->font, menu->player_name, menu->textColor);
            SDL_Rect name_pos = {menu->zone_input.x + 10, menu->zone_input.y + 10, 0, 0};
            SDL_BlitSurface(name_text, NULL, screen, &name_pos);
            SDL_FreeSurface(name_text);
        }
        
        // Affichage du bouton Valider
        SDL_BlitSurface(menu->button_validate, NULL, screen, &menu->pos_button_validate);
    } 
    else if (state == MENU_SCORES_DISPLAY) {
        // Affichage des scores
        displayScores(screen, menu);
        
        // Affichage des boutons Retour et Quitter
        SDL_BlitSurface(menu->button_return, NULL, screen, &menu->pos_button_return);
        SDL_BlitSurface(menu->button_quit, NULL, screen, &menu->pos_button_quit);
    }
    
    SDL_Flip(screen);
}

// Enregistrement du score dans un fichier
void saveScore(const char *player_name, int score) {
    // Si le nom est vide, utiliser "Anonyme"
    if (strlen(player_name) == 0) {
        player_name = "Anonyme";
    }
    
    // Lecture des scores existants
    FILE *file = fopen("scores.txt", "r");
    PlayerScore scores[10];
    int nb_scores = 0;
    
    if (file) {
        while (nb_scores < 10 && fscanf(file, "%99s %d", scores[nb_scores].name, &scores[nb_scores].score) == 2) {
            nb_scores++;
        }
        fclose(file);
    }
    
    // Ajout du nouveau score
    if (nb_scores < 10) {
        strcpy(scores[nb_scores].name, player_name);
        scores[nb_scores].score = score;
        nb_scores++;
    } 
    else {
        // Vérifier si le nouveau score est meilleur que le plus bas
        int min_score_idx = 0;
        for (int i = 1; i < 10; i++) {
            if (scores[i].score < scores[min_score_idx].score) {
                min_score_idx = i;
            }
        }
        
        if (score > scores[min_score_idx].score) {
            strcpy(scores[min_score_idx].name, player_name);
            scores[min_score_idx].score = score;
        }
    }
    
    // Tri des scores par ordre décroissant (tri à bulles simple)
    for (int i = 0; i < nb_scores - 1; i++) {
        for (int j = 0; j < nb_scores - i - 1; j++) {
            if (scores[j].score < scores[j + 1].score) {
                PlayerScore temp = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = temp;
            }
        }
    }
    
    // Enregistrement des scores triés
    file = fopen("scores.txt", "w");
    if (file) {
        for (int i = 0; i < nb_scores; i++) {
            fprintf(file, "%s %d\n", scores[i].name, scores[i].score);
        }
        fclose(file);
    }
}

// Affichage des meilleurs scores
void displayScores(SDL_Surface *screen, ScoreMenu *menu) {
    // Affichage du titre
    SDL_Surface *title_text = TTF_RenderText_Solid(menu->font, "Meilleurs Scores", menu->textColor);
    SDL_Rect title_pos = {700, 60, 0, 0};
    SDL_BlitSurface(title_text, NULL, screen, &title_pos);
    SDL_FreeSurface(title_text);
    
    // Lecture des scores
    FILE *file = fopen("scores.txt", "r");
    PlayerScore scores[10];
    int nb_scores = 0;
    
    if (file) {
        while (nb_scores < 10 && fscanf(file, "%99s %d", scores[nb_scores].name, &scores[nb_scores].score) == 2) {
            nb_scores++;
        }
        fclose(file);
    }
    
    // Si pas de scores, afficher un message
    if (nb_scores == 0) {
        SDL_Surface *no_scores = TTF_RenderText_Solid(menu->font, "Aucun score enregistré", menu->textColor);
        SDL_Rect no_scores_pos = {650, 150, 0, 0};
        SDL_BlitSurface(no_scores, NULL, screen, &no_scores_pos);
        SDL_FreeSurface(no_scores);
        return;
    }
    
    // Affichage des scores
    SDL_Rect pos = {600, 120, 0, 0};
    for (int i = 0; i < nb_scores; i++) {
        char score_text[150];
        snprintf(score_text, 150, "%d. %s: %d", i + 1, scores[i].name, scores[i].score);
        
        SDL_Surface *score_surface = TTF_RenderText_Solid(menu->font, score_text, menu->textColor);
        SDL_BlitSurface(score_surface, NULL, screen, &pos);
        SDL_FreeSurface(score_surface);
        
        pos.y += 60;
    }
}

// Incrémentation du score en fonction du déplacement
void incrementScore(Personne *p, Uint32 dt) {
    static int lastPosX = 0;
    static int lastPosY = 0;
    
    // Calcul de la distance parcourue
    int dx = abs(p->posX_absolue - lastPosX);
    int dy = abs(p->posY_absolue - lastPosY);
    
    // Augmentation du score basée sur la distance parcourue
    if (dx > 0 || dy > 0) {
        // On peut ajuster la formule en fonction de la vitesse désirée d'augmentation du score
        p->Vscore += (dx + dy) / 10;
    }
    
    // Mise à jour des dernières positions
    lastPosX = p->posX_absolue;
    lastPosY = p->posY_absolue;
    
    // Mise à jour du texte du score
    char score_text[20];
    sprintf(score_text, "Score: %d", p->Vscore);
    
    if (p->score.text != NULL) {
        SDL_FreeSurface(p->score.text);
    }
    p->score.text = TTF_RenderText_Solid(p->score.font, score_text, p->score.color);
}

// Boucle principale du menu des scores
int scoreMenuLoop(SDL_Surface *screen, int final_score) {
    ScoreMenu menu;
    initScoreMenu(&menu);
    
    int quit = 0;
    ScoreMenuState state = MENU_INPUT;
    SDL_Event event;
    
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            handleScoreMenuEvents(event, &menu, &state, &quit, final_score);
        }
        
        displayScoreMenu(screen, &menu, state, final_score);
        SDL_Delay(16);
    }
    
    cleanupScoreMenu(&menu);
    return 0;
}