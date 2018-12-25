/* Inclusion des entêtes nécessaires */
#include <stdlib.h>     /* Librairie standard */
#include <stdbool.h>    /* Pour utiliser les booléens */
#include <SDL/SDL.h>    /* Pour utiliser SDL */

/* Le buffer de positions de la souris */
enum
{
    MAX_MOUSE_POS = 20
};

typedef struct MousePositions_t
{
    uint16_t mouse_x[MAX_MOUSE_POS];
    uint16_t mouse_y[MAX_MOUSE_POS];
    uint16_t mouse_pos;
    uint16_t size;
} MousePositions;

MousePositions mouse_positions = { {0}, {0}, 0, MAX_MOUSE_POS};

/* Fonction d'affichage principale */
void draw(SDL_Surface* screen, const MousePositions* mouse_positions)
{
    /* Définition de la couleur noire afin d'effacer l'écran*/
    uint32_t color_black = SDL_MapRGB(screen->format, 0, 0, 0);
    SDL_FillRect(screen, NULL, color_black);

    /* Définition de la couleur bleu pour afficher les carrés */
    uint32_t color_blue = SDL_MapRGB(screen->format, 0, 0, 255);

    /* Affichage de tous les carrés */
    for (uint16_t index = 0; index < mouse_positions->size; index++)
    {
        /* Description du carré : 5 de côté, à la position indiquée par le buffer */
        SDL_Rect rect = {
                mouse_positions->mouse_x[index], // x
                mouse_positions->mouse_y[index], // y
                5, //w
                5  // h
        };
        SDL_FillRect(screen, &rect, color_blue);
    }

    /* On utilise du double buffer, on change donc de buffer afin d'afficher */
    SDL_Flip(screen);
}

void main_loop(SDL_Surface* screen)
{
    /* Boucle principale */
    bool quitProgram = false;
    while (!quitProgram)
    {
        SDL_Event event;

        /* Lecture des évènements dans la queue d'évènements */
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                /* Evenement de fermeture d'application, on sort la boucle */
                case SDL_QUIT:
                    quitProgram = true;
                    break;
                    /* Evenemnt de mouvement de la souris, on enregistre la nouvelle position */
                case SDL_MOUSEMOTION:
                    mouse_positions.mouse_x[mouse_positions.mouse_pos] = ((SDL_MouseMotionEvent*) &event)->x;
                    mouse_positions.mouse_y[mouse_positions.mouse_pos] = ((SDL_MouseMotionEvent*) &event)->y;
                    mouse_positions.mouse_pos++;
                    if (mouse_positions.mouse_pos == mouse_positions.size)
                    {
                        mouse_positions.mouse_pos = 0;
                    }
                    break;
                default:
                    break;
            }
        }

        /* Affichage à l'écran */
        draw(screen, &mouse_positions);
    }
}

SDL_Surface* sdl_init()
{
    /* Initialisation de la partie Vidéo de SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        /* En cas d'échec, sortie du programme */
        exit(1);
    }

    /* Lorsque le programme se termine, il ferme SDL correctement */
    atexit(SDL_Quit);

    /* Demande d'affichage en 640x480 16 bits */
    /* SDL_SWSURFACE : la surface est crée en RAM système */
    /* SDL_DOUBLEBUF : on utilise deux buffers */
    /* screen est le pointeur sur la surface représentant l'écran */
    SDL_Surface* screen = SDL_SetVideoMode(640, 480, 16, SDL_SWSURFACE | SDL_DOUBLEBUF);

    /* En cas d'echec, on sort */
    if (screen == NULL)
    {
        exit(2);
    }

    /* Titre de la fenêtre */
    SDL_WM_SetCaption("Serpent", NULL);

    return screen;
}

int main(int argc, char** argv)
{
    SDL_Surface* screen = sdl_init();
    main_loop(screen);

    return 0;
}

