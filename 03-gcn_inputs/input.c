#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL/SDL.h>

SDL_Joystick* tableJoysticks[4];

void keyboard_event(SDL_KeyboardEvent* event)
{
    /* type et state contiennent en fait la même information */
    /* On utilise type */
    SDL_keysym* keysym = &event->keysym;
    if (event->type == SDL_KEYDOWN)
    {
        if (((keysym->unicode & 0xFF80) == 0) && (keysym->unicode > 32))
        {
            printf("Touche enfoncée : %c\n", event->keysym.unicode);
        }
        else
        {
            printf("Touche enfoncée non traduite\n");
        }

        printf("Nom SDL de la touche : %s\n", SDL_GetKeyName(keysym->sym));
    }
    else
    {
        printf("Touche relevée : %s\n", SDL_GetKeyName(keysym->sym));
    }
}

void mouse_motion_event(SDL_MouseMotionEvent* event)
{
    printf("Mouvement de la souris (%i,%i) . Coordonnées dans la fenêtres (%i,%i)",
           event->xrel, event->yrel,
           event->x, event->y);

    if (event->state & SDL_BUTTON(1))
    {
        printf(" - B1");
    }
    if (event->state & SDL_BUTTON(2))
    {
        printf(" - B2");
    }
    if (event->state & SDL_BUTTON(3))
    {
        printf(" - B3");
    }
    printf("\n");
}

void mouse_button_event(SDL_MouseButtonEvent* event)
{
    if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        printf("Bouton %i appuyé", event->button);
    }
    else
    {
        printf("Bouton %i relaché", event->button);
    }
    printf(" aux coordonnées (%i,%i)\n", event->x, event->y);
}

void init_joysticks()
{
    int numJoystick = SDL_NumJoysticks();

    printf("Nombre de joysticks detectes : %i\n", numJoystick);

    if (numJoystick > 4)
    {
        printf("Seuls les quatre premiers joysticks seront pris en compte\n");
        numJoystick = 4;
    }

    for (int index = 0; index < numJoystick; index++)
    {
        printf("Nom du joystick %i : %s\n", index, SDL_JoystickName(index));
        printf("Essai d'ouverture\n");

        tableJoysticks[index] = SDL_JoystickOpen(index);
        if (tableJoysticks[index])
        {
            printf("Nombre d'axes : %d\n", SDL_JoystickNumAxes(tableJoysticks[index]));
            printf("Nombre de boutons : %d\n", SDL_JoystickNumButtons(tableJoysticks[index]));
            printf("... joystick ouvert\n");
        }
        else
        {
            printf("... problème d'ouverture avec ce joystick\n");
        }
    }
    for (int index = numJoystick; index < 4; index++)
    {
        tableJoysticks[index] = NULL;
    }
}

void close_joysticks()
{
    for (int index = 0; index < 4; index++)
    {
        if (tableJoysticks[index])
        {
            SDL_JoystickClose(tableJoysticks[index]);
            printf("Joystick %i fermé\n", index);
        }
    }
}

void joystick_axis_event(SDL_JoyAxisEvent* event)
{
    printf("Axe %i du joystick %i : %i\n", event->axis, event->which, event->value);
}

void joystick_button_event(SDL_JoyButtonEvent* event)
{
    printf("Bouton %i du joystick %i : ", event->button, event->which);
    if (event->state == SDL_PRESSED)
    {
        printf("appuyé\n");
    }
    else
    {
        printf("relaché\n");
    }
}

void quit()
{
    close_joysticks();
    SDL_Quit();
}

SDL_Surface* sdl_init()
{
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK) < 0)
    {
        exit(1);
    }
    atexit(quit);

    SDL_Surface* screen = SDL_SetVideoMode(128, 128, 16, SDL_SWSURFACE | SDL_DOUBLEBUF);

    if (screen == NULL)
    {
        exit(2);
    }

    SDL_WM_SetCaption("Inputs", NULL);

    return screen;
}

void main_loop(SDL_Surface* screen)
{
    bool quitProgram = false;   /* Drapeau de fin de boucle */

    SDL_EnableUNICODE(1);
    init_joysticks();

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
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    keyboard_event((SDL_KeyboardEvent*) &event);
                    break;
                case SDL_MOUSEMOTION:
                    mouse_motion_event((SDL_MouseMotionEvent*) &event);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                    mouse_button_event((SDL_MouseButtonEvent*) &event);
                    break;
                case SDL_JOYAXISMOTION:
                    joystick_axis_event((SDL_JoyAxisEvent*) &event);
                    break;
                case SDL_JOYBUTTONDOWN:
                case SDL_JOYBUTTONUP:
                    joystick_button_event((SDL_JoyButtonEvent*) &event);
                    break;
                default:break;
            }
        }

        SDL_Flip(screen);
    }
}

int main(int argc, char* argv[])
{
    SDL_Surface* screen = sdl_init();
    main_loop(screen);

    return 0;
}
