#include "fonction.h"
#include <SDL/SDL_image.h>
#include <stdio.h>

void initBackgroundAndPlatforms(Background *bg, Platform platforms[], int *taille) {
    bg->posimg.x = 0;
    bg->posimg.y = 0;
    // Utilisation de IMG_Load pour charger une image PNG
    bg->img[0] = IMG_Load("bggggggggg.png"); // Remplace "background.png" par ton fichier PNG
    if (!bg->img[0]) {
        printf("Erreur chargement image fond : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Surface *terre = IMG_Load("background_terre1.bmp");
    SDL_Surface *fireSprite1 = IMG_Load("fire1.png");
    SDL_Surface *fireSprite2 = IMG_Load("fire2.png");
    SDL_Surface *fireSprite3 = IMG_Load("fire3.png");
    
    // Obstacle fixe supplémentaire
    SDL_Surface *obstacleImage1 = IMG_Load("obstacle1.png");
    SDL_Surface *obstacleImage2 = IMG_Load("obstacle2.png");

    bg->guide.image = IMG_Load("guide.bmp");
    bg->commentJouer.image = IMG_Load("comment_jouer23.bmp");
    bg->commentJouer.w = bg->commentJouer.image->w;
    bg->commentJouer.h = bg->commentJouer.image->h;

    if (!terre || !fireSprite1 || !fireSprite2 || !fireSprite3 || !bg->guide.image || !bg->commentJouer.image) {
        printf("Erreur chargement image : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    bg->guide.position.x = 1900 - bg->guide.image->w - 10;
    bg->guide.position.y = 10;
    bg->afficherCommentJouer = 0;

    for (int i = 0; i < 10; i++) {
        platforms[i].image = terre;
        platforms[i].position.x = i * terre->w;
        platforms[i].position.y = 900;
        platforms[i].vitesse = 0;
        platforms[i].isAnimated = 0;
    }

    platforms[10].image = fireSprite1;
    platforms[10].frames[0] = fireSprite1;
    platforms[10].frames[1] = fireSprite2;
    platforms[10].frames[2] = fireSprite3;
    platforms[10].position.x = 100;
    platforms[10].position.y = 720;
    platforms[10].vitesse = 0;
    platforms[10].isAnimated = 1;
    platforms[10].currentFrame = 0;
    platforms[10].frameWidth = fireSprite1->w;
    platforms[10].frameHeight = fireSprite1->h;
    platforms[10].lastFrameTime = SDL_GetTicks();
    
    // Adding two additional obstacles
    platforms[11].image = obstacleImage1;
    platforms[11].position.x = 800;
    platforms[11].position.y = 750;
    platforms[11].vitesse = 0;
    platforms[11].isAnimated = 0; // Fixed obstacle
    platforms[11].currentFrame = 0;
    platforms[11].frameWidth = obstacleImage1->w;
    platforms[11].frameHeight = obstacleImage1->h;

    platforms[12].image = obstacleImage2;
    platforms[12].position.x = 1500;
    platforms[12].position.y = 750;
    platforms[12].vitesse = 0;
    platforms[12].isAnimated = 0; // Fixed obstacle
    platforms[12].currentFrame = 0;
    platforms[12].frameWidth = obstacleImage2->w;
    platforms[12].frameHeight = obstacleImage2->h;

    *taille = 13; // Update the size to include new obstacles
}


void afficherPlatforms(Platform platforms[], int taille, SDL_Surface *screen, int bgX, int bgY) {
    Uint32 currentTime = SDL_GetTicks();
    for (int i = 0; i < taille; i++) {
        SDL_Rect dest = {platforms[i].position.x - bgX, platforms[i].position.y - bgY};

        if (platforms[i].isAnimated) {
            if (currentTime - platforms[i].lastFrameTime > 150) {
                platforms[i].currentFrame = (platforms[i].currentFrame + 1) % MAX_FRAMES;
                platforms[i].lastFrameTime = currentTime;
            }

            SDL_Rect src = {0, 0, platforms[i].frameWidth, platforms[i].frameHeight};
            SDL_BlitSurface(platforms[i].frames[platforms[i].currentFrame], &src, screen, &dest);
        } else {
            SDL_BlitSurface(platforms[i].image, NULL, screen, &dest);
        }
    }
}

void gererScrollingDeuxJoueurs(SDL_Event event, int *bgX1, int *bgY1, int *bgX2, int *bgY2, int scrollSpeed) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_RIGHT: *bgX1 += scrollSpeed; break;
            case SDLK_LEFT:  *bgX1 -= scrollSpeed; break;
            case SDLK_UP:    *bgY1 -= scrollSpeed; break;
            case SDLK_DOWN:  *bgY1 += scrollSpeed; break;

            case SDLK_d: *bgX2 += scrollSpeed; break;
            case SDLK_q: *bgX2 -= scrollSpeed; break;
            case SDLK_z: *bgY2 -= scrollSpeed; break;
            case SDLK_s: *bgY2 += scrollSpeed; break;
            default: break;
        }
    }
}

void gererTemps(int *timeLeft, Uint32 *lastTime) {
    Uint32 currentTime = SDL_GetTicks();
    Uint32 delta = currentTime - *lastTime;
    if (delta >= 1000) {
        *timeLeft -= delta / 1000;
        *lastTime = currentTime;
    }
}

void gererGuideEtClic(SDL_Event event, GuideButton *guide, SDL_SurfaceWithRect *commentJouer, int *afficherCommentJouer) {
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        int x = event.button.x;
        int y = event.button.y;
        if (x >= guide->position.x && x <= guide->position.x + guide->image->w &&
            y >= guide->position.y && y <= guide->position.y + guide->image->h) {
            *afficherCommentJouer = 1;
        }
    }

    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_g || event.key.keysym.sym == SDLK_h) {
            *afficherCommentJouer = 1;
        }
        if (event.key.keysym.sym == SDLK_r || event.key.keysym.sym == SDLK_ESCAPE) {
            *afficherCommentJouer = 0;
        }
    }
}

void afficherBackgroundEtElements(SDL_Surface *screen, Background *bg, Platform platforms[], int taille, TTF_Font *font, SDL_Color textColor, int timeLeft, int bgX, int bgY) {
    int bgWidth = bg->img[0]->w;
    int normalizedX = bgX % bgWidth;
    if (normalizedX < 0) normalizedX += bgWidth;

    SDL_Rect dest1 = {-normalizedX, -bgY};
    SDL_Rect dest2 = {-normalizedX + bgWidth, -bgY};
    SDL_BlitSurface(bg->img[0], NULL, screen, &dest1);
    SDL_BlitSurface(bg->img[0], NULL, screen, &dest2);

    afficherPlatforms(platforms, taille, screen, bgX, bgY);

    char timeText[50];
    snprintf(timeText, sizeof(timeText), "Temps: %d", timeLeft);
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, timeText, textColor);
    if (textSurface != NULL) {
        SDL_Rect textRect = {10, 10};
        SDL_BlitSurface(textSurface, NULL, screen, &textRect);
        SDL_FreeSurface(textSurface);
    }

    SDL_BlitSurface(bg->guide.image, NULL, screen, &bg->guide.position);

    if (bg->afficherCommentJouer) {
        SDL_Rect dest;
        dest.x = (screen->w - bg->commentJouer.w) / 2;
        dest.y = (screen->h - bg->commentJouer.h) / 2;
        SDL_BlitSurface(bg->commentJouer.image, NULL, screen, &dest);
    }
}
