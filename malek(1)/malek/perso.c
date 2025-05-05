#include "perso.h"

void initPerso2(Personne *p) {
    (*p).imagePers = IMG_Load("spritesheet.png");

    p->position.x = 200;
    p->position.y = 700;

    p->image.w = 100;
    p->image.h = 100;
    p->image.x = 0;
    p->image.y = 0;

    p->nbimage = 0;

    p->direction = 0;

    p->vx_saut = 4;
    p->v_grav = 0.5;
    p->v_saut = -6;
    p->v_y = p->v_saut;

    p->acceleration = 0;
    p->vitesse = 5;
    p->imagec = IMG_Load("coeur.png");
    p->positionc.x = 1400;
    p->positionc.y = 0;
    p->nbcoeur = 3;
    p->up = 0; // Initialize jump flag

    TTF_Init();
    p->score.font = TTF_OpenFont("arial.ttf", 20);

    p->score.color = (SDL_Color){255, 255, 255};
    p->score.text =
        TTF_RenderText_Blended(p->score.font, "score2:", p->score.color);
    p->vie.text =
        TTF_RenderText_Blended(p->score.font, "vie2:", p->score.color);
    p->vie.postext.x = 1100;
    p->vie.postext.y = 0;

    p->score.postext.x = 800;
    p->score.postext.y = 0;

    p->Vscore = 0;
}

void initPerso(Personne *p) {
    (*p).imagePers = IMG_Load("spritesheet.png");

    p->position.x = 0;
    p->position.y = 700;

    p->image.w = 100;
    p->image.h = 100;
    p->image.x = 0;
    p->image.y = 0;

    p->nbimage = 0;

    p->direction = 0;

    p->vx_saut = 4;
    p->v_grav = 0.5;
    p->v_saut = -6;
    p->v_y = p->v_saut;

    p->acceleration = 0;
    p->vitesse = 5;
    p->imagec = IMG_Load("coeur.png");
    p->positionc.x = 545;
    p->positionc.y = 0;
    p->nbcoeur = 3;
    p->up = 0; // Initialize jump flag
    TTF_Init();
    p->score.font = TTF_OpenFont("arial.ttf", 20);

    p->score.color = (SDL_Color){255, 255, 255};
    p->score.text =
        TTF_RenderText_Blended(p->score.font, "score1:", p->score.color);
    p->vie.text = TTF_RenderText_Blended(p->score.font, "vie1: ", p->score.color);
    p->vie.postext.x = 300;
    p->vie.postext.y = 0;

    p->score.postext.x = 100;
    p->score.postext.y = 0;

    p->Vscore = 0;
}

void afficherPerso(Personne p, SDL_Surface *screen) {
    char vsc[30];
    sprintf(vsc, "score: %d", p.Vscore);

    p.score.text = TTF_RenderText_Blended(p.score.font, vsc, p.score.color);
    SDL_BlitSurface(p.score.text, NULL, screen, &p.score.postext);
    SDL_BlitSurface(p.vie.text, NULL, screen, &p.vie.postext);

    SDL_BlitSurface(p.imagePers, &(p.image), screen,
                    &(p.position));  

    switch (p.nbcoeur) {
        case 1:
            SDL_BlitSurface(p.imagec, NULL, screen, &p.positionc);
            break;
        case 2:
            SDL_BlitSurface(p.imagec, NULL, screen, &p.positionc);
            p.positionc.x = 545 - 60;
            SDL_BlitSurface(p.imagec, NULL, screen, &p.positionc);
            break;
        case 3:
            SDL_BlitSurface(p.imagec, NULL, screen, &p.positionc);
            p.positionc.x = 545 - 60;
            SDL_BlitSurface(p.imagec, NULL, screen, &p.positionc);
            p.positionc.x = 545 - 60 - 60;
            SDL_BlitSurface(p.imagec, NULL, screen, &p.positionc);
            break;
    }
}

void afficherPerso2(Personne p, SDL_Surface *screen) {
    char vsc[30];
    sprintf(vsc, "score2: %d", p.Vscore);

    p.score.text = TTF_RenderText_Blended(p.score.font, vsc, p.score.color);
    SDL_BlitSurface(p.score.text, NULL, screen, &p.score.postext);
    SDL_BlitSurface(p.vie.text, NULL, screen, &p.vie.postext);

    SDL_BlitSurface(p.imagePers, &(p.image), screen,
                    &(p.position));  

    switch (p.nbcoeur) {
        case 1:
            SDL_BlitSurface(p.imagec, NULL, screen, &p.positionc);
            break;
        case 2:
            SDL_BlitSurface(p.imagec, NULL, screen, &p.positionc);
            p.positionc.x = 1400 - 60;
            SDL_BlitSurface(p.imagec, NULL, screen, &p.positionc);
            break;
        case 3:
            SDL_BlitSurface(p.imagec, NULL, screen, &p.positionc);
            p.positionc.x = 1400 - 60;
            SDL_BlitSurface(p.imagec, NULL, screen, &p.positionc);
            p.positionc.x = 1400 - 60 - 60;
            SDL_BlitSurface(p.imagec, NULL, screen, &p.positionc);
            break;
    }
}

void deplacerPerso(Personne *p, Uint32 dt) {
    switch (p->direction) {
        case 1:
            p->position.x +=
                0.5 * p->acceleration * (dt) * (dt) + p->vitesse * dt;
            break;
        case 2:
            p->position.x -=
                0.5 * p->acceleration * (dt) * (dt) + p->vitesse * dt;

            break;
        case 3:
            p->position.y -= 3;
            break;
    }
}

void animerPerso(Personne *p) {
    if (p->direction == 1) {
        p->image.y = 0;
        p->nbimage++;
        if (p->nbimage >= 6) {
            p->nbimage = 0;
        }
        p->image.x = p->nbimage * p->image.w;

    } else if (p->direction == 2) {
        p->image.y = 100;
        p->nbimage++;
        if (p->nbimage >= 6) {
            p->nbimage = 0;
        }
        p->image.x = p->nbimage * p->image.w;

    } else if (p->direction == 3) {
        p->image.y = 200; // Attack right animation
        p->nbimage++;
        if (p->nbimage >= 6) {
            p->nbimage = 0;
        }
        p->image.x = p->nbimage * p->image.w;

    } else if (p->direction == 4) {
        p->image.y = 300; // Attack left animation
        p->nbimage++;
        if (p->nbimage >= 6) {
            p->nbimage = 0;
        }
        p->image.x = p->nbimage * p->image.w;

    } else if (p->direction == 0) {
        p->image.x = 0;
    }
}



// Implement the new jump function according to the specification
void saut_Personnage(Personne *P, Uint32 dt, int posx_absolu, int posy_absolu) {
    if (P->up == 1) {  // Si le personnage a fait un saut
        // Mise à jour de la position du personnage selon l'axe des abscisses
        if (P->direction == 1) {  // Droite
            P->position.x += P->vx_saut;  // Déplacement horizontal vers la droite
        } else if (P->direction == 2) {  // Gauche
            P->position.x -= P->vx_saut;  // Déplacement horizontal vers la gauche
        }
        
        // Mise à jour de la position verticale avec la gravité
        P->position.y += P->v_y;
        P->v_y += P->v_grav;
        
        // Vérifier si le personnage a atteint ou dépassé la position initiale au sol
        if (P->position.y >= posy_absolu) {
            P->position.y = posy_absolu;  // Replacer le personnage au niveau du sol
            P->v_y = P->v_saut;  // Réinitialiser la vitesse verticale pour le prochain saut
            P->up = 0;  // Désactiver l'état de saut
        }
        
        // Animer le personnage pendant le saut selon sa direction
        animerPerso(P);
    }
}
