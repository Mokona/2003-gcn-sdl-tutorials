#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "common.h"
#include "sprite.h"

enum
{
    NB_SPRITES = 10
};

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

void main_loop(SDL_Surface* screen)
{
    SDL_Surface* loadedPicture = load_picture("32sprites.bmp"); /* Charge l'image qui contient les sprites */

    srand(SDL_GetTicks()); /* Initialisation du générateur pseudo aléatoire */

    /* Tableau de sprites */
    Sprite* arraySprites[NB_SPRITES];
    for (int i = 0; i < NB_SPRITES; i++)
    {
        arraySprites[i] = create_random_sprite(loadedPicture);
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

        /* Effacer tout l'écran */
        SDL_FillRect(screen, NULL, 0);

        /* Appel de la fonction d'affichage et de mise à jour d'un sprite */
        /* pour tout le tableau des sprites */
        for (int i = 0; i < NB_SPRITES; i++)
        {
            update_sprite(arraySprites[i], NULL, currentTime);
            display_sprite(screen, arraySprites[i]);
        }

        /* Échange des deux buffers */
        SDL_Flip(screen);
    }

    for (int i = 0; i < NB_SPRITES; i++)
    {
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
