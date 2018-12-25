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

    SDL_WM_SetCaption("Fade 2", NULL);

    return screen;
}

SDL_Surface* load_picture(const char* filename)
{
    /* Surface contenant l'image */
    SDL_Surface* sdlLoadedPicture;

    /* Charge l'image qui contient les sprites */
    sdlLoadedPicture = SDL_LoadBMP(filename);
    if (sdlLoadedPicture == NULL)
    {
        exit(3);
    }
    return sdlLoadedPicture;

}

void main_loop(SDL_Surface* screen)
{
    /* Surfaces contenant les deux images */
    SDL_Surface* loadedPicture1 = load_picture("image1.bmp");
    SDL_Surface* loadedPicture2 = load_picture("image2.bmp");

    bool quitProgram = false;   /* Drapeau de fin de boucle */

    uint16_t alpha = 0;         /* Valeur d'alpha */
    int16_t delta_alpha = 3;    /* Delta d'alpha */

    /* Boucle principale */
    while (!quitProgram)
    {
        SDL_Event event;

        SDL_Delay(10);
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

        SDL_BlitSurface(loadedPicture1, NULL, screen, NULL);
        SDL_SetAlpha(loadedPicture2, SDL_SRCALPHA, (uint8_t) alpha);
        SDL_BlitSurface(loadedPicture2, NULL, screen, NULL);

        /* Echange des deux buffers */
        SDL_Flip(screen);
    }

    SDL_FreeSurface(loadedPicture2);
    SDL_FreeSurface(loadedPicture1);
}

int main(int argc, char* argv[])
{
    SDL_Surface* screen = sdl_init();
    main_loop(screen);

    return 0;
}
