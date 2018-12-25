#include "common.h"

SDL_Surface* sdl_init()
{
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0)
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
