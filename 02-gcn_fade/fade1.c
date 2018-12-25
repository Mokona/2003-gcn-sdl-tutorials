#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL/SDL.h>

SDL_Surface* sdl_init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        exit(1);
    }
    atexit(SDL_Quit);

    SDL_Surface* screen = SDL_SetVideoMode(640, 480, 16, SDL_SWSURFACE | SDL_DOUBLEBUF);

    if (screen == NULL)
    {
        exit(2);
    }

    SDL_WM_SetCaption("Fade 1", NULL);

    return screen;
}

SDL_Surface* loadPicture()
{
    /* Surface contenant l'image */
    SDL_Surface* sdlLoadedPicture;

    /* Charge l'image qui contient les sprites */
    sdlLoadedPicture = SDL_LoadBMP("image1.bmp");
    if (sdlLoadedPicture == NULL)
    {
        exit(3);
    }
    return sdlLoadedPicture;
}

void main_loop(SDL_Surface* screen)
{
    SDL_Surface* loadedPicture = loadPicture();

    bool quitProgram = false;   /* Drapeau de fin de boucle */

    uint16_t alpha = 0;         /* Valeur d'alpha */
    int16_t delta_alpha = 3;    /* Delta d'alpha */

    /* Boucle principale */
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

        alpha += delta_alpha;
        if (alpha >= SDL_ALPHA_OPAQUE)
        {
            alpha = SDL_ALPHA_OPAQUE;
            delta_alpha = -delta_alpha;
        }
        else if (alpha <= 0)
        {
            alpha = 0;
            delta_alpha = -delta_alpha;
        }

        SDL_SetAlpha(loadedPicture, SDL_SRCALPHA, (uint8_t) alpha); /* Mise à jour de l'alpha de l'image */

        SDL_FillRect(screen, NULL, 0); /* On efface l'écran */
        SDL_BlitSurface(loadedPicture, NULL, screen, NULL); /* Affichage de l'image dans la surface */

        SDL_Flip(screen); /* Échange des deux buffers pour affichage */
    }

    // Libération des ressources de l'image
    SDL_FreeSurface(loadedPicture);
}

int main(int argc, char* argv[])
{
    SDL_Surface* screen = sdl_init();
    main_loop(screen);

    return 0;
}
