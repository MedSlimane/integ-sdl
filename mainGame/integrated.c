#include "integrated.h"
#include <stdio.h>
#include <stdlib.h>

// Initialisation du personnage
void initPerso(Personne *p) {
    p->imagePers = IMG_Load("img/spritesheet.png");
    if (!p->imagePers) {
        printf("Erreur de chargement du spritesheet: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    p->position.x = 100;
    p->position.y = 700;  // Position initiale sur le sol
    p->posX_absolue = 100;
    p->posY_absolue = 700;

    p->image.w = 100;     // Largeur d'une frame dans le spritesheet
    p->image.h = 100;     // Hauteur d'une frame dans le spritesheet
    p->image.x = 0;
    p->image.y = 0;

    p->nbimage = 0;       // Première frame de l'animation
    p->direction = 0;     // Pas de direction initiale
    p->lastDirection = 1; // Direction par défaut (droite)

    // Paramètres de physique pour le saut
    p->vx_saut = 4;       // Vitesse horizontale pendant le saut
    p->v_grav = 0.5;      // Gravité
    p->v_saut = -6;       // Vitesse verticale initiale du saut (négative = vers le haut)
    p->v_y = p->v_saut;   // Vitesse verticale courante

    p->acceleration = 0;
    p->vitesse = 5;       // Vitesse de déplacement horizontale

    // Initialisation des coeurs (vies)
    p->imagec = IMG_Load("img/coeur.png");
    if (!p->imagec) {
        printf("Erreur de chargement de l'image du coeur: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    p->positionc.x = 545;
    p->positionc.y = 0;
    p->nbcoeur = 3;
    p->up = 0;           // Pas en train de sauter
    p->controls = 0;     // Contrôles par défaut: flèches

    // Initialisation des textes
    TTF_Init();
    p->score.font = TTF_OpenFont("img/arial.ttf", 20);
    if (!p->score.font) {
        printf("Erreur de chargement de la police: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    p->score.color = (SDL_Color){255, 255, 255};
    p->score.text = TTF_RenderText_Blended(p->score.font, "Score: 0", p->score.color);
    p->vie.text = TTF_RenderText_Blended(p->score.font, "Vies: ", p->score.color);
    
    p->vie.postext.x = 300;
    p->vie.postext.y = 0;
    p->score.postext.x = 100;
    p->score.postext.y = 0;

    p->Vscore = 0;
}

// Initialisation du second personnage pour le mode duo
void initPerso2(Personne *p) {
    p->imagePers = IMG_Load("img/spritesheet.png");
    if (!p->imagePers) {
        printf("Erreur de chargement du spritesheet: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    p->position.x = 900;  // Position initiale ajustée pour l'écran droit
    p->position.y = 700;  // Position initiale sur le sol
    p->posX_absolue = 900;
    p->posY_absolue = 700;

    p->image.w = 100;     // Largeur d'une frame dans le spritesheet
    p->image.h = 100;     // Hauteur d'une frame dans le spritesheet
    p->image.x = 0;
    p->image.y = 0;

    p->nbimage = 0;       // Première frame de l'animation
    p->direction = 0;     // Pas de direction initiale
    p->lastDirection = 1; // Direction par défaut (droite)

    // Paramètres de physique pour le saut
    p->vx_saut = 4;       // Vitesse horizontale pendant le saut
    p->v_grav = 0.5;      // Gravité
    p->v_saut = -6;       // Vitesse verticale initiale du saut (négative = vers le haut)
    p->v_y = p->v_saut;   // Vitesse verticale courante

    p->acceleration = 0;
    p->vitesse = 5;       // Vitesse de déplacement horizontale

    // Initialisation des coeurs (vies)
    p->imagec = IMG_Load("img/coeur.png");
    if (!p->imagec) {
        printf("Erreur de chargement de l'image du coeur: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    p->positionc.x = 1700;
    p->positionc.y = 0;
    p->nbcoeur = 3;
    p->up = 0;            // Pas en train de sauter
    p->controls = 1;      // Contrôles ZQSD pour le joueur 2

    // Initialisation des textes
    TTF_Init();
    p->score.font = TTF_OpenFont("img/arial.ttf", 20);
    if (!p->score.font) {
        printf("Erreur de chargement de la police: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    p->score.color = (SDL_Color){255, 255, 255};
    p->score.text = TTF_RenderText_Blended(p->score.font, "Score J2: 0", p->score.color);
    p->vie.text = TTF_RenderText_Blended(p->score.font, "Vies J2: ", p->score.color);
    
    p->vie.postext.x = 1500;
    p->vie.postext.y = 0;
    p->score.postext.x = 1300;
    p->score.postext.y = 0;

    p->Vscore = 0;
}

// Affichage du personnage
void afficherPerso(Personne p, SDL_Surface *screen) {
    // Affichage du score
    char vsc[30];
    sprintf(vsc, "Score: %d", p.Vscore);

    p.score.text = TTF_RenderText_Blended(p.score.font, vsc, p.score.color);
    SDL_BlitSurface(p.score.text, NULL, screen, &p.score.postext);
    SDL_BlitSurface(p.vie.text, NULL, screen, &p.vie.postext);

    // Affichage du personnage
    SDL_BlitSurface(p.imagePers, &(p.image), screen, &(p.position));

    // Affichage des coeurs (vies)
    SDL_Rect pos = p.positionc;
    for (int i = 0; i < p.nbcoeur; i++) {
        SDL_BlitSurface(p.imagec, NULL, screen, &pos);
        pos.x -= 60; // Espacement entre les coeurs
    }
}

// Déplacement du personnage
void deplacerPerso(Personne *p, Uint32 dt) {
    float deplacement = 0.5 * p->acceleration * (dt) * (dt) + p->vitesse * dt;
    
    switch (p->direction) {
        case 1: // Droite
            p->position.x += deplacement;
            p->posX_absolue += deplacement;
            break;
        case 2: // Gauche
            p->position.x -= deplacement;
            p->posX_absolue -= deplacement;
            if (p->posX_absolue < 0) {
                p->posX_absolue = 0;
                p->position.x = p->posX_absolue;
            }
            break;
        case 3: // Haut/Saut
            p->position.y -= 3;
            p->posY_absolue -= 3;
            break;
    }
}

// Animation du personnage selon sa direction
void animerPerso(Personne *p) {
    switch(p->direction) {
        case 1: // Animation marche vers la droite
            p->image.y = 0;
            p->nbimage++;
            if (p->nbimage >= 6) {
                p->nbimage = 0;
            }
            p->image.x = p->nbimage * p->image.w;
            break;
        case 2: // Animation marche vers la gauche
            p->image.y = 100;
            p->nbimage++;
            if (p->nbimage >= 6) {
                p->nbimage = 0;
            }
            p->image.x = p->nbimage * p->image.w;
            break;
        case 3: // Animation attaque droite
            p->image.y = 200;
            p->nbimage++;
            if (p->nbimage >= 6) {
                p->nbimage = 0;
            }
            p->image.x = p->nbimage * p->image.w;
            break;
        case 4: // Animation attaque gauche
            p->image.y = 300;
            p->nbimage++;
            if (p->nbimage >= 6) {
                p->nbimage = 0;
            }
            p->image.x = p->nbimage * p->image.w;
            break;
        case 0: // Immobile
            p->image.x = 0;
            break;
    }
}

// Gestion du saut parabolique avec mouvement horizontal amélioré
void saut_Personnage(Personne *p, Uint32 dt, int posx_absolu, int posy_absolu) {
    if (p->up == 1) {  // Si le personnage est en train de sauter
        // Déplacement horizontal pendant le saut selon la direction
        // On augmente la vitesse horizontale pour un meilleur mouvement
        float vx = p->vx_saut * 1.5;  // Multiplication par 1.5 pour un mouvement horizontal plus prononcé
        
        if (p->direction == 1) {  // Droite
            p->position.x += vx;
            p->posX_absolue += vx;
        } else if (p->direction == 2) {  // Gauche
            p->position.x -= vx;
            p->posX_absolue -= vx;
            // Empêcher le personnage de sortir des limites gauches
            if (p->posX_absolue < 0) {
                p->posX_absolue = 0;
                p->position.x = p->posX_absolue;
            }
        } 
        // Même si aucune touche n'est pressée, on conserve la direction horizontale du saut
        else if (p->v_y < 0) {  // Si on monte encore (début du saut)
            // On continue dans la dernière direction horizontale si on est au début du saut
            if (p->lastDirection == 1) {  // Droite
                p->position.x += vx * 0.7;  // Vitesse réduite pour un effet d'inertie
                p->posX_absolue += vx * 0.7;
            } else if (p->lastDirection == 2) {  // Gauche
                p->position.x -= vx * 0.7;
                p->posX_absolue -= vx * 0.7;
                if (p->posX_absolue < 0) {
                    p->posX_absolue = 0;
                    p->position.x = p->posX_absolue;
                }
            }
        }
        
        // Mise à jour de la position verticale avec la gravité (mouvement parabolique)
        p->position.y += p->v_y;
        p->posY_absolue += p->v_y;
        p->v_y += p->v_grav;
        
        // Vérification si le personnage a atteint le sol
        if (p->position.y >= posy_absolu) {
            p->position.y = posy_absolu;  // Replace le personnage sur le sol
            p->posY_absolue = posy_absolu;  // Mettre à jour la position absolue également
            p->v_y = p->v_saut;          // Réinitialise la vitesse verticale
            p->up = 0;                   // Désactive l'état de saut
        }
        
        // Animation du personnage pendant le saut
        animerPerso(p);
    } 
    // Si on n'est pas en train de sauter, on mémorise la direction actuelle pour le prochain saut
    else if (p->direction == 1 || p->direction == 2) {
        p->lastDirection = p->direction;
    }
}

// Gestion du saut avec défilement d'écran
void saut_Personnage_Split(Personne *p, int *bgX, Uint32 dt, int posx_absolu, int posy_absolu, int screenWidth) {
    if (p->up == 1) {
        // Déplacement horizontal amélioré pendant le saut
        float vx = p->vx_saut * 1.5;
        
        if (p->direction == 1) { // Droite
            p->position.x += vx;
            p->posX_absolue += vx;
            
            // Gestion défilement
            if (p->position.x > (screenWidth - CAMERA_SEUIL_X)) {
                int decalage = p->position.x - (screenWidth - CAMERA_SEUIL_X);
                *bgX += decalage;
                p->position.x = screenWidth - CAMERA_SEUIL_X;
            }
        } else if (p->direction == 2) { // Gauche
            p->position.x -= vx;
            p->posX_absolue -= vx;
            
            // Gestion défilement et limites
            if (p->position.x < CAMERA_SEUIL_X && *bgX > 0) {
                int decalage = CAMERA_SEUIL_X - p->position.x;
                *bgX -= decalage;
                p->position.x = CAMERA_SEUIL_X;
                if (*bgX < 0) {
                    p->position.x += *bgX;
                    *bgX = 0;
                }
            }
            
            if (p->posX_absolue < 0) {
                p->posX_absolue = 0;
                p->position.x = p->posX_absolue;
            }
        } else if (p->v_y < 0) { // Inertie au début du saut
            if (p->lastDirection == 1) {
                p->position.x += vx * 0.7;
                p->posX_absolue += vx * 0.7;
                
                // Gestion défilement
                if (p->position.x > (screenWidth - CAMERA_SEUIL_X)) {
                    *bgX += (p->position.x - (screenWidth - CAMERA_SEUIL_X));
                    p->position.x = screenWidth - CAMERA_SEUIL_X;
                }
            } else if (p->lastDirection == 2) {
                p->position.x -= vx * 0.7;
                p->posX_absolue -= vx * 0.7;
                
                // Gestion défilement et limites
                if (p->position.x < CAMERA_SEUIL_X && *bgX > 0) {
                    *bgX -= (CAMERA_SEUIL_X - p->position.x);
                    p->position.x = CAMERA_SEUIL_X;
                    if (*bgX < 0) {
                        p->position.x += *bgX;
                        *bgX = 0;
                    }
                }
                
                if (p->posX_absolue < 0) {
                    p->posX_absolue = 0;
                    p->position.x = p->posX_absolue;
                }
            }
        }
        
        // Mise à jour position verticale
        p->position.y += p->v_y;
        p->posY_absolue += p->v_y;
        p->v_y += p->v_grav;
        
        // Vérif sol
        if (p->position.y >= posy_absolu) {
            p->position.y = posy_absolu;
            p->posY_absolue = posy_absolu;
            p->v_y = p->v_saut;
            p->up = 0;
        }
        
        animerPerso(p);
    } else if (p->direction == 1 || p->direction == 2) {
        p->lastDirection = p->direction;
    }
}

// Initialisation du fond d'écran et des plateformes
void initBackgroundAndPlatforms(Background *bg, Platform platforms[], int *taille) {
    // Initialisation du fond d'écran
    bg->posimg.x = 0;
    bg->posimg.y = 0;
    bg->img = IMG_Load("img/bggggggggg.png");
    if (!bg->img) {
        printf("Erreur chargement image fond: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Chargement des images pour les plateformes et obstacles
    SDL_Surface *terre = IMG_Load("img/background_terre1.bmp");
    SDL_Surface *fireSprite1 = IMG_Load("img/fire1.png");
    SDL_Surface *fireSprite2 = IMG_Load("img/fire2.png");
    SDL_Surface *fireSprite3 = IMG_Load("img/fire3.png");
    SDL_Surface *obstacleImage1 = IMG_Load("img/obstacle1.png");
    SDL_Surface *obstacleImage2 = IMG_Load("img/obstacle2.png");

    if (!terre || !fireSprite1 || !fireSprite2 || !fireSprite3 || 
        !obstacleImage1 || !obstacleImage2) {
        printf("Erreur chargement des images: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Initialisation des plateformes de terrain
    for (int i = 0; i < 10; i++) {
        platforms[i].image = terre;
        platforms[i].position.x = i * terre->w;
        platforms[i].position.y = 900;
        platforms[i].vitesse = 0;
        platforms[i].isAnimated = 0;
    }

    // Initialisation des éléments animés (feu)
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
    
    // Initialisation des obstacles
    platforms[11].image = obstacleImage1;
    platforms[11].position.x = 800;
    platforms[11].position.y = 750;
    platforms[11].vitesse = 0;
    platforms[11].isAnimated = 0;
    platforms[11].currentFrame = 0;
    platforms[11].frameWidth = obstacleImage1->w;
    platforms[11].frameHeight = obstacleImage1->h;

    platforms[12].image = obstacleImage2;
    platforms[12].position.x = 1500;
    platforms[12].position.y = 750;
    platforms[12].vitesse = 0;
    platforms[12].isAnimated = 0;
    platforms[12].currentFrame = 0;
    platforms[12].frameWidth = obstacleImage2->w;
    platforms[12].frameHeight = obstacleImage2->h;

    *taille = 13; // Nombre total de plateformes et obstacles
}

// Affichage des plateformes
void afficherPlatforms(Platform platforms[], int taille, SDL_Surface *screen, int bgX, int bgY) {
    Uint32 currentTime = SDL_GetTicks();
    
    for (int i = 0; i < taille; i++) {
        SDL_Rect dest = {platforms[i].position.x - bgX, platforms[i].position.y - bgY};

        // Si l'élément est animé (comme le feu)
        if (platforms[i].isAnimated) {
            // Mise à jour de la frame d'animation toutes les 150ms
            if (currentTime - platforms[i].lastFrameTime > 150) {
                platforms[i].currentFrame = (platforms[i].currentFrame + 1) % 3; // 3 frames
                platforms[i].lastFrameTime = currentTime;
            }

            SDL_Rect src = {0, 0, platforms[i].frameWidth, platforms[i].frameHeight};
            SDL_BlitSurface(platforms[i].frames[platforms[i].currentFrame], &src, screen, &dest);
        } else {
            // Éléments statiques
            SDL_BlitSurface(platforms[i].image, NULL, screen, &dest);
        }
    }
}

// Affichage du fond d'écran avec effet parallaxe
void afficherBackground(SDL_Surface *screen, Background *bg, int bgX, int bgY) {
    int bgWidth = bg->img->w;
    // Normalisation de la position pour l'effet de défilement infini
    int normalizedX = bgX % bgWidth;
    if (normalizedX < 0) normalizedX += bgWidth;

    // Affichage de deux instances du fond côte à côte pour l'effet de défilement
    SDL_Rect dest1 = {-normalizedX, -bgY};
    SDL_Rect dest2 = {-normalizedX + bgWidth, -bgY};
    SDL_BlitSurface(bg->img, NULL, screen, &dest1);
    SDL_BlitSurface(bg->img, NULL, screen, &dest2);
}

// Gestion des événements (clavier, souris, etc.)
void handleEvents(SDL_Event event, Personne *perso, int *running, int *bgX, int *bgY, int sol_y) {
    if (event.type == SDL_QUIT) {
        *running = 0;
        return;
    }

    // Gestion des touches clavier
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_RIGHT:
                perso->direction = 1; // Droite
                break;
            case SDLK_LEFT:
                perso->direction = 2; // Gauche
                break;
            case SDLK_SPACE:
                // Sauter uniquement si on n'est pas déjà en train de sauter
                if (perso->position.y >= sol_y && perso->up == 0) {
                    perso->up = 1; // Active le saut
                }
                break;
            case SDLK_ESCAPE:
                *running = 0; // Quitter le jeu
                break;
            default:
                break;
        }
    } 
    else if (event.type == SDL_KEYUP) {
        // Arrêter le mouvement quand les touches sont relâchées
        if ((event.key.keysym.sym == SDLK_RIGHT && perso->direction == 1) ||
            (event.key.keysym.sym == SDLK_LEFT && perso->direction == 2)) {
            perso->direction = 0;
        }
    }
}

// Gestion des événements pour le mode duo (écran partagé)
void handleEventsDuo(SDL_Event event, Personne *perso1, Personne *perso2, int *running, int *bgX1, int *bgY1, int *bgX2, int *bgY2, int sol_y) {
    if (event.type == SDL_QUIT) {
        *running = 0;
        return;
    }

    // Gestion des touches clavier
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            // Joueur 1 (Flèches)
            case SDLK_RIGHT:
                perso1->direction = 1; // Droite
                break;
            case SDLK_LEFT:
                perso1->direction = 2; // Gauche
                break;
            case SDLK_UP:
                // Sauter uniquement si on n'est pas déjà en train de sauter
                if (perso1->position.y >= sol_y && perso1->up == 0) {
                    perso1->up = 1; // Active le saut
                }
                break;

            // Joueur 2 (ZQSD)
            case SDLK_d:
                perso2->direction = 1; // Droite
                break;
            case SDLK_q:
                perso2->direction = 2; // Gauche
                break;
            case SDLK_z:
                // Sauter uniquement si on n'est pas déjà en train de sauter
                if (perso2->position.y >= sol_y && perso2->up == 0) {
                    perso2->up = 1; // Active le saut
                }
                break;

            case SDLK_ESCAPE:
                *running = 0; // Quitter le jeu
                break;
            default:
                break;
        }
    } 
    else if (event.type == SDL_KEYUP) {
        // Arrêter le mouvement quand les touches sont relâchées
        switch (event.key.keysym.sym) {
            case SDLK_RIGHT:
                if (perso1->direction == 1) perso1->direction = 0;
                break;
            case SDLK_LEFT:
                if (perso1->direction == 2) perso1->direction = 0;
                break;
            case SDLK_d:
                if (perso2->direction == 1) perso2->direction = 0;
                break;
            case SDLK_q:
                if (perso2->direction == 2) perso2->direction = 0;
                break;
            default:
                break;
        }
    }
}

// Fonctions pour le mode solo et duo
void jouerModeSolo(SDL_Surface *screen) {
    // Variables pour le jeu
    Personne perso;
    Background background;
    Platform platforms[20]; // Tableau pour stocker les plateformes et obstacles
    int taillePlatforms;
    
    // Variables pour les ennemis
    Ennemi ennemis[1]; // Tableau pour stocker un seul ennemi
    int nbEnnemis = 1; // Un seul ennemi à afficher
    
    // Variables pour les objets bonus (coeurs)
    Coeur coeurs[8];
    
    // Variables pour le temps
    Uint32 lastTime = SDL_GetTicks();
    Uint32 currentTime;
    Uint32 deltaTime;
    int timeLeft = 120; // 2 minutes de jeu
    
    // Variables pour le défilement du fond
    int bgX = 0, bgY = 0;
    
    // Initialisation du fond d'écran et des plateformes
    initBackgroundAndPlatforms(&background, platforms, &taillePlatforms);
    
    // Initialisation du personnage
    initPerso(&perso);
    
    // Initialisation d'un seul ennemi
    initEnnemi(&ennemis[0], "ES2");
    ennemis[0].pos.x = 500;
    ennemis[0].pos.y = 600;
    
    // Initialisation des objets bonus (coeurs)
    initCoeurs(coeurs, "ES2/bonus");
    
    // Boucle principale du jeu
    int running = 1;
    SDL_Event event;
    
    // Position Y du sol pour le personnage
    int sol_y = 700;
    
    while (running) {
        // Calcul du temps écoulé
        currentTime = SDL_GetTicks();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        
        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            handleEvents(event, &perso, &running, &bgX, &bgY, sol_y);
        }
        
        // Déplacement et animation du personnage
        if (perso.direction != 0) {
            deplacerPerso(&perso, deltaTime);
            animerPerso(&perso);
            
            // Logique du seuil pour le défilement de la caméra
            if (perso.position.x > (screen->w - CAMERA_SEUIL_X)) {
                bgX += perso.position.x - (screen->w - CAMERA_SEUIL_X);
                perso.position.x = screen->w - CAMERA_SEUIL_X;
            }
            else if (perso.position.x < CAMERA_SEUIL_X && bgX > 0) {
                bgX -= (CAMERA_SEUIL_X - perso.position.x);
                perso.position.x = CAMERA_SEUIL_X;
                if (bgX < 0) {
                    perso.position.x += bgX;
                    bgX = 0;
                }
            }
        }
        
        // Gestion du saut du personnage
        saut_Personnage(&perso, deltaTime, perso.posX_absolue, sol_y);
        
        // Mise à jour des ennemis
        for (int i = 0; i < nbEnnemis; i++) {
            if (ennemis[i].active) {
                deplacerEnnemi(&ennemis[i]);
                animerEnnemi(&ennemis[i]);
                
                // Vérifier collision avec le personnage
                if (collisionEnnemi(perso, ennemis[i])) {
                    // Perte de vie si collision avec un ennemi
                    if (perso.nbcoeur > 0) {
                        perso.nbcoeur--;
                        // Désactiver l'ennemi temporairement pour éviter des collisions multiples
                        ennemis[i].active = 0;
                        // Utiliser lastFrameTime comme indicateur de quand réactiver
                        ennemis[i].lastFrameTime = SDL_GetTicks() + 2000; // Réactiver dans 2 secondes
                    }
                }
            } else {
                // Réactiver l'ennemi après le délai
                if (SDL_GetTicks() > ennemis[i].lastFrameTime) {
                    ennemis[i].active = 1;
                }
            }
        }
        
        // Vérifier les collisions avec les objets bonus (coeurs)
        for (int i = 0; i < 8; i++) {
            if (coeurs[i].visible && collisionCoeur(perso, coeurs[i])) {
                coeurs[i].visible = 0;
                perso.Vscore += 10; // Augmenter le score
            }
        }
        
        // Effacement de l'écran
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        
        // Affichage du fond d'écran avec effet de parallaxe
        afficherBackground(screen, &background, bgX, bgY);
        
        // Affichage des plateformes et obstacles
        afficherPlatforms(platforms, taillePlatforms, screen, bgX, bgY);
        
        // Affichage des ennemis
        for (int i = 0; i < nbEnnemis; i++) {
            afficherEnnemi(ennemis[i], screen, bgX, bgY);
        }
        
        // Affichage des objets bonus (coeurs)
        afficherCoeurs(coeurs, screen, bgX, bgY);
        
        // Affichage du personnage
        afficherPerso(perso, screen);
        
        // Mise à jour de l'affichage
        SDL_Flip(screen);
        
        // Limiter à ~60 FPS
        if (deltaTime < 16) {
            SDL_Delay(16 - deltaTime);
        }
    }
    
    // Libération des ressources
    SDL_FreeSurface(background.img);
    SDL_FreeSurface(perso.imagePers);
    SDL_FreeSurface(perso.imagec);
    SDL_FreeSurface(perso.score.text);
    SDL_FreeSurface(perso.vie.text);
    
    // Libérer les ressources des ennemis
    for (int i = 0; i < nbEnnemis; i++) {
        for (int j = 1; j <= 17; j++) {
            SDL_FreeSurface(ennemis[i].enem[j]);
        }
    }
    
    // Libérer les ressources des objets bonus
    for (int i = 0; i < 8; i++) {
        SDL_FreeSurface(coeurs[i].image);
    }
    
    TTF_CloseFont(perso.score.font);
}

void jouerModeDuo(SDL_Surface *screen) {
    // Variables pour le jeu
    Personne perso1, perso2;
    Background background1, background2;
    Platform platforms1[20], platforms2[20]; // Tableaux pour stocker les plateformes et obstacles
    int taillePlatforms1, taillePlatforms2;
    
    // Variables pour les ennemis
    Ennemi ennemis1[1], ennemis2[1]; // Un seul ennemi pour chaque joueur
    int nbEnnemis = 1; // Un seul ennemi par écran
    
    // Variables pour les objets bonus (coeurs)
    Coeur coeurs1[8], coeurs2[8]; // Bonus pour chaque joueur
    
    // Variables pour le temps
    Uint32 lastTime = SDL_GetTicks();
    Uint32 currentTime;
    Uint32 deltaTime;
    
    // Variables pour le défilement du fond
    int bgX1 = 0, bgY1 = 0, bgX2 = 0, bgY2 = 0;
    
    // Initialisation du fond d'écran et des plateformes pour les deux joueurs
    initBackgroundAndPlatforms(&background1, platforms1, &taillePlatforms1);
    initBackgroundAndPlatforms(&background2, platforms2, &taillePlatforms2);
    
    // Initialisation des personnages
    initPerso(&perso1);
    initPerso2(&perso2);
    
    // Initialisation d'un seul ennemi pour le joueur 1
    initEnnemi(&ennemis1[0], "ES2");
    ennemis1[0].pos.x = 300;
    ennemis1[0].pos.y = 600;
    
    // Initialisation d'un seul ennemi pour le joueur 2
    initEnnemi(&ennemis2[0], "ES2");
    ennemis2[0].pos.x = 400;
    ennemis2[0].pos.y = 600;
    
    // Initialisation des objets bonus pour les deux joueurs
    initCoeurs(coeurs1, "ES2/bonus");
    initCoeurs(coeurs2, "ES2/bonus");
    
    // Boucle principale du jeu
    int running = 1;
    SDL_Event event;

    // Définir la ligne de séparation pour l'écran partagé vertical (gauche/droite)
    SDL_Rect separation = {screen->w / 2, 0, 2, screen->h};
    SDL_Rect ecranGauche = {0, 0, screen->w / 2, screen->h};
    SDL_Rect ecranDroite = {screen->w / 2, 0, screen->w / 2, screen->h};
    
    // Position Y du sol pour les deux joueurs
    int sol_y = 700;
    
    while (running) {
        // Calcul du temps écoulé
        currentTime = SDL_GetTicks();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        
        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            handleEventsDuo(event, &perso1, &perso2, &running, &bgX1, &bgY1, &bgX2, &bgY2, sol_y);
        }
        
        // Déplacement et animation des personnages
        if (perso1.direction != 0) {
            deplacerPerso(&perso1, deltaTime);
            animerPerso(&perso1);
            
            // Logique du seuil pour joueur 1
            // Si le joueur dépasse le seuil à droite, on défile le fond
            if (perso1.position.x > (ecranGauche.w - CAMERA_SEUIL_X)) {
                bgX1 += perso1.position.x - (ecranGauche.w - CAMERA_SEUIL_X);
                perso1.position.x = ecranGauche.w - CAMERA_SEUIL_X;
            }
            // Si le joueur dépasse le seuil à gauche, on défile le fond (si possible)
            else if (perso1.position.x < CAMERA_SEUIL_X && bgX1 > 0) {
                bgX1 -= (CAMERA_SEUIL_X - perso1.position.x);
                perso1.position.x = CAMERA_SEUIL_X;
                if (bgX1 < 0) {
                    perso1.position.x += bgX1;
                    bgX1 = 0;
                }
            }
        }
        
        if (perso2.direction != 0) {
            deplacerPerso(&perso2, deltaTime);
            animerPerso(&perso2);
            
            // Logique du seuil pour joueur 2
            // Si le joueur dépasse le seuil à droite, on défile le fond
            if (perso2.position.x > (ecranDroite.w - CAMERA_SEUIL_X)) {
                bgX2 += perso2.position.x - (ecranDroite.w - CAMERA_SEUIL_X);
                perso2.position.x = ecranDroite.w - CAMERA_SEUIL_X;
            }
            // Si le joueur dépasse le seuil à gauche, on défile le fond (si possible)
            else if (perso2.position.x < CAMERA_SEUIL_X && bgX2 > 0) {
                bgX2 -= (CAMERA_SEUIL_X - perso2.position.x);
                perso2.position.x = CAMERA_SEUIL_X;
                if (bgX2 < 0) {
                    perso2.position.x += bgX2;
                    bgX2 = 0;
                }
            }
        }
        
        // Gestion du saut des personnages
        saut_Personnage_Split(&perso1, &bgX1, deltaTime, perso1.posX_absolue, sol_y, ecranGauche.w);
        saut_Personnage_Split(&perso2, &bgX2, deltaTime, perso2.posX_absolue, sol_y, ecranDroite.w);
        
        // Mise à jour des ennemis pour le joueur 1
        for (int i = 0; i < nbEnnemis; i++) {
            if (ennemis1[i].active) {
                deplacerEnnemi(&ennemis1[i]);
                animerEnnemi(&ennemis1[i]);
                
                // Vérifier collision avec le personnage 1
                if (collisionEnnemi(perso1, ennemis1[i])) {
                    // Perte de vie si collision avec un ennemi
                    if (perso1.nbcoeur > 0) {
                        perso1.nbcoeur--;
                        // Désactiver l'ennemi temporairement pour éviter des collisions multiples
                        ennemis1[i].active = 0;
                        // Réactiver l'ennemi après 2 secondes (utilisation d'un timer simplifiée)
                        ennemis1[i].lastFrameTime = SDL_GetTicks() + 2000; // On utilisera lastFrameTime comme indicateur de temps
                    }
                }
            } else {
                // Réactiver l'ennemi après le délai
                if (SDL_GetTicks() > ennemis1[i].lastFrameTime) {
                    ennemis1[i].active = 1;
                }
            }
        }
        
        // Mise à jour des ennemis pour le joueur 2
        for (int i = 0; i < nbEnnemis; i++) {
            if (ennemis2[i].active) {
                deplacerEnnemi(&ennemis2[i]);
                animerEnnemi(&ennemis2[i]);
                
                // Vérifier collision avec le personnage 2
                if (collisionEnnemi(perso2, ennemis2[i])) {
                    // Perte de vie si collision avec un ennemi
                    if (perso2.nbcoeur > 0) {
                        perso2.nbcoeur--;
                        // Désactiver l'ennemi temporairement pour éviter des collisions multiples
                        ennemis2[i].active = 0;
                        // Réactiver l'ennemi après 2 secondes
                        ennemis2[i].lastFrameTime = SDL_GetTicks() + 2000;
                    }
                }
            } else {
                // Réactiver l'ennemi après le délai
                if (SDL_GetTicks() > ennemis2[i].lastFrameTime) {
                    ennemis2[i].active = 1;
                }
            }
        }
        
        // Vérifier les collisions avec les objets bonus (coeurs) - Joueur 1
        for (int i = 0; i < 8; i++) {
            if (coeurs1[i].visible && collisionCoeur(perso1, coeurs1[i])) {
                coeurs1[i].visible = 0;
                perso1.Vscore += 10; // Augmenter le score
            }
        }
        
        // Vérifier les collisions avec les objets bonus (coeurs) - Joueur 2
        for (int i = 0; i < 8; i++) {
            if (coeurs2[i].visible && collisionCoeur(perso2, coeurs2[i])) {
                coeurs2[i].visible = 0;
                perso2.Vscore += 10; // Augmenter le score
            }
        }
        
        // Effacement de l'écran
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        
        // Affichage de l'écran du joueur 1 (côté gauche)
        SDL_SetClipRect(screen, &ecranGauche);
        
        // Affichage du fond d'écran avec effet de parallaxe pour joueur 1
        afficherBackground(screen, &background1, bgX1, bgY1);
        
        // Affichage des plateformes et obstacles pour joueur 1
        afficherPlatforms(platforms1, taillePlatforms1, screen, bgX1, bgY1);
        
        // Affichage des ennemis pour joueur 1
        for (int i = 0; i < nbEnnemis; i++) {
            afficherEnnemi(ennemis1[i], screen, bgX1, bgY1);
        }
        
        // Affichage des objets bonus pour joueur 1
        afficherCoeurs(coeurs1, screen, bgX1, bgY1);
        
        // Affichage du joueur 1
        // Ajuster les coordonnées du texte pour l'écran gauche
        int scoreX = perso1.score.postext.x;
        int vieX = perso1.vie.postext.x;
        int coeurX = perso1.positionc.x;
        
        perso1.score.postext.x = 20;  // Position du score ajustée pour l'écran gauche
        perso1.vie.postext.x = 200;   // Position du texte de vie ajustée pour l'écran gauche
        perso1.positionc.x = 350;     // Position des cœurs ajustée pour l'écran gauche
        
        afficherPerso(perso1, screen);
        
        // Restaurer les coordonnées originales
        perso1.score.postext.x = scoreX;
        perso1.vie.postext.x = vieX;
        perso1.positionc.x = coeurX;
        
        // Affichage de l'écran du joueur 2 (côté droit)
        SDL_SetClipRect(screen, &ecranDroite);
        
        // Affichage du fond d'écran avec effet de parallaxe pour joueur 2
        afficherBackground(screen, &background2, bgX2, bgY2);
        
        // Affichage des plateformes et obstacles pour joueur 2
        afficherPlatforms(platforms2, taillePlatforms2, screen, bgX2, bgY2);
        
        // Affichage des ennemis pour joueur 2
        for (int i = 0; i < nbEnnemis; i++) {
            afficherEnnemi(ennemis2[i], screen, bgX2, bgY2);
        }
        
        // Affichage des objets bonus pour joueur 2
        afficherCoeurs(coeurs2, screen, bgX2, bgY2);
        
        // Affichage du joueur 2
        // Ajuster les coordonnées du texte pour l'écran droit
        scoreX = perso2.score.postext.x;
        vieX = perso2.vie.postext.x;
        coeurX = perso2.positionc.x;
        
        perso2.score.postext.x = screen->w / 2 + 20;   // Position du score ajustée pour l'écran droit
        perso2.vie.postext.x = screen->w / 2 + 200;    // Position du texte de vie ajustée pour l'écran droit
        perso2.positionc.x = screen->w / 2 + 350;      // Position des cœurs ajustée pour l'écran droit
        
        // Ajuster la position d'affichage pour l'écran droit
        // Ceci est la correction principale
        SDL_Rect positionJ2 = perso2.position;
        positionJ2.x = positionJ2.x + screen->w / 2;
        
        // Utiliser la position ajustée pour l'affichage
        SDL_BlitSurface(perso2.imagePers, &(perso2.image), screen, &positionJ2);
        
        // Afficher le reste des informations du joueur 2
        SDL_BlitSurface(perso2.score.text, NULL, screen, &(perso2.score.postext));
        SDL_BlitSurface(perso2.vie.text, NULL, screen, &(perso2.vie.postext));
        
        // Afficher les cœurs du joueur 2
        SDL_Rect pos = perso2.positionc;
        for (int i = 0; i < perso2.nbcoeur; i++) {
            SDL_BlitSurface(perso2.imagec, NULL, screen, &pos);
            pos.x -= 60; // Espacement entre les coeurs
        }
        
        // Restaurer les coordonnées originales
        perso2.score.postext.x = scoreX;
        perso2.vie.postext.x = vieX;
        perso2.positionc.x = coeurX;
        
        // Réinitialiser le viewport pour dessiner la séparation
        SDL_SetClipRect(screen, NULL);
        
        // Dessiner la ligne de séparation verticale
        SDL_FillRect(screen, &separation, SDL_MapRGB(screen->format, 255, 255, 255));
        
        // Mise à jour de l'affichage
        SDL_Flip(screen);
        
        // Limiter à ~60 FPS
        if (deltaTime < 16) {
            SDL_Delay(16 - deltaTime);
        }
    }
    
    // Libération des ressources
    SDL_FreeSurface(background1.img);
    SDL_FreeSurface(background2.img);
    
    // Libération des ennemis
    for (int i = 0; i < nbEnnemis; i++) {
        for (int j = 1; j <= 17; j++) {
            SDL_FreeSurface(ennemis1[i].enem[j]);
            SDL_FreeSurface(ennemis2[i].enem[j]);
        }
    }
    
    // Libération des objets bonus
    for (int i = 0; i < 8; i++) {
        SDL_FreeSurface(coeurs1[i].image);
        SDL_FreeSurface(coeurs2[i].image);
    }
    
    // Libération des ressources des joueurs
    SDL_FreeSurface(perso1.imagePers);
    SDL_FreeSurface(perso1.imagec);
    SDL_FreeSurface(perso1.score.text);
    SDL_FreeSurface(perso1.vie.text);
    SDL_FreeSurface(perso2.imagePers);
    SDL_FreeSurface(perso2.imagec);
    SDL_FreeSurface(perso2.score.text);
    SDL_FreeSurface(perso2.vie.text);
    TTF_CloseFont(perso1.score.font);
    TTF_CloseFont(perso2.score.font);
}

// Initialisation de l'ennemi avec chemin d'image paramétrable
void initEnnemi(Ennemi *e, const char* imagePath) {
    char filename[100];
    // Chargement des images d'animation de l'ennemi
    for (int i = 1; i <= 17; i++) {
        sprintf(filename, "%s/%d.png", imagePath, i);
        e->enem[i] = IMG_Load(filename);
        if (!e->enem[i]) {
            printf("Erreur chargement image ennemi %d: %s\n", i, SDL_GetError());
            exit(EXIT_FAILURE);
        }
    }

    // Position initiale de l'ennemi
    e->pos.x = 300;
    e->pos.y = 300;
    e->pos.w = e->enem[1]->w;
    e->pos.h = e->enem[1]->h;

    e->d = 0;         // Direction initiale (0 = vers la droite)
    e->num = 1;       // Première frame de l'animation
    e->lastFrameTime = SDL_GetTicks();
    e->active = 1;    // L'ennemi est actif par défaut
}

// Affichage de l'ennemi avec prise en compte du défilement de la caméra
void afficherEnnemi(Ennemi e, SDL_Surface *screen, int bgX, int bgY) {
    if (e.active) {
        SDL_Rect dest = {e.pos.x - bgX, e.pos.y - bgY, e.pos.w, e.pos.h};
        SDL_BlitSurface(e.enem[e.num], NULL, screen, &dest);
    }
}

// Animation de l'ennemi avec meilleure gestion du timing
void animerEnnemi(Ennemi *e) {
    // Animation fluide avec limitation de framerate
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - e->lastFrameTime > 100) { // 100ms par frame = 10fps
        e->num++;
        if (e->num > 17) {
            e->num = 1;
        }
        e->lastFrameTime = currentTime;
    }
}

// Déplacement de l'ennemi entre deux bornes
void deplacerEnnemi(Ennemi *e) {
    int posmax = 800;
    int posmin = 300;
    
    // Déplacement entre les deux bornes
    if (e->pos.x > posmax) {
        e->d = 1;
    }
    if (e->pos.x < posmin) {
        e->d = 0;
    }
    
    // Application du mouvement selon la direction
    if (e->d == 0) {
        e->pos.x += 2; // Vitesse réduite pour un mouvement plus fluide
    } else if (e->d == 1) {
        e->pos.x -= 2;
    }
}

// Détection de collision entre le personnage et l'ennemi (méthode des boîtes englobantes)
int collisionEnnemi(Personne p, Ennemi e) {
    // Pas de collision si l'ennemi n'est pas actif
    if (!e.active) return 0;
    
    // Test de collision par boîtes englobantes
    if ((p.position.x + p.image.w < e.pos.x) ||
        (p.position.x > e.pos.x + e.pos.w) ||
        (p.position.y + p.image.h < e.pos.y) ||
        (p.position.y > e.pos.y + e.pos.h)) {
        return 0; // Pas de collision
    } else {
        return 1; // Collision détectée
    }
}

// Initialisation des objets bonus (coeurs)
void initCoeurs(Coeur coeurs[], const char* imagePath) {
    char filename[100];
    
    for (int i = 0; i < 8; i++) {
        sprintf(filename, "%s/%d.png", imagePath, i);
        coeurs[i].image = IMG_Load(filename);
        if (!coeurs[i].image) {
            printf("Erreur chargement image coeur %d: %s\n", i, SDL_GetError());
            exit(EXIT_FAILURE);
        }
        
        coeurs[i].position.x = rand() % 600;
        coeurs[i].position.y = rand() % 500;
        coeurs[i].position.w = coeurs[i].image->w;
        coeurs[i].position.h = coeurs[i].image->h;
        coeurs[i].visible = 1;
    }
}

// Affichage des objets bonus (coeurs) avec prise en compte du défilement
void afficherCoeurs(Coeur coeurs[], SDL_Surface *screen, int bgX, int bgY) {
    for (int i = 0; i < 8; i++) {
        if (coeurs[i].visible) {
            SDL_Rect dest = {coeurs[i].position.x - bgX, coeurs[i].position.y - bgY, 
                             coeurs[i].position.w, coeurs[i].position.h};
            SDL_BlitSurface(coeurs[i].image, NULL, screen, &dest);
        }
    }
}

// Détection de collision entre le personnage et un objet bonus (méthode du cercle)
int collisionCoeur(Personne p, Coeur coeur) {
    if (!coeur.visible) return 0;
    
    // Calcul des centres des cercles
    float centerX1 = p.position.x + p.image.w / 2.0f;
    float centerY1 = p.position.y + p.image.h / 2.0f;
    float R1 = fmax(p.image.w / 2.0f, p.image.h / 2.0f); // Rayon du cercle
    
    float centerX2 = coeur.position.x + coeur.position.w / 2.0f;
    float centerY2 = coeur.position.y + coeur.position.h / 2.0f;
    float R2 = coeur.position.w / 2.0f; // Rayon du cercle
    
    // Calcul de la distance entre les centres
    float dx = centerX2 - centerX1;
    float dy = centerY2 - centerY1;
    float D = sqrt(dx * dx + dy * dy);
    
    // Si la distance est inférieure à la somme des rayons, il y a collision
    if (D <= (R1 + R2)) {
        return 1; // Collision détectée
    } else {
        return 0; // Pas de collision
    }
}