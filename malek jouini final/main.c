#include "fonction.h"
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (init() < 0) {
        printf("Initialization failed!\n");
        return -1;
    }

    // Start the main loop
    main_loop();

    // Free resources
    Mix_FreeMusic(music);
    Mix_FreeChunk(click_sound);
    TTF_CloseFont(font);
    SDL_FreeSurface(button_diminuer);
    SDL_FreeSurface(button_diminuer_hovered);
    SDL_FreeSurface(button_augmenter);
    SDL_FreeSurface(button_augmenter_hovered);
    SDL_FreeSurface(button_plein_ecran);
    SDL_FreeSurface(button_plein_ecran_hovered);
    SDL_FreeSurface(button_normal);
    SDL_FreeSurface(button_normal_hovered);
    SDL_FreeSurface(button_retour);
    SDL_FreeSurface(button_retour_hovered);
    SDL_FreeSurface(background);
    SDL_FreeSurface(fullscreen_background);

    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();

    return 0;
}

