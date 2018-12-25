#include "sprite.h"
#include "common.h"

/* Création d'un sprite aléatoirement placé sur l'écran, */
/* et ayant une période d'évolution aléatoire            */
Sprite* create_random_sprite(SDL_Surface* source)
{
    /* Allocation dynamique de la structure du sprite */
    Sprite* oneSprite = malloc(sizeof(Sprite));

    oneSprite->source = source;
    oneSprite->step = 0;
    oneSprite->destX = (int16_t) get_random(0, 608);
    oneSprite->destY = (int16_t) get_random(0, 448);
    oneSprite->speed = (uint16_t) get_random(1, 1000);
    oneSprite->nextUpdateTime = oneSprite->speed;
    oneSprite->dispX = get_random(-10, 10);
    oneSprite->dispY = get_random(-10, 10);

    return oneSprite;
}

/* Affiche un sprite à partir de sa structure */
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
