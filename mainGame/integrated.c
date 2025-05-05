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
                // Lancer le menu des scores au lieu de quitter directement
                scoreMenuLoop(SDL_GetVideoSurface(), perso->Vscore);
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
                // Lancer le menu des scores avec le score du joueur ayant le meilleur score
                if (perso1->Vscore > perso2->Vscore) {
                    scoreMenuLoop(SDL_GetVideoSurface(), perso1->Vscore);
                } else {
                    scoreMenuLoop(SDL_GetVideoSurface(), perso2->Vscore);
                }
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
    
    // Variable pour la minicarte
    Minimap minimap;
    
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
    
    // Initialisation de la minicarte
    initMinimap(&minimap, screen->w, screen->h);
    
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
            
            // Incrémenter le score lorsque le joueur se déplace
            incrementScore(&perso, deltaTime);
            
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
        
        // Si le joueur est en train de sauter, incrémenter également le score
        if (perso.up == 1) {
            incrementScore(&perso, deltaTime);
        }
        
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
                        // Ne pas désactiver l'ennemi temporairement lors de la première collision (tant que le joueur a encore des vies)
                        // Modification: garder l'ennemi actif si le joueur a encore des vies après cette collision
                        if (perso.nbcoeur == 0) {
                            // Si le joueur n'a plus de vies, lancer l'énigme
                            // Montrer un message indiquant qu'une énigme va commencer
                            SDL_Surface *messageText;
                            TTF_Font *messageFont = TTF_OpenFont("enigme/arial.ttf", 24);
                            if (!messageFont) {
                                messageFont = TTF_OpenFont("img/arial.ttf", 24);
                            }
                            
                            if (messageFont) {
                                SDL_Color textColor = {255, 255, 255};
                                messageText = TTF_RenderText_Blended(messageFont, "Vous avez perdu toutes vos vies! Résolvez l'énigme pour continuer...", textColor);
                                
                                SDL_Rect messagePosition = {
                                    (screen->w - messageText->w) / 2,
                                    (screen->h - messageText->h) / 2,
                                    0, 0
                                };
                                
                                // Afficher le fond et le message
                                SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
                                SDL_BlitSurface(messageText, NULL, screen, &messagePosition);
                                SDL_Flip(screen);
                                SDL_Delay(3000);
                                
                                // Libérer les ressources
                                SDL_FreeSurface(messageText);
                                TTF_CloseFont(messageFont);
                            }
                            
                            // Sauvegarder la position et l'état du joueur avant l'énigme
                            int savedPosX = perso.position.x;
                            int savedPosY = perso.position.y;
                            int savedPosX_abs = perso.posX_absolue;
                            int savedPosY_abs = perso.posY_absolue;
                            int savedDirection = perso.direction;
                            int savedUp = perso.up;
                            
                            // Temporairement désactiver les mouvements pendant l'énigme
                            perso.direction = 0;
                            perso.up = 0;
                            
                            // Lancer l'énigme
                            int enigmeResult = jouerEnigme(screen);
                            
                            // Restaurer la position et l'état du joueur après l'énigme
                            perso.position.x = savedPosX;
                            perso.position.y = savedPosY;
                            perso.posX_absolue = savedPosX_abs;
                            perso.posY_absolue = savedPosY_abs;
                            perso.direction = savedDirection;
                            perso.up = savedUp;
                            
                            if (enigmeResult) {
                                // Si l'énigme est réussie, restaurer les vies du joueur et désactiver l'ennemi
                                perso.nbcoeur = 3;
                                
                                // Désactiver l'ennemi (collision) mais garder le visuel de l'ennemi
                                // Déplacer l'ennemi hors de l'écran sans désactiver l'ennemi
                                ennemis[i].pos.x = -1000; 
                                ennemis[i].pos.y = -1000;
                                
                                // Laisser l'ennemi actif pour l'affichage mais le déplacer hors écran
                                // Ne pas désactiver l'ennemi pour éviter les problèmes d'affichage
                                // ennemis[i].active = 0; - ON SUPPRIME CETTE LIGNE
                                
                                // Forcer une mise à jour de l'affichage pour montrer les changements
                                // Redessiner l'écran avant de continuer
                                afficherPerso(perso, screen);
                                SDL_Flip(screen);
                            } else {
                                // Si l'énigme échoue, game over
                                if (messageFont) {
                                    SDL_Color textColor = {255, 0, 0};
                                    messageText = TTF_RenderText_Blended(messageFont, "Game Over! Vous avez échoué à l'énigme.", textColor);
                                    
                                    SDL_Rect messagePosition = {
                                        (screen->w - messageText->w) / 2,
                                        (screen->h - messageText->h) / 2,
                                        0, 0
                                    };
                                    
                                    // Afficher le fond et le message
                                    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
                                    SDL_BlitSurface(messageText, NULL, screen, &messagePosition);
                                    SDL_Flip(screen);
                                    SDL_Delay(3000);
                                    
                                    // Libérer les ressources
                                    SDL_FreeSurface(messageText);
                                    TTF_CloseFont(messageFont);
                                }
                                
                                running = 0; // Fin du jeu
                            }
                        }
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
        
        // Mise à jour et affichage de la minicarte
        updateMinimap(&minimap, &background, perso, bgX, bgY);
        afficherMinimap(minimap, screen);
        
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
    
    // Libérer les ressources de la minicarte
    freeMinimap(&minimap);
    
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
    
    // Variables pour la minicarte
    Minimap minimap1, minimap2;
    
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
    
    // Initialisation des minicartes
    initMinimap(&minimap1, screen->w / 2, screen->h);  // Pour l'écran gauche
    initMinimap(&minimap2, screen->w / 2, screen->h);  // Pour l'écran droit
    
    // Ajustement des positions des minicartes pour chaque moitié de l'écran
    minimap1.position.x = (screen->w / 2) - MINIMAP_WIDTH - MINIMAP_PADDING;
    minimap2.position.x = screen->w - MINIMAP_WIDTH - MINIMAP_PADDING;
    
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
            
            // Incrémenter le score lorsque le joueur 1 se déplace
            incrementScore(&perso1, deltaTime);
            
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
            
            // Incrémenter le score lorsque le joueur 2 se déplace
            incrementScore(&perso2, deltaTime);
            
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
        
        // Mise à jour et affichage de la minicarte pour joueur 1
        updateMinimap(&minimap1, &background1, perso1, bgX1, bgY1);
        afficherMinimap(minimap1, screen);
        
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
        
        // Mise à jour et affichage de la minicarte pour joueur 2
        updateMinimap(&minimap2, &background2, perso2, bgX2, bgY2);
        afficherMinimap(minimap2, screen);
        
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
    
    // Libération des minicartes
    freeMinimap(&minimap1);
    freeMinimap(&minimap2);
    
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

// Initialisation du menu des paramètres
void initSettingsMenu(SettingsMenu *menu) {
    // Initialisation des valeurs par défaut
    menu->volume = 64;  // Volume par défaut à 50%
    menu->is_fullscreen = 0;
    menu->textColor.r = 255;
    menu->textColor.g = 255;
    menu->textColor.b = 255;

    // Chargement des images pour les boutons
    menu->background = IMG_Load("img/background.jpeg");
    menu->fullscreen_background = IMG_Load("img/fullscreen_background.jpeg");
    
    // S'il n'y a pas de fond spécifique, utiliser une couleur par défaut
    if (!menu->background) {
        menu->background = SDL_CreateRGBSurface(SDL_SWSURFACE, 1500, 800, 32, 0, 0, 0, 0);
        SDL_FillRect(menu->background, NULL, SDL_MapRGB(menu->background->format, 50, 50, 100)); // Fond bleu foncé
    }
    if (!menu->fullscreen_background) {
        menu->fullscreen_background = SDL_CreateRGBSurface(SDL_SWSURFACE, 1920, 1080, 32, 0, 0, 0, 0);
        SDL_FillRect(menu->fullscreen_background, NULL, SDL_MapRGB(menu->fullscreen_background->format, 50, 50, 100)); // Fond bleu foncé
    }

    // Chargement des boutons avec gestion des erreurs
    menu->button_diminuer = IMG_Load("img/diminuer.png");
    menu->button_diminuer_hovered = IMG_Load("img/diminuer2.png");
    menu->button_augmenter = IMG_Load("img/augmenter.png");
    menu->button_augmenter_hovered = IMG_Load("img/augmenter2.png");
    menu->button_plein_ecran = IMG_Load("img/plein_ecran.png");
    menu->button_plein_ecran_hovered = IMG_Load("img/plein_ecran2.png");
    menu->button_normal = IMG_Load("img/normal.png");
    menu->button_normal_hovered = IMG_Load("img/normal2.png");
    menu->button_retour = IMG_Load("img/retour.png");
    menu->button_retour_hovered = IMG_Load("img/retour2.png");

    // En cas d'erreur, créer des boutons texte simples
    SDL_Color textColor = {255, 255, 255};
    TTF_Font *tmpFont = TTF_OpenFont("img/arial.ttf", 24);
    
    if (!menu->button_diminuer) {
        if (tmpFont) {
            menu->button_diminuer = TTF_RenderText_Solid(tmpFont, "- Volume", textColor);
            menu->button_diminuer_hovered = TTF_RenderText_Solid(tmpFont, "- VOLUME", textColor);
        }
    }
    if (!menu->button_augmenter) {
        if (tmpFont) {
            menu->button_augmenter = TTF_RenderText_Solid(tmpFont, "+ Volume", textColor);
            menu->button_augmenter_hovered = TTF_RenderText_Solid(tmpFont, "+ VOLUME", textColor);
        }
    }
    if (!menu->button_plein_ecran) {
        if (tmpFont) {
            menu->button_plein_ecran = TTF_RenderText_Solid(tmpFont, "Plein Ecran", textColor);
            menu->button_plein_ecran_hovered = TTF_RenderText_Solid(tmpFont, "PLEIN ECRAN", textColor);
        }
    }
    if (!menu->button_normal) {
        if (tmpFont) {
            menu->button_normal = TTF_RenderText_Solid(tmpFont, "Mode Normal", textColor);
            menu->button_normal_hovered = TTF_RenderText_Solid(tmpFont, "MODE NORMAL", textColor);
        }
    }
    if (!menu->button_retour) {
        if (tmpFont) {
            menu->button_retour = TTF_RenderText_Solid(tmpFont, "Retour", textColor);
            menu->button_retour_hovered = TTF_RenderText_Solid(tmpFont, "RETOUR", textColor);
        }
    }
    
    // Libérer la police temporaire si utilisée
    if (tmpFont) {
        TTF_CloseFont(tmpFont);
    }

    // Chargement du son de clic
    menu->click_sound = Mix_LoadWAV("sound/click.wav");
    if (!menu->click_sound) {
        printf("Erreur de chargement du son: %s\n", Mix_GetError());
    }

    // Chargement de la police
    menu->font = TTF_OpenFont("img/arial.ttf", 30);
    if (!menu->font) {
        printf("Erreur de chargement de la police: %s\n", TTF_GetError());
    }

    // Initialiser le volume sonore
    Mix_Volume(-1, menu->volume);
    Mix_VolumeMusic(menu->volume);
}

// Nettoyage des ressources du menu des paramètres
void cleanupSettingsMenu(SettingsMenu *menu) {
    if (menu->background) SDL_FreeSurface(menu->background);
    if (menu->fullscreen_background) SDL_FreeSurface(menu->fullscreen_background);
    if (menu->button_diminuer) SDL_FreeSurface(menu->button_diminuer);
    if (menu->button_diminuer_hovered) SDL_FreeSurface(menu->button_diminuer_hovered);
    if (menu->button_augmenter) SDL_FreeSurface(menu->button_augmenter);
    if (menu->button_augmenter_hovered) SDL_FreeSurface(menu->button_augmenter_hovered);
    if (menu->button_plein_ecran) SDL_FreeSurface(menu->button_plein_ecran);
    if (menu->button_plein_ecran_hovered) SDL_FreeSurface(menu->button_plein_ecran_hovered);
    if (menu->button_normal) SDL_FreeSurface(menu->button_normal);
    if (menu->button_normal_hovered) SDL_FreeSurface(menu->button_normal_hovered);
    if (menu->button_retour) SDL_FreeSurface(menu->button_retour);
    if (menu->button_retour_hovered) SDL_FreeSurface(menu->button_retour_hovered);
    if (menu->click_sound) Mix_FreeChunk(menu->click_sound);
    if (menu->font) TTF_CloseFont(menu->font);
}

// Gestion des survols de boutons
void handleMouseOverSettings(SDL_Rect button_rect, SDL_Surface *button_normal, SDL_Surface *button_hovered, SDL_Surface **current_button) {
    int x, y;
    SDL_GetMouseState(&x, &y);

    if (x >= button_rect.x && x <= button_rect.x + button_rect.w &&
        y >= button_rect.y && y <= button_rect.y + button_rect.h) {
        *current_button = button_hovered;  // Image survolée
    } else {
        *current_button = button_normal;   // Image normale
    }
}

// Augmentation du volume
void increaseVolume(SettingsMenu *menu) {
    menu->volume += 8;
    if (menu->volume > 128) menu->volume = 128;  // Volume maximum pour SDL_mixer
    Mix_Volume(-1, menu->volume);
    Mix_VolumeMusic(menu->volume);
}

// Diminution du volume
void decreaseVolume(SettingsMenu *menu) {
    menu->volume -= 8;
    if (menu->volume < 0) menu->volume = 0;  // Volume minimum pour SDL_mixer
    Mix_Volume(-1, menu->volume);
    Mix_VolumeMusic(menu->volume);
}

// Basculer en mode plein écran
void toggleFullscreen(SDL_Surface **screen, SettingsMenu *menu) {
    if (menu->is_fullscreen) {
        *screen = SDL_SetVideoMode(1500, 800, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
        menu->is_fullscreen = 0;
    } else {
        *screen = SDL_SetVideoMode(0, 0, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
        menu->is_fullscreen = 1;
    }
}

// Fonction pour dessiner du texte
SDL_Surface* drawSettingsText(const char *text, TTF_Font *font, SDL_Color textColor) {
    return TTF_RenderText_Solid(font, text, textColor);
}

// Boucle principale du menu des paramètres
void settingsMenuLoop(SDL_Surface **screen, SettingsMenu *menu) {
    int running = 1;
    SDL_Event event;

    // Définition des positions des boutons
    SDL_Rect volume_label_rect = { 200, 120, 0, 0 }; // Texte "Volume"
    SDL_Rect button_diminuer_rect = { 200, 200, 100, 50 }; // Bouton diminuer
    SDL_Rect button_augmenter_rect = { 500, 200, 100, 50 }; // Bouton augmenter
    SDL_Rect display_label_rect = { 200, 270, 0, 0 }; // Texte "Mode d'affichage"
    SDL_Rect button_plein_ecran_rect = { 200, 350, 100, 50 }; // Bouton plein écran
    SDL_Rect button_normal_rect = { 500, 350, 100, 50 }; // Bouton mode normal
    SDL_Rect button_retour_rect = { 680, 600, 150, 50 }; // Bouton retour
    SDL_Rect volume_value_rect = { 350, 200, 100, 50 }; // Affichage valeur du volume

    SDL_Surface *current_button_diminuer = menu->button_diminuer;
    SDL_Surface *current_button_augmenter = menu->button_augmenter;
    SDL_Surface *current_button_plein_ecran = menu->button_plein_ecran;
    SDL_Surface *current_button_normal = menu->button_normal;
    SDL_Surface *current_button_retour = menu->button_retour;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }

            // Gestion des survols de boutons
            handleMouseOverSettings(button_diminuer_rect, menu->button_diminuer, menu->button_diminuer_hovered, &current_button_diminuer);
            handleMouseOverSettings(button_augmenter_rect, menu->button_augmenter, menu->button_augmenter_hovered, &current_button_augmenter);
            handleMouseOverSettings(button_plein_ecran_rect, menu->button_plein_ecran, menu->button_plein_ecran_hovered, &current_button_plein_ecran);
            handleMouseOverSettings(button_normal_rect, menu->button_normal, menu->button_normal_hovered, &current_button_normal);
            handleMouseOverSettings(button_retour_rect, menu->button_retour, menu->button_retour_hovered, &current_button_retour);

            // Gestion des clics sur les boutons
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    // Bouton diminuer le volume
                    if (event.button.x >= button_diminuer_rect.x && event.button.x <= button_diminuer_rect.x + button_diminuer_rect.w &&
                        event.button.y >= button_diminuer_rect.y && event.button.y <= button_diminuer_rect.y + button_diminuer_rect.h) {
                        decreaseVolume(menu);
                        if (menu->click_sound) Mix_PlayChannel(-1, menu->click_sound, 0);
                    }
                    // Bouton augmenter le volume
                    if (event.button.x >= button_augmenter_rect.x && event.button.x <= button_augmenter_rect.x + button_augmenter_rect.w &&
                        event.button.y >= button_augmenter_rect.y && event.button.y <= button_augmenter_rect.y + button_augmenter_rect.h) {
                        increaseVolume(menu);
                        if (menu->click_sound) Mix_PlayChannel(-1, menu->click_sound, 0);
                    }
                    // Bouton plein écran
                    if (event.button.x >= button_plein_ecran_rect.x && event.button.x <= button_plein_ecran_rect.x + button_plein_ecran_rect.w &&
                        event.button.y >= button_plein_ecran_rect.y && event.button.y <= button_plein_ecran_rect.y + button_plein_ecran_rect.h) {
                        if (!menu->is_fullscreen) {
                            toggleFullscreen(screen, menu);
                            if (menu->click_sound) Mix_PlayChannel(-1, menu->click_sound, 0);
                        }
                    }
                    // Bouton mode normal
                    if (event.button.x >= button_normal_rect.x && event.button.x <= button_normal_rect.x + button_normal_rect.w &&
                        event.button.y >= button_normal_rect.y && event.button.y <= button_normal_rect.y + button_normal_rect.h) {
                        if (menu->is_fullscreen) {
                            toggleFullscreen(screen, menu);
                            if (menu->click_sound) Mix_PlayChannel(-1, menu->click_sound, 0);
                        }
                    }
                    // Bouton retour
                    if (event.button.x >= button_retour_rect.x && event.button.x <= button_retour_rect.x + button_retour_rect.w &&
                        event.button.y >= button_retour_rect.y && event.button.y <= button_retour_rect.y + button_retour_rect.h) {
                        running = 0;
                        if (menu->click_sound) Mix_PlayChannel(-1, menu->click_sound, 0);
                    }
                }
            }

            // Gestion des touches clavier
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = 0;
                }
                if (event.key.keysym.sym == SDLK_PLUS || event.key.keysym.sym == SDLK_KP_PLUS) {
                    increaseVolume(menu);
                    if (menu->click_sound) Mix_PlayChannel(-1, menu->click_sound, 0);
                }
                if (event.key.keysym.sym == SDLK_MINUS || event.key.keysym.sym == SDLK_KP_MINUS) {
                    decreaseVolume(menu);
                    if (menu->click_sound) Mix_PlayChannel(-1, menu->click_sound, 0);
                }
                if (event.key.keysym.sym == SDLK_F11) {
                    toggleFullscreen(screen, menu);
                    if (menu->click_sound) Mix_PlayChannel(-1, menu->click_sound, 0);
                }
            }
        }

        // Dessiner le fond d'écran
        if (menu->is_fullscreen && menu->fullscreen_background) {
            SDL_BlitSurface(menu->fullscreen_background, NULL, *screen, NULL);
        } else if (menu->background) {
            SDL_BlitSurface(menu->background, NULL, *screen, NULL);
        }

        // Dessiner les textes
        SDL_Surface *volume_label = NULL;
        SDL_Surface *display_label = NULL;
        SDL_Surface *volume_value = NULL;
        
        if (menu->font) {
            // Texte "Volume"
            volume_label = drawSettingsText("Volume", menu->font, menu->textColor);
            // Texte "Mode d'affichage"
            display_label = drawSettingsText("Mode d'affichage", menu->font, menu->textColor);
            
            // Affichage de la valeur du volume (0-100%)
            char volume_text[10];
            int percentage = (menu->volume * 100) / 128;
            sprintf(volume_text, "%d%%", percentage);
            volume_value = drawSettingsText(volume_text, menu->font, menu->textColor);
        }

        // Position des textes
        SDL_Rect volume_label_position = { 350, 120, 0, 0 };
        SDL_Rect display_label_position = { 350, 300, 0, 0 };
        SDL_Rect volume_value_position = { 350, 200, 0, 0 };

        // Dessiner les textes
        if (volume_label) SDL_BlitSurface(volume_label, NULL, *screen, &volume_label_position);
        if (display_label) SDL_BlitSurface(display_label, NULL, *screen, &display_label_position);
        if (volume_value) SDL_BlitSurface(volume_value, NULL, *screen, &volume_value_position);

        // Dessiner les boutons
        SDL_BlitSurface(current_button_diminuer, NULL, *screen, &button_diminuer_rect);
        SDL_BlitSurface(current_button_augmenter, NULL, *screen, &button_augmenter_rect);
        SDL_BlitSurface(current_button_plein_ecran, NULL, *screen, &button_plein_ecran_rect);
        SDL_BlitSurface(current_button_normal, NULL, *screen, &button_normal_rect);
        SDL_BlitSurface(current_button_retour, NULL, *screen, &button_retour_rect);

        // Mise à jour de l'écran
        SDL_Flip(*screen);

        // Libérer les surfaces
        if (volume_label) SDL_FreeSurface(volume_label);
        if (display_label) SDL_FreeSurface(display_label);
        if (volume_value) SDL_FreeSurface(volume_value);

        // Limiter à ~60 FPS
        SDL_Delay(16);
    }
}

// Fonctions pour l'énigme
void InitEnigme(enigmetf *e, char nomfichier[])
{
    FILE *f = NULL;

    e->pos_selected = 0;
    e->rc = 0;
    e->background1 = IMG_Load("enigme/img/background.png");
    e->background2 = IMG_Load("enigme/img/bg_animated.png");
    e->currentbackg = 1;
    e->button = IMG_Load("enigme/img/button.png");
    e->button_s = IMG_Load("enigme/img/button_s.png");

    SDL_Color couleur = {255, 255, 255};

    e->police = TTF_OpenFont("arial.ttf", 17);
    e->police1 = TTF_OpenFont("arial.ttf", 30);
    
    if (e->police == NULL || e->police1 == NULL) {
        printf("Erreur lors du chargement de la police: %s\n", TTF_GetError());
        if (e->police == NULL) e->police = TTF_OpenFont("img/arial.ttf", 17);
        if (e->police1 == NULL) e->police1 = TTF_OpenFont("img/arial.ttf", 30);
    }

    srand(time(NULL));
    e->num_question = rand() % 4;

    while (e->num_question == 0)
    {
        srand(time(NULL));
        e->num_question = rand() % 4;
    }
    printf("Question numéro: %d\n", e->num_question);

    char rep1[50];
    char rep2[50];
    char rep3[50];
    strcpy(rep1, "");
    strcpy(rep2, "");
    strcpy(rep3, "");
    int rc = 0;

    f = fopen(nomfichier, "r");
    if (f != NULL)
    {
        fscanf(f, "%[^_]_%[^_]_%[^_]_%[^_]_%d", e->chquestion, rep1, rep2, rep3, &rc);
        int test = 1;
        while (test != e->num_question)
        {
            fscanf(f, "%[^_]_%[^_]_%[^_]_%[^_]_%d", e->chquestion, rep1, rep2, rep3, &rc);
            test++;
        }
        fclose(f);
    }
    else {
        printf("Impossible d'ouvrir le fichier de questions.\n");
        strcpy(e->chquestion, "Quelle est la capitale de la France?");
        strcpy(rep1, "Paris");
        strcpy(rep2, "Londres");
        strcpy(rep3, "Rome");
        rc = 1;
    }

    e->rc = rc;
    printf("Réponse correcte: %d\n", e->rc);

    e->question = TTF_RenderText_Blended(e->police, e->chquestion, couleur);
    e->reponses[0] = TTF_RenderText_Blended(e->police1, rep1, couleur);
    e->reponses[1] = TTF_RenderText_Blended(e->police1, rep2, couleur);
    e->reponses[2] = TTF_RenderText_Blended(e->police1, rep3, couleur);

    // Positions des éléments de l'énigme
    e->pos_question.x = 270;
    e->pos_question.y = 270;

    e->pos_reponse1.x = 380;
    e->pos_reponse1.y = 500;

    e->pos_reponse2.x = 380 - 240;
    e->pos_reponse2.y = 500 + 180;

    e->pos_reponse3.x = 380 + 240;
    e->pos_reponse3.y = 500 + 180;

    e->pos_reponse1txt.x = e->pos_reponse1.x + 50;
    e->pos_reponse1txt.y = e->pos_reponse1.y + 30;

    e->pos_reponse2txt.x = e->pos_reponse2.x + 50;
    e->pos_reponse2txt.y = e->pos_reponse2.y + 30;

    e->pos_reponse3txt.x = e->pos_reponse3.x + 50;
    e->pos_reponse3txt.y = e->pos_reponse3.y + 30;

    e->image_clock = IMG_Load("enigme/img/clock1.png");

    e->pos_image_clock.x = 500;
    e->pos_image_clock.y = 100;

    e->single_Clock.w = 57;
    e->single_Clock.h = 81;
    e->single_Clock.x = 0;
    e->single_Clock.y = 0;

    e->clock_num = 0;
}

void afficherEnigme(enigmetf e, SDL_Surface *ecran)
{
    if (e.currentbackg == 1)
    {
        SDL_BlitSurface(e.background1, NULL, ecran, NULL);
    }
    else
    {
        SDL_BlitSurface(e.background2, NULL, ecran, NULL);
    }
    displayClock(e, ecran);
    SDL_BlitSurface(e.question, NULL, ecran, &e.pos_question);

    switch (e.pos_selected)
    {
    case 0:
        SDL_BlitSurface(e.button, NULL, ecran, &e.pos_reponse1);
        SDL_BlitSurface(e.button, NULL, ecran, &e.pos_reponse2);
        SDL_BlitSurface(e.button, NULL, ecran, &e.pos_reponse3);
        SDL_BlitSurface(e.reponses[0], NULL, ecran, &e.pos_reponse1txt);
        SDL_BlitSurface(e.reponses[1], NULL, ecran, &e.pos_reponse2txt);
        SDL_BlitSurface(e.reponses[2], NULL, ecran, &e.pos_reponse3txt);
        break;

    case 1:
    {
        SDL_BlitSurface(e.button_s, NULL, ecran, &e.pos_reponse1);
        SDL_BlitSurface(e.button, NULL, ecran, &e.pos_reponse2);
        SDL_BlitSurface(e.button, NULL, ecran, &e.pos_reponse3);
        SDL_BlitSurface(e.reponses[0], NULL, ecran, &e.pos_reponse1txt);
        SDL_BlitSurface(e.reponses[1], NULL, ecran, &e.pos_reponse2txt);
        SDL_BlitSurface(e.reponses[2], NULL, ecran, &e.pos_reponse3txt);
    }
    break;
    case 2:
    {
        SDL_BlitSurface(e.button, NULL, ecran, &e.pos_reponse1);
        SDL_BlitSurface(e.button_s, NULL, ecran, &e.pos_reponse2);
        SDL_BlitSurface(e.button, NULL, ecran, &e.pos_reponse3);
        SDL_BlitSurface(e.reponses[0], NULL, ecran, &e.pos_reponse1txt);
        SDL_BlitSurface(e.reponses[1], NULL, ecran, &e.pos_reponse2txt);
        SDL_BlitSurface(e.reponses[2], NULL, ecran, &e.pos_reponse3txt);
    }
    break;
    case 3:
    {
        SDL_BlitSurface(e.button, NULL, ecran, &e.pos_reponse1);
        SDL_BlitSurface(e.button, NULL, ecran, &e.pos_reponse2);
        SDL_BlitSurface(e.button_s, NULL, ecran, &e.pos_reponse3);
        SDL_BlitSurface(e.reponses[0], NULL, ecran, &e.pos_reponse1txt);
        SDL_BlitSurface(e.reponses[1], NULL, ecran, &e.pos_reponse2txt);
        SDL_BlitSurface(e.reponses[2], NULL, ecran, &e.pos_reponse3txt);
    }
    break;
    default:
        break;
    }
}

void displayClock(enigmetf e, SDL_Surface *ecran)
{
    SDL_BlitSurface(e.image_clock, &(e.single_Clock), ecran, &e.pos_image_clock);
}

void verify_enigme(enigmetf e, SDL_Surface *ecran)
{
    SDL_Rect pos = {160, 330};
    printf("Choix sélectionné: %d\n", e.pos_selected);
    printf("Réponse correcte: %d\n", e.rc);

    if (e.pos_selected == e.rc)
    {
        printf("Vous avez gagné!\n");
        SDL_Surface *win = IMG_Load("enigme/img/youwin.png");
        if (win) {
            SDL_BlitSurface(win, NULL, ecran, &pos);
            SDL_Flip(ecran);
            SDL_FreeSurface(win);
            SDL_Delay(2000);
        }
    }
    else
    {
        SDL_Surface *lost = IMG_Load("enigme/img/youlost.png");
        if (lost) {
            SDL_BlitSurface(lost, NULL, ecran, &pos);
            SDL_Flip(ecran);
            printf("Vous avez perdu!\n");
            SDL_FreeSurface(lost);
            SDL_Delay(2000);
        }
    }
}

void animer(enigmetf *e)
{
    if (e->clock_num >= 0 && e->clock_num < 8)
    {
        e->single_Clock.x = e->clock_num * e->single_Clock.w;
        e->clock_num++;
    }

    if (e->clock_num == 8)
    {
        e->single_Clock.x = e->clock_num * e->single_Clock.w;
        e->clock_num = 0;
    }
    SDL_Delay(50);
}

void animer_background(enigmetf *e)
{
    static int frame = 0;
    frame++;
    if (frame % 7 == 0)
    {
        e->currentbackg = (e->currentbackg == 1) ? 2 : 1;
    }
}

void free_Surface_enigme(enigmetf *e)
{
    if (e->background1) SDL_FreeSurface(e->background1);
    if (e->background2) SDL_FreeSurface(e->background2);
    if (e->question) SDL_FreeSurface(e->question);
    if (e->reponses[0]) SDL_FreeSurface(e->reponses[0]);
    if (e->reponses[1]) SDL_FreeSurface(e->reponses[1]);
    if (e->reponses[2]) SDL_FreeSurface(e->reponses[2]);
    if (e->button) SDL_FreeSurface(e->button);
    if (e->button_s) SDL_FreeSurface(e->button_s);
    if (e->image_clock) SDL_FreeSurface(e->image_clock);
    
    if (e->police) TTF_CloseFont(e->police);
    if (e->police1) TTF_CloseFont(e->police1);
}

// Fonction principale pour jouer à l'énigme
int jouerEnigme(SDL_Surface *ecran)
{
    enigmetf e;
    int quit = 0;
    int timeOut = 0;
    int result = 0; // 0 = perdu, 1 = gagné
    Uint32 timerDebut;
    Uint32 tempsRestant = 30000; // 30 secondes
    SDL_Event event;
    
    // Initialiser TTF si ce n'est pas déjà fait
    if (TTF_WasInit() == 0) {
        if (TTF_Init() == -1) {
            printf("Erreur d'initialisation de TTF: %s\n", TTF_GetError());
            return 0;
        }
    }

    // Initialiser l'énigme
    InitEnigme(&e, "enigme/fichier.txt");
    timerDebut = SDL_GetTicks();

    // Boucle principale du jeu d'énigme
    while (!quit && !timeOut)
    {
        // Gestion des événements
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = 1;
                break;
            case SDL_KEYDOWN:
                // Liste des touches à ignorer pour bloquer les mouvements du joueur pendant l'énigme
                if (event.key.keysym.sym == SDLK_SPACE || 
                    event.key.keysym.sym == SDLK_d ||
                    event.key.keysym.sym == SDLK_q ||
                    event.key.keysym.sym == SDLK_z) {
                    // Ne rien faire pour ces touches (bloquer les mouvements)
                    break;
                }
                
                // Gestion des autres touches pour l'énigme
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    quit = 1;
                    break;
                case SDLK_UP:
                    e.pos_selected = 1;
                    break;
                case SDLK_DOWN:
                    if (e.pos_selected == 1)
                        e.pos_selected = 2;
                    else
                        e.pos_selected = 3;
                    break;
                case SDLK_RIGHT:
                    e.pos_selected = 3;
                    break;
                case SDLK_LEFT:
                    e.pos_selected = 2;
                    break;
                case SDLK_RETURN:
                    verify_enigme(e, ecran);
                    if (e.pos_selected == e.rc)
                    {
                        result = 1;
                    }
                    quit = 1;
                    break;
                default:
                    break;
                }
                break;
            case SDL_MOUSEMOTION:
                // Détecter sur quelle réponse la souris se trouve
                if (e.button && event.motion.x >= e.pos_reponse1.x && event.motion.x <= e.pos_reponse1.x + e.button->w &&
                    event.motion.y >= e.pos_reponse1.y && event.motion.y <= e.pos_reponse1.y + e.button->h)
                {
                    e.pos_selected = 1;
                }
                else if (e.button && event.motion.x >= e.pos_reponse2.x && event.motion.x <= e.pos_reponse2.x + e.button->w &&
                         event.motion.y >= e.pos_reponse2.y && event.motion.y <= e.pos_reponse2.y + e.button->h)
                {
                    e.pos_selected = 2;
                }
                else if (e.button && event.motion.x >= e.pos_reponse3.x && event.motion.x <= e.pos_reponse3.x + e.button->w &&
                         event.motion.y >= e.pos_reponse3.y && event.motion.y <= e.pos_reponse3.y + e.button->h)
                {
                    e.pos_selected = 3;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    // Vérifier sur quel bouton l'utilisateur a cliqué
                    if (e.button && event.button.x >= e.pos_reponse1.x && event.button.x <= e.pos_reponse1.x + e.button->w &&
                        event.button.y >= e.pos_reponse1.y && event.button.y <= e.pos_reponse1.y + e.button->h)
                    {
                        e.pos_selected = 1;
                        verify_enigme(e, ecran);
                        if (e.pos_selected == e.rc)
                        {
                            result = 1;
                        }
                        quit = 1;
                    }
                    else if (e.button && event.button.x >= e.pos_reponse2.x && event.button.x <= e.pos_reponse2.x + e.button->w &&
                             event.button.y >= e.pos_reponse2.y && event.button.y <= e.pos_reponse2.y + e.button->h)
                    {
                        e.pos_selected = 2;
                        verify_enigme(e, ecran);
                        if (e.pos_selected == e.rc)
                        {
                            result = 1;
                        }
                        quit = 1;
                    }
                    else if (e.button && event.button.x >= e.pos_reponse3.x && event.button.x <= e.pos_reponse3.x + e.button->w &&
                             event.button.y >= e.pos_reponse3.y && event.button.y <= e.pos_reponse3.y + e.button->h)
                    {
                        e.pos_selected = 3;
                        verify_enigme(e, ecran);
                        if (e.pos_selected == e.rc)
                        {
                            result = 1;
                        }
                        quit = 1;
                    }
                }
                break;
            }
        }

        // Vérifier le temps restant
        Uint32 tempsEcoule = SDL_GetTicks() - timerDebut;
        if (tempsEcoule > tempsRestant)
        {
            timeOut = 1;
            SDL_Rect pos = {160, 330};
            SDL_Surface *lost = IMG_Load("enigme/img/youlost.png");
            if (lost) {
                SDL_BlitSurface(lost, NULL, ecran, &pos);
                SDL_Flip(ecran);
                printf("Temps écoulé! Vous avez perdu!\n");
                SDL_FreeSurface(lost);
                SDL_Delay(2000);
            }
        }

        // Animer l'horloge et le fond
        animer(&e);
        animer_background(&e);

        // Afficher l'énigme
        afficherEnigme(e, ecran);
        SDL_Flip(ecran);
    }

    // Libérer les ressources
    free_Surface_enigme(&e);
    
    // Un petit délai pour assurer que la mémoire est bien libérée
    SDL_Delay(100);

    // Retourner le résultat
    return result;
}

// Initialisation de la minicarte
void initMinimap(Minimap *minimap, int screenWidth, int screenHeight) {
    // Créer la surface pour la minicarte
    minimap->surface = SDL_CreateRGBSurface(SDL_SWSURFACE, MINIMAP_WIDTH, MINIMAP_HEIGHT, 32, 
                                          0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    
    // Créer une petite icône rouge pour représenter le joueur sur la minicarte
    minimap->player_icon = SDL_CreateRGBSurface(SDL_SWSURFACE, 6, 6, 32, 
                                              0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    SDL_FillRect(minimap->player_icon, NULL, SDL_MapRGB(minimap->player_icon->format, 255, 0, 0));
    
    // Position de la minicarte (coin supérieur droit)
    minimap->position.x = screenWidth - MINIMAP_WIDTH - MINIMAP_PADDING;
    minimap->position.y = MINIMAP_PADDING;
    minimap->position.w = MINIMAP_WIDTH;
    minimap->position.h = MINIMAP_HEIGHT;
    
    // Facteurs d'échelle par défaut (seront ajustés plus tard)
    minimap->scale_x = 0.1f;
    minimap->scale_y = 0.1f;
}

// Mise à jour de la minicarte pour le mode solo
void updateMinimap(Minimap *minimap, Background *bg, Personne p, int bgX, int bgY) {
    // Calcul des facteurs d'échelle en fonction de la taille du monde
    // Supposons que la largeur du monde est la largeur de l'image de fond
    minimap->scale_x = (float)MINIMAP_WIDTH / bg->img->w;
    minimap->scale_y = (float)MINIMAP_HEIGHT / bg->img->h;
    
    // Créer une version réduite du fond
    SDL_Surface *scaled_bg = SDL_CreateRGBSurface(SDL_SWSURFACE, MINIMAP_WIDTH, MINIMAP_HEIGHT, 32,
                                                0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    
    // Remplir la minicarte avec une couleur de fond sombre
    SDL_FillRect(minimap->surface, NULL, SDL_MapRGB(minimap->surface->format, 20, 20, 50));
    
    // Dessiner une version réduite du fond visible actuellement
    SDL_Rect src = {bgX, bgY, bg->img->w, bg->img->h};
    SDL_Rect dst = {0, 0, MINIMAP_WIDTH, MINIMAP_HEIGHT};
    SDL_SoftStretch(bg->img, &src, scaled_bg, NULL);
    SDL_BlitSurface(scaled_bg, NULL, minimap->surface, NULL);
    
    // Calculer la position du joueur sur la minicarte
    // Position X = (position absolue * facteur d'échelle)
    // Position Y = (position absolue * facteur d'échelle)
    SDL_Rect player_pos = {
        (int)(p.posX_absolue * minimap->scale_x),
        (int)(p.posY_absolue * minimap->scale_y),
        minimap->player_icon->w,
        minimap->player_icon->h
    };
    
    // Dessiner l'icône du joueur sur la minicarte
    SDL_BlitSurface(minimap->player_icon, NULL, minimap->surface, &player_pos);
    
    // Ajouter un cadre autour de la minicarte
    SDL_Rect border = {0, 0, MINIMAP_WIDTH, MINIMAP_HEIGHT};
    // Dessiner les lignes horizontales du cadre
    SDL_Rect top_line = {0, 0, MINIMAP_WIDTH, 2};
    SDL_Rect bottom_line = {0, MINIMAP_HEIGHT - 2, MINIMAP_WIDTH, 2};
    SDL_FillRect(minimap->surface, &top_line, SDL_MapRGB(minimap->surface->format, 255, 255, 255));
    SDL_FillRect(minimap->surface, &bottom_line, SDL_MapRGB(minimap->surface->format, 255, 255, 255));
    
    // Dessiner les lignes verticales du cadre
    SDL_Rect left_line = {0, 0, 2, MINIMAP_HEIGHT};
    SDL_Rect right_line = {MINIMAP_WIDTH - 2, 0, 2, MINIMAP_HEIGHT};
    SDL_FillRect(minimap->surface, &left_line, SDL_MapRGB(minimap->surface->format, 255, 255, 255));
    SDL_FillRect(minimap->surface, &right_line, SDL_MapRGB(minimap->surface->format, 255, 255, 255));
    
    // Libérer la surface temporaire
    SDL_FreeSurface(scaled_bg);
}

// Mise à jour de la minicarte pour le mode duo
void updateMinimapDuo(Minimap *minimap, Background *bg, Personne p1, Personne p2, int bgX, int bgY) {
    // Calcul des facteurs d'échelle
    minimap->scale_x = (float)MINIMAP_WIDTH / bg->img->w;
    minimap->scale_y = (float)MINIMAP_HEIGHT / bg->img->h;
    
    // Créer une version réduite du fond
    SDL_Surface *scaled_bg = SDL_CreateRGBSurface(SDL_SWSURFACE, MINIMAP_WIDTH, MINIMAP_HEIGHT, 32,
                                                0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    
    // Remplir la minicarte avec une couleur de fond sombre
    SDL_FillRect(minimap->surface, NULL, SDL_MapRGB(minimap->surface->format, 20, 20, 50));
    
    // Dessiner une version réduite du fond visible actuellement
    SDL_Rect src = {bgX, bgY, bg->img->w, bg->img->h};
    SDL_Rect dst = {0, 0, MINIMAP_WIDTH, MINIMAP_HEIGHT};
    SDL_SoftStretch(bg->img, &src, scaled_bg, NULL);
    SDL_BlitSurface(scaled_bg, NULL, minimap->surface, NULL);
    
    // Calculer la position du joueur 1 sur la minicarte
    SDL_Rect player1_pos = {
        (int)(p1.posX_absolue * minimap->scale_x),
        (int)(p1.posY_absolue * minimap->scale_y),
        minimap->player_icon->w,
        minimap->player_icon->h
    };
    
    // Calculer la position du joueur 2 sur la minicarte
    SDL_Rect player2_pos = {
        (int)(p2.posX_absolue * minimap->scale_x),
        (int)(p2.posY_absolue * minimap->scale_y),
        minimap->player_icon->w,
        minimap->player_icon->h
    };
    
    // Dessiner l'icône du joueur 1 sur la minicarte (point rouge)
    SDL_BlitSurface(minimap->player_icon, NULL, minimap->surface, &player1_pos);
    
    // Créer une icône bleue pour le joueur 2
    SDL_Surface *player2_icon = SDL_CreateRGBSurface(SDL_SWSURFACE, 6, 6, 32,
                                                   0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    SDL_FillRect(player2_icon, NULL, SDL_MapRGB(player2_icon->format, 0, 0, 255));
    
    // Dessiner l'icône du joueur 2 sur la minicarte (point bleu)
    SDL_BlitSurface(player2_icon, NULL, minimap->surface, &player2_pos);
    
    // Ajouter un cadre autour de la minicarte
    SDL_Rect top_line = {0, 0, MINIMAP_WIDTH, 2};
    SDL_Rect bottom_line = {0, MINIMAP_HEIGHT - 2, MINIMAP_WIDTH, 2};
    SDL_FillRect(minimap->surface, &top_line, SDL_MapRGB(minimap->surface->format, 255, 255, 255));
    SDL_FillRect(minimap->surface, &bottom_line, SDL_MapRGB(minimap->surface->format, 255, 255, 255));
    
    SDL_Rect left_line = {0, 0, 2, MINIMAP_HEIGHT};
    SDL_Rect right_line = {MINIMAP_WIDTH - 2, 0, 2, MINIMAP_HEIGHT};
    SDL_FillRect(minimap->surface, &left_line, SDL_MapRGB(minimap->surface->format, 255, 255, 255));
    SDL_FillRect(minimap->surface, &right_line, SDL_MapRGB(minimap->surface->format, 255, 255, 255));
    
    // Libérer les surfaces temporaires
    SDL_FreeSurface(scaled_bg);
    SDL_FreeSurface(player2_icon);
}

// Affichage de la minicarte
void afficherMinimap(Minimap minimap, SDL_Surface *screen) {
    // Afficher la minicarte sur l'écran
    SDL_BlitSurface(minimap.surface, NULL, screen, &minimap.position);
}

// Libération des ressources de la minicarte
void freeMinimap(Minimap *minimap) {
    if (minimap->surface) {
        SDL_FreeSurface(minimap->surface);
    }
    if (minimap->player_icon) {
        SDL_FreeSurface(minimap->player_icon);
    }
}