#include "menu.h"

#define SCREEN_WIDTH 1500
#define SCREEN_HEIGHT 800

SDL_Surface *background = NULL;
Mix_Music *music = NULL;
Mix_Chunk *click_sound = NULL;

int afficher_menu_options(SDL_Surface *screen) {
    SDL_Surface *btn_zqsd = IMG_Load("zqsd.png");
    SDL_Surface *btn_arrows = IMG_Load("arrows.png");
    SDL_Surface *btn_avatar1 = IMG_Load("avatar1.png");
    SDL_Surface *btn_avatar2 = IMG_Load("avatar2.png");
    SDL_Surface *btn_back = IMG_Load("back.png");
    SDL_Surface *logo = IMG_Load("LOGO.png");
    if (!btn_zqsd || !btn_arrows || !btn_avatar1 || !btn_avatar2 || !btn_back) {
        printf("Erreur de chargement des fichiers du menu options : %s\n", SDL_GetError());
        return 0;
    }

    SDL_Rect pos_zqsd = {400, 300, 0, 0};
    SDL_Rect pos_arrows = {800, 300, 0, 0};
    SDL_Rect pos_avatar1 = {400, 500, 0, 0};
    SDL_Rect pos_avatar2 = {800, 500, 0, 0};
    SDL_Rect pos_back = {500, 650, 0, 0};
    SDL_Rect pos_logo = {405, 50, 0, 0};


    int running = 1;
    SDL_Event event;

    while (running) {
        SDL_BlitSurface(background, NULL, screen, NULL);
        SDL_BlitSurface(btn_zqsd, NULL, screen, &pos_zqsd);
        SDL_BlitSurface(btn_arrows, NULL, screen, &pos_arrows);
        SDL_BlitSurface(btn_avatar1, NULL, screen, &pos_avatar1);
        SDL_BlitSurface(btn_avatar2, NULL, screen, &pos_avatar2);
        SDL_BlitSurface(btn_back, NULL, screen, &pos_back);
        SDL_BlitSurface(logo, NULL, screen, &pos_logo);
        SDL_Flip(screen);

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        int x = event.button.x;
                        int y = event.button.y;

                        if (x >= pos_zqsd.x && x <= pos_zqsd.x + btn_zqsd->w &&
                            y >= pos_zqsd.y && y <= pos_zqsd.y + btn_zqsd->h) {
                            Mix_PlayChannel(-1, click_sound, 0);
                            SDL_Delay(300);
                        }

                        if (x >= pos_arrows.x && x <= pos_arrows.x + btn_arrows->w &&
                            y >= pos_arrows.y && y <= pos_arrows.y + btn_arrows->h) {
                            Mix_PlayChannel(-1, click_sound, 0);
                            SDL_Delay(300);
                        }

                        if (x >= pos_avatar1.x && x <= pos_avatar1.x + btn_avatar1->w &&
                            y >= pos_avatar1.y && y <= pos_avatar1.y + btn_avatar1->h) {
                            Mix_PlayChannel(-1, click_sound, 0);
                            return 1;

                        }

                        if (x >= pos_avatar2.x && x <= pos_avatar2.x + btn_avatar2->w &&
                            y >= pos_avatar2.y && y <= pos_avatar2.y + btn_avatar2->h) {
                            Mix_PlayChannel(-1, click_sound, 0);
                                                         return 1;
                        }

                        if (x >= pos_back.x && x <= pos_back.x + btn_back->w &&
                            y >= pos_back.y && y <= pos_back.y + btn_back->h) {
                            Mix_PlayChannel(-1, click_sound, 0);
                            SDL_Delay(300);
                            running = 0;
                        }
                    }
                    break;
            }
        }
    }

    SDL_FreeSurface(btn_zqsd);
    SDL_FreeSurface(btn_arrows);
    SDL_FreeSurface(btn_avatar1);
    SDL_FreeSurface(btn_avatar2);
    SDL_FreeSurface(btn_back);
    SDL_FreeSurface(logo);
    return 0;
}

int  afficher_menu(SDL_Surface *screen) {
    if (!background) background = IMG_Load("background2.jpg");
    if (!music) music = Mix_LoadMUS("background.wav");
    if (!click_sound) click_sound = Mix_LoadWAV("click.wav");

    SDL_Surface *btn_single = IMG_Load("singleplayer.png");
    SDL_Surface *btn_multi = IMG_Load("multiplayer.png");
    SDL_Surface *btn_multi2 = IMG_Load("multiplayer.png");
    SDL_Surface *btn_back = IMG_Load("back.png");
    SDL_Surface *logo = IMG_Load("LOGO.png");
    if (!btn_single || !btn_multi || !btn_back) {
        printf("Erreur de chargement des fichiers du menu joueur : %s\n", SDL_GetError());
        return 0;
    }

    if (Mix_PlayingMusic() == 0) {
        Mix_PlayMusic(music, -1);
    }

    SDL_Rect pos_single = {475, 400, 0, 0};
    SDL_Rect pos_multi = {475, 500, 0, 0};
    SDL_Rect pos_multi2 = {475, 500, 0, 0};
    SDL_Rect pos_back = {475, 600, 0, 0};
	SDL_Rect pos_logo = {600, 50, 0, 0};

    int running = 1;
    SDL_Event event;

    while (running) {
        SDL_BlitSurface(background, NULL, screen, NULL);
        SDL_BlitSurface(btn_single, NULL, screen, &pos_single);
        SDL_BlitSurface(btn_multi, NULL, screen, &pos_multi);
        SDL_BlitSurface(btn_multi2, NULL, screen, &pos_multi2);
        SDL_BlitSurface(btn_back, NULL, screen, &pos_back);
        SDL_BlitSurface(logo, NULL, screen, &pos_logo);
        SDL_Flip(screen);
       int x;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        int x = event.button.x;
                        int y = event.button.y;

                        if (x >= pos_single.x && x <= pos_single.x + btn_single->w &&
                            y >= pos_single.y && y <= pos_single.y + btn_single->h) {
                            Mix_PlayChannel(-1, click_sound, 0);
                            SDL_Delay(300);
                            running = 0; // Quitte le menu actuel
                         x=afficher_menu_options(screen); // Affiche le menu options
                         return x;
                        }

                        if (x >= pos_back.x && x <= pos_back.x + btn_back->w &&
                            y >= pos_back.y && y <= pos_back.y + btn_back->h) {
                            Mix_PlayChannel(-1, click_sound, 0);
                         
                            running = 0;
                        }
                        
                         if (x >= pos_multi2.x && x <= pos_multi2.x + btn_multi2->w &&
                            y >= pos_multi2.y && y <= pos_multi2.y + btn_multi2->h) {
                            Mix_PlayChannel(-1, click_sound, 0);
                         
                            return 2;
                        }
                    }
                    break;
            }
        }
    }

    SDL_FreeSurface(btn_single);
    SDL_FreeSurface(btn_multi);
    SDL_FreeSurface(btn_multi2);
    SDL_FreeSurface(btn_back);
    SDL_FreeSurface(logo);
}


