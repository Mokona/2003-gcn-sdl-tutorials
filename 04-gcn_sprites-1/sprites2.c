#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "common.h"

void main_loop(SDL_Surface* screen)
{
    uint8_t displayedSpriteIndex = 0; /* Numéro du sprite à afficher */

    SDL_Surface* loadedPicture = load_picture("32sprites.bmp"); /* Charge l'image qui contient les sprites */
    SDL_Rect source_rect = { 0, 0, 32, 32 }; /* Rectangle décrivant le sprite actuel à afficher */

    int nextUpdateTime = SDL_GetTicks(); /* "Date" à laquelle on changera de sprite */

    /* Boucle principale */
    bool quitProgram = false;   /* Drapeau de fin de boucle */
    while (!quitProgram)
    {
        SDL_Event event;

        /* Lecture des évènements dans la queue d'évènements */
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                /* Evenement de fermture d'application, on sort la boucle */
                case SDL_QUIT:
                    quitProgram = true;
                    break;
                default:break;
            }
        }

        /* On va chercher la coordonnée du sprite à afficher dans l'image */
        source_rect.x = (Sint16) (displayedSpriteIndex * 32);

        /* On recopie le rectangle du sprite sur le buffer d'affichage */
        SDL_BlitSurface(loadedPicture, &source_rect, screen, NULL);

        /* On met à jour la "date" actuelle */
        int currentTime = SDL_GetTicks();

        /* Si on a dépassé la "date" de mise à jour du sprite, on passe */
        /* à l'étape suivante de l'animation */
        if (currentTime > nextUpdateTime)
        {
            /* On sélectionne le sprite suivant */
            displayedSpriteIndex++;
            if (displayedSpriteIndex > 9)
            {
                displayedSpriteIndex = 0;
            }

            /* On fixe la date du prochain changement : une demi seconde après */
            nextUpdateTime = currentTime + 500;
        }

        SDL_Flip(screen);
    }

    SDL_FreeSurface(loadedPicture);
}

int main(int argc, char* argv[])
{
    SDL_Surface* screen = sdl_init();
    main_loop(screen);

    return 0;
}
