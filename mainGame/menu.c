#include "menu.h"
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

// Initialisation du menu
void initMenu(Menu *menu) {
    // Chargement des images pour le menu principal
    menu->background = IMG_Load("img/menu/background2.jpg");
    menu->logo = IMG_Load("img/menu/LOGO.png");
    menu->btn_solo = IMG_Load("img/menu/singleplayer.png");
    menu->btn_duo = IMG_Load("img/menu/multiplayer.png");
    menu->btn_quitter = IMG_Load("img/menu/back.png");
    menu->btn_settings = IMG_Load("img/menu/settings.png");
    
    // Chargement des images pour le menu des options
    menu->btn_zqsd = IMG_Load("img/menu/zqsd.png");
    menu->btn_arrows = IMG_Load("img/menu/arrows.png");
    menu->btn_avatar1 = IMG_Load("img/menu/avatar1.png");
    menu->btn_avatar2 = IMG_Load("img/menu/avatar2.png");
    menu->btn_back = IMG_Load("img/menu/back.png");
    
    // Vérification du chargement des images
    if (!menu->background || !menu->logo || !menu->btn_solo || !menu->btn_duo || 
        !menu->btn_quitter || !menu->btn_zqsd || !menu->btn_arrows || 
        !menu->btn_avatar1 || !menu->btn_avatar2 || !menu->btn_back || !menu->btn_settings) {
        printf("Erreur de chargement des images du menu: %s\n", SDL_GetError());
        
        // Si l'image settings n'est pas trouvée, créer une image simple
        if (!menu->btn_settings) {
            printf("Création d'un bouton de remplacement pour les paramètres\n");
            TTF_Font *font = TTF_OpenFont("img/arial.ttf", 24);
            if (font) {
                SDL_Color textColor = {255, 255, 255};
                menu->btn_settings = TTF_RenderText_Solid(font, "Paramètres", textColor);
                TTF_CloseFont(font);
            } else {
                menu->btn_settings = SDL_CreateRGBSurface(SDL_SWSURFACE, 200, 50, 32, 0, 0, 0, 0);
                SDL_FillRect(menu->btn_settings, NULL, SDL_MapRGB(menu->btn_settings->format, 100, 100, 200));
            }
        }
        
        if (!menu->background || !menu->logo || !menu->btn_solo || !menu->btn_duo || 
            !menu->btn_quitter || !menu->btn_settings) {
            exit(EXIT_FAILURE);
        }
    }
    
    // Initialisation de SDL_mixer si ce n'est pas déjà fait
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Erreur lors de l'initialisation de SDL_mixer: %s\n", Mix_GetError());
    }
    
    // Chargement des sons
    menu->musique = Mix_LoadMUS("sound/background.wav");
    menu->son_clic = Mix_LoadWAV("sound/click.wav");
    
    if (!menu->musique || !menu->son_clic) {
        printf("Erreur de chargement des sons: %s\n", Mix_GetError());
    }
    
    // Démarrer la musique de fond
    if (Mix_PlayingMusic() == 0) {
        Mix_PlayMusic(menu->musique, -1);
    }
}

// Libération des ressources du menu
void libererMenu(Menu *menu) {
    SDL_FreeSurface(menu->background);
    SDL_FreeSurface(menu->logo);
    SDL_FreeSurface(menu->btn_solo);
    SDL_FreeSurface(menu->btn_duo);
    SDL_FreeSurface(menu->btn_quitter);
    SDL_FreeSurface(menu->btn_settings);
    SDL_FreeSurface(menu->btn_zqsd);
    SDL_FreeSurface(menu->btn_arrows);
    SDL_FreeSurface(menu->btn_avatar1);
    SDL_FreeSurface(menu->btn_avatar2);
    SDL_FreeSurface(menu->btn_back);
    
    Mix_FreeMusic(menu->musique);
    Mix_FreeChunk(menu->son_clic);
}

// Affichage du menu des options
int afficherMenuOptions(SDL_Surface *screen, Menu *menu) {
    // Positions des éléments du menu
    SDL_Rect pos_logo = {(SCREEN_WIDTH - menu->logo->w) / 2, 50, 0, 0};
    SDL_Rect pos_zqsd = {SCREEN_WIDTH / 2 - 300, 300, 0, 0};
    SDL_Rect pos_arrows = {SCREEN_WIDTH / 2 + 100, 300, 0, 0};
    SDL_Rect pos_avatar1 = {SCREEN_WIDTH / 2 - 300, 500, 0, 0};
    SDL_Rect pos_avatar2 = {SCREEN_WIDTH / 2 + 100, 500, 0, 0};
    SDL_Rect pos_back = {SCREEN_WIDTH / 2 - 100, 700, 0, 0};
    
    int running = 1;
    SDL_Event event;
    
    while (running) {
        // Affichage des éléments du menu
        SDL_BlitSurface(menu->background, NULL, screen, NULL);
        SDL_BlitSurface(menu->logo, NULL, screen, &pos_logo);
        SDL_BlitSurface(menu->btn_zqsd, NULL, screen, &pos_zqsd);
        SDL_BlitSurface(menu->btn_arrows, NULL, screen, &pos_arrows);
        SDL_BlitSurface(menu->btn_avatar1, NULL, screen, &pos_avatar1);
        SDL_BlitSurface(menu->btn_avatar2, NULL, screen, &pos_avatar2);
        SDL_BlitSurface(menu->btn_back, NULL, screen, &pos_back);
        SDL_Flip(screen);
        
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    return MODE_QUITTER;
                    
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        int x = event.button.x;
                        int y = event.button.y;
                        
                        // Clic sur le bouton ZQSD
                        if (x >= pos_zqsd.x && x <= pos_zqsd.x + menu->btn_zqsd->w &&
                            y >= pos_zqsd.y && y <= pos_zqsd.y + menu->btn_zqsd->h) {
                            Mix_PlayChannel(-1, menu->son_clic, 0);
                            SDL_Delay(200);
                        }
                        
                        // Clic sur le bouton flèches
                        if (x >= pos_arrows.x && x <= pos_arrows.x + menu->btn_arrows->w &&
                            y >= pos_arrows.y && y <= pos_arrows.y + menu->btn_arrows->h) {
                            Mix_PlayChannel(-1, menu->son_clic, 0);
                            SDL_Delay(200);
                        }
                        
                        // Clic sur l'avatar 1
                        if (x >= pos_avatar1.x && x <= pos_avatar1.x + menu->btn_avatar1->w &&
                            y >= pos_avatar1.y && y <= pos_avatar1.y + menu->btn_avatar1->h) {
                            Mix_PlayChannel(-1, menu->son_clic, 0);
                            return MODE_SOLO;
                        }
                        
                        // Clic sur l'avatar 2
                        if (x >= pos_avatar2.x && x <= pos_avatar2.x + menu->btn_avatar2->w &&
                            y >= pos_avatar2.y && y <= pos_avatar2.y + menu->btn_avatar2->h) {
                            Mix_PlayChannel(-1, menu->son_clic, 0);
                            return MODE_SOLO;
                        }
                        
                        // Clic sur le bouton retour
                        if (x >= pos_back.x && x <= pos_back.x + menu->btn_back->w &&
                            y >= pos_back.y && y <= pos_back.y + menu->btn_back->h) {
                            Mix_PlayChannel(-1, menu->son_clic, 0);
                            SDL_Delay(200);
                            return 0;
                        }
                    }
                    break;
            }
        }
    }
    
    return 0;
}

// Affichage du menu principal
int afficherMenu(SDL_Surface *screen, Menu *menu) {
    // Positions des éléments du menu
    SDL_Rect pos_logo = {(SCREEN_WIDTH - menu->logo->w) / 2, 50, 0, 0};
    SDL_Rect pos_solo = {(SCREEN_WIDTH - menu->btn_solo->w) / 2, 300, 0, 0};
    SDL_Rect pos_duo = {(SCREEN_WIDTH - menu->btn_duo->w) / 2, 450, 0, 0};
    SDL_Rect pos_settings = {(SCREEN_WIDTH - menu->btn_settings->w) / 2, 550, 0, 0};
    SDL_Rect pos_quitter = {(SCREEN_WIDTH - menu->btn_quitter->w) / 2, 650, 0, 0};
    
    int running = 1;
    SDL_Event event;
    int mode = 0;
    
    while (running) {
        // Affichage des éléments du menu
        SDL_BlitSurface(menu->background, NULL, screen, NULL);
        SDL_BlitSurface(menu->logo, NULL, screen, &pos_logo);
        SDL_BlitSurface(menu->btn_solo, NULL, screen, &pos_solo);
        SDL_BlitSurface(menu->btn_duo, NULL, screen, &pos_duo);
        SDL_BlitSurface(menu->btn_settings, NULL, screen, &pos_settings);
        SDL_BlitSurface(menu->btn_quitter, NULL, screen, &pos_quitter);
        SDL_Flip(screen);
        
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    return MODE_QUITTER;
                    
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        int x = event.button.x;
                        int y = event.button.y;
                        
                        // Clic sur le bouton Solo
                        if (x >= pos_solo.x && x <= pos_solo.x + menu->btn_solo->w &&
                            y >= pos_solo.y && y <= pos_solo.y + menu->btn_solo->h) {
                            Mix_PlayChannel(-1, menu->son_clic, 0);
                            mode = afficherMenuOptions(screen, menu);
                            if (mode != 0) {
                                return mode; // Retourner le mode choisi
                            }
                        }
                        
                        // Clic sur le bouton Duo
                        if (x >= pos_duo.x && x <= pos_duo.x + menu->btn_duo->w &&
                            y >= pos_duo.y && y <= pos_duo.y + menu->btn_duo->h) {
                            Mix_PlayChannel(-1, menu->son_clic, 0);
                            SDL_Delay(200);
                            return MODE_DUO;
                        }
                        
                        // Clic sur le bouton Paramètres
                        if (x >= pos_settings.x && x <= pos_settings.x + menu->btn_settings->w &&
                            y >= pos_settings.y && y <= pos_settings.y + menu->btn_settings->h) {
                            Mix_PlayChannel(-1, menu->son_clic, 0);
                            SDL_Delay(200);
                            return MODE_SETTINGS;
                        }
                        
                        // Clic sur le bouton quitter
                        if (x >= pos_quitter.x && x <= pos_quitter.x + menu->btn_quitter->w &&
                            y >= pos_quitter.y && y <= pos_quitter.y + menu->btn_quitter->h) {
                            Mix_PlayChannel(-1, menu->son_clic, 0);
                            SDL_Delay(200);
                            return MODE_QUITTER;
                        }
                    }
                    break;
            }
        }
    }
    
    return MODE_QUITTER;
}

// Affichage du menu des paramètres
void afficherMenuSettings(SDL_Surface **screen, Menu *menu, SettingsMenu *settingsMenu) {
    // Initialiser le menu des paramètres
    initSettingsMenu(settingsMenu);
    
    // Passer en mode plein écran si nécessaire
    if (settingsMenu->is_fullscreen) {
        *screen = SDL_SetVideoMode(0, 0, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
    }
    
    // Exécuter la boucle du menu des paramètres
    settingsMenuLoop(screen, settingsMenu);
    
    // Libérer les ressources du menu des paramètres
    cleanupSettingsMenu(settingsMenu);
}