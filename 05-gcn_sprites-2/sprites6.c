#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "common.h"
#include "sprite.h"

enum
{
    NB_SPRITES = 100
};

typedef struct Positions_t
{
    int16_t destX[2];
    int16_t destY[2];
} Positions;

/* Met à jour un sprite à partir de sa structure */
void update_sprite(Sprite* oneSprite, Positions* oldPositions, int currentTime)
{
    /* Si on a dépassé la "date" de mise à jour du sprite, on passe */
    /* à l'étape suivante de l'animation */
    if (currentTime > oneSprite->nextUpdateTime)
    {
        /* On sélectionne le sprite suivant */
        oneSprite->step++;
        if (oneSprite->step > 9)
        {
            oneSprite->step = 0;
        }

        /* On fixe la date du prochain changement */
        /* en fonction de la période du sprite    */
        oneSprite->nextUpdateTime = currentTime + oneSprite->speed;
    }

    /* Sauvegarde des anciennes positions */
    oldPositions->destX[1] = oldPositions->destX[0];
    oldPositions->destX[0] = oneSprite->destX;

    oldPositions->destY[1] = oldPositions->destY[0];
    oldPositions->destY[0] = oneSprite->destY;

    /* Mise à jour de la position */
    oneSprite->destX += oneSprite->dispX;
    oneSprite->destY += oneSprite->dispY;

    /* Rebondi sur les bords */
    if (oneSprite->destX < 0)
    {
        oneSprite->destX = 0;
        oneSprite->dispX = get_random(1, 10);
    }
    if (oneSprite->destX > 639 - 32)
    {
        oneSprite->destX = 639 - 32;
        oneSprite->dispX = get_random(-10, -1);
    }
    if (oneSprite->destY < 0)
    {
        oneSprite->destY = 0;
        oneSprite->dispY = get_random(1, 10);
    }
    if (oneSprite->destY > 479 - 32)
    {
        oneSprite->destY = 479 - 32;
        oneSprite->dispY = get_random(-10, -1);
    }
}

Positions* create_old_positions()
{
    Positions* oldPositions = malloc(sizeof(Positions));

    return oldPositions;
}

/* Efface le sprite à son ancienne position */
void erase_sprite(SDL_Surface* screen, Positions* oldPositions)
{
    /* Rectangle contenant les anciennes coordonnées du sprite à l'écran */
    SDL_Rect sdlDstSprite = {
            oldPositions->destX[1],
            oldPositions->destY[1],
            32,
            32
    };

    /* On efface cette position */
    SDL_FillRect(screen, &sdlDstSprite, 0);
}

void main_loop(SDL_Surface* screen)
{
    SDL_Surface* loadedPicture = load_picture("32sprites.bmp"); /* Charge l'image qui contient les sprites */

    /* Déclare la couleur noire comme transparente sur cette surface */
    SDL_SetColorKey(loadedPicture, SDL_SRCCOLORKEY, 0);

    srand(SDL_GetTicks()); /* Initialisation du générateur pseudo aléatoire */

    /* Tableau de sprites */
    Sprite* arraySprites[NB_SPRITES];
    Positions* spriteOldPositions[NB_SPRITES];
    for (int i = 0; i < NB_SPRITES; i++)
    {
        arraySprites[i] = create_random_sprite(loadedPicture);
        spriteOldPositions[i] = create_old_positions();
    }

    /* Boucle principale */
    bool quitProgram = false;   /* Drapeau de fin de boucle */
    while (!quitProgram)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    quitProgram = true;
                    break;
                default:
                    break;
            }
        }

        int currentTime = SDL_GetTicks(); /* On prend la "date" actuelle */

        /* Appel de la fonction mise à jour d'un sprite */
        /* pour tout le tableau des sprites */
        /* d'effacement des anciennes positions */
        /* puis d'affichage aux nouvelles positions */
        for (int i = 0; i < NB_SPRITES; i++)
        {
            update_sprite(arraySprites[i], spriteOldPositions[i], currentTime);
            erase_sprite(screen, spriteOldPositions[i]);
        }

        /* Affichage de tous les sprites */
        for (int i = 0; i < NB_SPRITES; i++)
        {
            display_sprite(screen, arraySprites[i]);
        }

        /* Échange des deux buffers */
        SDL_Flip(screen);
    }

    for (int i = 0; i < NB_SPRITES; i++)
    {
        free(spriteOldPositions[i]);
        free(arraySprites[i]);
    }

    SDL_FreeSurface(loadedPicture);
}

int main(int argc, char* argv[])
{
    SDL_Surface* screen = sdl_init();
    main_loop(screen);

    return 0;
}
