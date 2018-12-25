#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "common.h"

void main_loop(SDL_Surface* screen)
{
    int8_t displayedSpriteIndex = 0; /* Numéro du sprite à afficher */

    SDL_Surface* loadedPicture = load_picture("32sprites.bmp"); /* Charge l'image qui contient les sprites */

    SDL_Rect source_rect = { 0, 0, 32, 32 }; /* Rectangle décrivant le sprite actuel à afficher */

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
                default:break;
            }
        }

        /* On va chercher la coordonnée du sprite à afficher dans l'image */
        source_rect.x = (int16_t) (displayedSpriteIndex * 32);

        /* On recopie le rectangle du sprite sur le buffer d'affichage */
        SDL_BlitSurface(loadedPicture, &source_rect, screen, NULL);

        /* On sélectionne le sprite suivant */
        displayedSpriteIndex++;
        if (displayedSpriteIndex > 9)
        {
            displayedSpriteIndex = 0;
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
