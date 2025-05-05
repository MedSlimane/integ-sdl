#ifndef INTEGRATED_H
#define INTEGRATED_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <math.h>

// Structure pour le texte
typedef struct {
    TTF_Font *font;
    SDL_Color color;
    SDL_Surface* text;
    SDL_Rect postext;
} Text;

// Structure pour l'énigme
typedef struct { 
    SDL_Surface *background, *background1, *background2;  
    SDL_Surface *question;    
    SDL_Surface *reponses[4]; 
    SDL_Surface *button, *button_s;
    char chquestion[50];
    char chrep[2][50];
    int rc, pos_selected;     
    SDL_Rect pos_question, pos_reponse1, pos_reponse2, pos_reponse3; 
    int num_question;
    SDL_Rect pos_reponse1txt, pos_reponse2txt, pos_reponse3txt;        
    TTF_Font *police, *police1;
    SDL_Surface *image_clock;
    SDL_Rect pos_image_clock;
    SDL_Rect single_Clock;
    int clock_num, currentbackg;
} enigmetf;

// Structure pour le menu des paramètres
typedef struct {
    SDL_Surface *background;
    SDL_Surface *fullscreen_background;
    SDL_Surface *button_diminuer, *button_diminuer_hovered;
    SDL_Surface *button_augmenter, *button_augmenter_hovered;
    SDL_Surface *button_plein_ecran, *button_plein_ecran_hovered;
    SDL_Surface *button_normal, *button_normal_hovered;
    SDL_Surface *button_retour, *button_retour_hovered;
    Mix_Chunk *click_sound;
    TTF_Font *font;
    SDL_Color textColor;
    int volume;
    int is_fullscreen;
} SettingsMenu;

// Structure pour le menu des scores
typedef struct {
    SDL_Surface *background;
    SDL_Surface *button_validate;
    SDL_Surface *button_return;
    SDL_Surface *button_quit;
    SDL_Rect zone_input;
    SDL_Rect pos_button_validate;
    SDL_Rect pos_button_return;
    SDL_Rect pos_button_quit;
    int is_hovering_validate;
    int is_hovering_return;
    int is_hovering_quit;
    TTF_Font *font;
    SDL_Color textColor;
    char player_name[100];
    Mix_Chunk *click_sound;
    Mix_Music *validation_music;
} ScoreMenu;

typedef enum { MENU_INPUT, MENU_SCORES_DISPLAY } ScoreMenuState;

// Structure pour le personnage
typedef struct {
    SDL_Surface *imagePers;
    SDL_Rect position;
    SDL_Rect image; // Rectangle de l'image source (spritesheet)
    int nbimage;    // Numéro de l'image courante dans l'animation
    SDL_Surface *imagec; // Image du coeur
    SDL_Rect positionc;  // Position des coeurs
    int nbcoeur;         // Nombre de coeurs (points de vie)
    Text vie;            // Texte pour l'affichage des vies
    int direction;       // Direction du mouvement
    int lastDirection;   // Dernière direction du mouvement (pour le saut)
    Text score;          // Texte pour l'affichage du score
    double acceleration; // Accélération du personnage
    double vitesse;      // Vitesse de déplacement
    double vx_saut;      // Vitesse horizontale en saut
    double v_grav;       // Gravité
    double v_saut;       // Vitesse initiale du saut
    double v_y;          // Vitesse verticale courante
    int Vscore;          // Valeur du score
    int up;              // Indicateur de saut
    int controls;        // 0 pour les flèches, 1 pour ZQSD
    int posX_absolue;    // Position X absolue dans le monde
    int posY_absolue;    // Position Y absolue dans le monde
} Personne;

// Structure pour l'ennemi
typedef struct {
    SDL_Rect pos;
    SDL_Surface* enem[18]; // Tableau des frames de l'animation (0-17)
    int d;                 // Direction du déplacement (0=droite, 1=gauche)
    int num;               // Numéro de l'image courante
    Uint32 lastFrameTime;  // Temps de la dernière mise à jour d'animation
    int active;            // Indique si l'ennemi est actif ou non
} Ennemi;

// Structure pour les objets bonus de type coeur
typedef struct {
    SDL_Surface *image;
    SDL_Rect position;
    int visible;
} Coeur;

// Structure pour les plateformes et obstacles
typedef struct {
    SDL_Surface *image;
    SDL_Surface *frames[4]; // Pour l'animation
    SDL_Rect position;
    int vitesse;
    int isAnimated;
    int currentFrame;
    int frameWidth;
    int frameHeight;
    Uint32 lastFrameTime;
} Platform;

// Structure pour le bouton guide
typedef struct {
    SDL_Surface *image;
    SDL_Rect position;
} GuideButton;

// Structure pour une surface SDL avec dimensions
typedef struct {
    SDL_Surface *image;
    int w, h;
} SDL_SurfaceWithRect;

// Structure pour le fond d'écran
typedef struct {
    SDL_Surface *img;
    SDL_Rect posimg;
    GuideButton guide;
    SDL_SurfaceWithRect commentJouer;
    int afficherCommentJouer;
} Background;

// Configuration du défilement de la caméra
#define CAMERA_SEUIL_X 350  // Distance par rapport au bord de l'écran avant que la caméra commence à défiler
#define CAMERA_SEUIL_Y 250  // Distance par rapport au bord de l'écran avant que la caméra commence à défiler verticalement

// Fonctions pour le personnage
void initPerso(Personne *p);
void initPerso2(Personne *p);  // Nouveau pour le deuxième joueur
void afficherPerso(Personne p, SDL_Surface *screen);
void deplacerPerso(Personne *p, Uint32 dt);
void animerPerso(Personne *p);
void saut_Personnage(Personne *P, Uint32 dt, int posx_absolu, int posy_absolu);
void saut_Personnage_Split(Personne *P, int *bgX, Uint32 dt, int posx_absolu, int posy_absolu, int screenWidth);

// Fonctions pour le fond et les plateformes
void initBackgroundAndPlatforms(Background *bg, Platform platforms[], int *taille);
void afficherPlatforms(Platform platforms[], int taille, SDL_Surface *screen, int bgX, int bgY);
void afficherBackground(SDL_Surface *screen, Background *bg, int bgX, int bgY);

// Fonctions pour les ennemis
void initEnnemi(Ennemi *e, const char* imagePath);
void afficherEnnemi(Ennemi e, SDL_Surface *screen, int bgX, int bgY);
void animerEnnemi(Ennemi *e);
void deplacerEnnemi(Ennemi *e);
int collisionEnnemi(Personne p, Ennemi e);

// Fonctions pour les bonus
void initCoeurs(Coeur coeurs[], const char* imagePath);
void afficherCoeurs(Coeur coeurs[], SDL_Surface *screen, int bgX, int bgY);
int collisionCoeur(Personne p, Coeur coeur);

// Fonction pour gérer les événements
void handleEvents(SDL_Event event, Personne *perso, int *running, int *bgX, int *bgY, int sol_y);
void handleEventsDuo(SDL_Event event, Personne *perso1, Personne *perso2, int *running, int *bgX1, int *bgY1, int *bgX2, int *bgY2, int sol_y);

// Fonctions pour le mode duo (écran partagé)
void jouerModeSolo(SDL_Surface *screen);
void jouerModeDuo(SDL_Surface *screen);

// Fonctions pour le menu des paramètres
void initSettingsMenu(SettingsMenu *menu);
void cleanupSettingsMenu(SettingsMenu *menu);
void handleMouseOverSettings(SDL_Rect button_rect, SDL_Surface *button_normal, SDL_Surface *button_hovered, SDL_Surface **current_button);
void increaseVolume(SettingsMenu *menu);
void decreaseVolume(SettingsMenu *menu);
void toggleFullscreen(SDL_Surface **screen, SettingsMenu *menu);
void settingsMenuLoop(SDL_Surface **screen, SettingsMenu *menu);
SDL_Surface* drawSettingsText(const char *text, TTF_Font *font, SDL_Color textColor);

// Fonctions pour la gestion des scores
void initScoreMenu(ScoreMenu *menu);
void cleanupScoreMenu(ScoreMenu *menu);
void handleScoreMenuEvents(SDL_Event event, ScoreMenu *menu, ScoreMenuState *state, int *quit, int score);
void displayScoreMenu(SDL_Surface *screen, ScoreMenu *menu, ScoreMenuState state, int score);
void saveScore(const char *player_name, int score);
void displayScores(SDL_Surface *screen, ScoreMenu *menu);
void incrementScore(Personne *p, Uint32 dt);
int scoreMenuLoop(SDL_Surface *screen, int final_score);

// Fonctions pour l'énigme
void InitEnigme(enigmetf *e, char nomfichier[]);
void afficherEnigme(enigmetf e, SDL_Surface *ecran);
void displayClock(enigmetf e, SDL_Surface *ecran);
void verify_enigme(enigmetf e, SDL_Surface *ecran);
void animer(enigmetf *e);
void free_Surface_enigme(enigmetf *e);
void animer_background(enigmetf *e);
int jouerEnigme(SDL_Surface *ecran); // Fonction principale qui retourne 1 si gagné, 0 si perdu

#endif