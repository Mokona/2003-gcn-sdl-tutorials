#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "common.h"

enum
{
    NB_SPRITES = 10
};

typedef struct Sprite_t
{
    SDL_Surface* source;
    int nextUpdateTime;
    int16_t destX;
    int16_t destY;
    uint16_t speed;
    uint8_t step;
} Sprite;

int get_random(int first, int last)
{
    int range = last - first;
    float random = (float) rand();

    random = (random / (float) RAND_MAX) * (float) range;
    random += first;

    return (int) random;
}

/* Création d'un sprite aléatoirement placé sur l'écran, */
/* et ayant une période d'évolution aléatoire            */
Sprite* create_random_sprite(SDL_Surface* source)
{
    /* Allocation dynamique de la structure du sprite */
    Sprite* oneSprite = malloc(sizeof(Sprite));

    oneSprite->source = source;
    oneSprite->nextUpdateTime = oneSprite->speed;
    oneSprite->destX = (int16_t) get_random(0, 608);
    oneSprite->destY = (int16_t) get_random(0, 448);
    oneSprite->speed = (uint16_t) get_random(1, 1000);
    oneSprite->step = 0;

    return oneSprite;
}

/* Affiche d'un sprite a partir de sa structure */
void display_sprite(SDL_Surface* screen, Sprite* oneSprite)
{
    /* Rectangle décrivant le sprite actuel à afficher */
    SDL_Rect source_rect = {
            (Sint16) (oneSprite->step * 32),
            0,
            32,
            32 };

    /* Coordonnées à l'écran */
    SDL_Rect dest_rect = {
            oneSprite->destX,
            oneSprite->destY
    };

    /* On recopie le rectangle du sprite sur le buffer d'affichage */
    SDL_BlitSurface(oneSprite->source, &source_rect, screen, &dest_rect);
}

/* Affiche d'un sprite a partir de sa structure */
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

        /* Appel de la fonction d'affichage et de mise à jour d'un sprite */
        /* pour tout le tableau des sprites */
        for (int i = 0; i < NB_SPRITES; i++)
        {
            update_sprite(arraySprites[i], NULL, currentTime);
            display_sprite(screen, arraySprites[i]);
        }

        /* Echange des deux buffers */
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
