#ifndef FONCTION_H
#define FONCTION_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#define MAX_FRAMES 3

typedef struct {
    SDL_Surface *image;
    SDL_Surface *frames[4];
    SDL_Rect position;
    int vitesse;
    int isAnimated;
    int currentFrame;
    int frameWidth;
    int frameHeight;
    Uint32 lastFrameTime;
} Platform;

typedef struct {
    SDL_Surface *image;
    SDL_Rect position;
} GuideButton;

typedef struct {
    SDL_Surface *image;
    int w, h;
} SDL_SurfaceWithRect;

typedef struct {
    SDL_Surface *img[1];
    SDL_Rect posimg;
    GuideButton guide;
    SDL_SurfaceWithRect commentJouer;
    int afficherCommentJouer;
} Background;

void initBackgroundAndPlatforms(Background *bg, Platform platforms[], int *taille);
void afficherPlatforms(Platform platforms[], int taille, SDL_Surface *screen, int bgX, int bgY);
void gererScrollingDeuxJoueurs(SDL_Event event, int *bgX1, int *bgY1, int *bgX2, int *bgY2, int scrollSpeed);
void gererTemps(int *timeLeft, Uint32 *lastTime);
void gererGuideEtClic(SDL_Event event, GuideButton *guide, SDL_SurfaceWithRect *commentJouer, int *afficherCommentJouer);
void afficherBackgroundEtElements(SDL_Surface *screen, Background *bg, Platform platforms[], int taille, TTF_Font *font, SDL_Color textColor, int timeLeft, int bgX, int bgY);

#endif 
