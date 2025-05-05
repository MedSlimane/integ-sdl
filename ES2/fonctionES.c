#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <math.h>
#include <stdio.h>

#include "SDL/SDL.h"
#include "headerES.h"

void initEnnemi(Ennemi *e) {
    e->enem[1] = IMG_Load("1.png");
    e->enem[2] = IMG_Load("2.png");
    e->enem[3] = IMG_Load("3.png");
    e->enem[4] = IMG_Load("4.png");
    e->enem[5] = IMG_Load("5.png");
    e->enem[6] = IMG_Load("6.png");
    e->enem[7] = IMG_Load("7.png");
    e->enem[8] = IMG_Load("8.png");
    e->enem[9] = IMG_Load("9.png");
    e->enem[10] = IMG_Load("10.png");
    e->enem[11] = IMG_Load("11.png");
    e->enem[12] = IMG_Load("12.png");
    e->enem[13] = IMG_Load("13.png");
    e->enem[14] = IMG_Load("14.png");
    e->enem[15] = IMG_Load("15.png");
    e->enem[16] = IMG_Load("16.png");
    e->enem[17] = IMG_Load("17.png");
    e->pos.x = 300;
    e->pos.y = 300;
    e->pos.w =
        e->enem[1]->w;  // Peu importe lequel, ils sont sûrement tous pareils
    e->pos.h = e->enem[1]->h;

    e->d = 0;
    e->num = 0;
}
void afficherEnnemi(Ennemi e, SDL_Surface *screen)

{
    SDL_BlitSurface(e.enem[e.num], NULL, screen, &e.pos);
    // SDL_Flip(screen);
}

void animerEnnemi(Ennemi *e, SDL_Surface *screen) {
    SDL_Delay(200);

    SDL_Flip(screen);
    e->num++;
    if (e->num == 17) {
        e->num = 0;
    }
}
void deplacer(Ennemi *e) {
    int posmax = 800;
    int posmin = 300;
    if (e->pos.x > posmax) {
        e->d = 1;
    }
    if (e->pos.x < posmin) {
        e->d = 0;
    }
    if (e->d == 0) {
        e->pos.x += 50;
    } else if (e->d == 1) {
        e->pos.x -= 50;
    }
}
/*void deplacer(Ennemi *e){
if(e->d == 0) {
   if(e->pos.x < 850) {
       e->pos.x += 10;
       e->pos.y = 200;
   } else {
       e->d = 1;
   }
} else if(e->d == 1) {
   if(e->pos.x > 50) {
       e->pos.x -= 10;
       e->pos.y = 100;
   } else {
       e->d = 0;
   }
}
}*/
int collisionBB(personne p, Ennemi e) {
    int collision;
    if ((p.pos.x + p.pos.w < e.pos.x) || (p.pos.x > e.pos.x + e.pos.w) ||
        (p.pos.y + p.pos.h < e.pos.y) || (p.pos.y > e.pos.y + e.pos.h)) {
        collision = 0;  // pas de collision
    } else {
        collision = 1;  // il y a une collision
    }
    return collision;
}

/*void initCoeurs(Coeur coeurs[])
{
    int i;
    char nom_image[50];
    for (i = 0; i < 8; i++)
    {

        sprintf(nom_image, "bonus/%d.png", i + 1);
        coeurs[i].image = IMG_Load(nom_image);
        coeurs[i].position.x = 100; // Position aléatoire en x
        coeurs[i].position.y = 300; // Position aléatoire en y
        coeurs[i].position.w = 50;
        coeurs[i].position.h = 50;
        coeurs[i].visible = 1;
    }
}*/
void initCoeurs(Coeur coeurs[]) {
    int i;
    char nom_image[50];
    for (i = 0; i < 8; i++) {
        sprintf(nom_image, "bonus/%d.png", i + 1);
        coeurs[i].image = IMG_Load(nom_image);
        coeurs[i].position.x = rand() % 600;  // Position aléatoire en x
        coeurs[i].position.y = rand() % 700;  // Position aléatoire en y

        if (coeurs[i].image != NULL) {
            coeurs[i].position.w = coeurs[i].image->w;
            coeurs[i].position.h = coeurs[i].image->h;
        }

        coeurs[i].visible = 1;
    }
}

void afficherCoeurs(Coeur coeurs[], SDL_Surface *screen) {
    int i;
    for (i = 0; i < 8; i++) {
        if (coeurs[i].visible) {
            SDL_BlitSurface(coeurs[i].image, NULL, screen, &coeurs[i].position);
        }
    }
}

int collisionTri(personne p, Coeur coeur) {
    float centerX1 = p.pos.x + p.pos.w / 2.0;
    float centerY1 = p.pos.y + p.pos.h / 2.0;
    float R1 = fmax(p.pos.w / 2.0, p.pos.h / 2.0);  // Rayon du cercle

    float centerX2 = coeur.position.x + coeur.position.w / 2.0;
    float centerY2 = coeur.position.y + coeur.position.h / 2.0;
    float R2 = coeur.position.w / 2.0;  // Rayon du cercle

    float dx = centerX2 - centerX1;
    float dy = centerY2 - centerY1;
    float D =
        sqrt(dx * dx + dy * dy);  // Distance entre les centres des deux cercles

    if (D <= R1 + R2) {
        return 1;
    } else {
        return 0;
    }
}
