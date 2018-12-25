#include "common.h"

SDL_Surface* sdl_init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
    {
        exit(1);
    }
    atexit(SDL_Quit);

    SDL_Surface* screen = SDL_SetVideoMode(640, 480, 16, SDL_SWSURFACE | SDL_DOUBLEBUF);

    if (screen == NULL)
    {
        exit(2);
    }

    SDL_WM_SetCaption("Sprites", NULL);

    return screen;
}

SDL_Surface* load_picture(const char* filename)
{
    SDL_Surface* loadedPicture;

    loadedPicture = SDL_LoadBMP(filename);
    if (loadedPicture == NULL)
    {
        exit(3);
    }
    return loadedPicture;
}

int get_random(int first, int last)
{
    int range = last - first;
    float random = (float) rand();

    random = (random / (float) RAND_MAX) * (float) range;
    random += first;

    return (int) random;
}
