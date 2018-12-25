#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../07-gcn_sprites-4/common.h"

enum
{
    NB_SPRITES = 100
};

typedef struct Frame_t
{
    SDL_Surface* source;
    SDL_Rect sourceRect;
    struct Frame_t* nextFrame;
} Frame;

typedef struct AnimatedSprite_t
{
    Frame* currentFrame;
    int nextUpdateTime;
    int16_t destX;
    int16_t destY;
    int dispX;
    int dispY;
    uint16_t speed;
} AnimatedSprite;


/* Création d'une frame d'animation */
Frame* create_frame(SDL_Surface* sourceSurface, int16_t sourceX, int16_t sourceY, uint16_t width, uint16_t height)
{
    Frame* oneFrame;
    oneFrame = malloc(sizeof(Frame));
    oneFrame->source = sourceSurface;
    oneFrame->sourceRect.x = sourceX;
    oneFrame->sourceRect.y = sourceY;
    oneFrame->sourceRect.w = width;
    oneFrame->sourceRect.h = height;
    oneFrame->nextFrame = NULL;

    return oneFrame;
}

/* Création d'un sprite placé aléatoirement sur l'écran, */
/* et ayant une période d'évolution aléatoire            */
AnimatedSprite* create_random_animated_sprite(Frame* firstFrame)
{
    /* Allocation dynamique de la structure du sprite */
    AnimatedSprite* oneSprite;
    oneSprite = malloc(sizeof(AnimatedSprite));

    oneSprite->currentFrame = firstFrame;
    oneSprite->destX = (int16_t) get_random(0, 608);
    oneSprite->destY = (int16_t) get_random(0, 448);
    oneSprite->speed = (uint16_t) get_random(1, 1000);
    oneSprite->nextUpdateTime = oneSprite->speed;
    oneSprite->dispX = get_random(-10, 10);
    oneSprite->dispY = get_random(-10, 10);

    return oneSprite;
}

void bounce(AnimatedSprite* oneSprite)
{
    /* Rebondi sur les bords */
    if (oneSprite->destX < 0)
    {
        oneSprite->destX = 0;
        oneSprite->dispX = get_random(1, 10);
    }
    if (oneSprite->destX > 639 - 32)
    {
        oneSprite->destX = 639 - 32;
        oneSprite->dispX = get_random(-10, -1);
    }
    if (oneSprite->destY < 0)
    {
        oneSprite->destY = 0;
        oneSprite->dispY = get_random(1, 10);
    }
    if (oneSprite->destY > 479 - 32)
    {
        oneSprite->destY = 479 - 32;
        oneSprite->dispY = get_random(-10, -1);
    }
}

/* Met à jour un sprite à partir de sa structure */
void update_animated_sprite(AnimatedSprite* oneSprite, int currentTime)
{
    /* Si on a dépassé la "date" de mise à jour du sprite, on passe */
    /* à l'étape suivante de l'animation */
    if (currentTime > oneSprite->nextUpdateTime)
    {
        /* On sélectionne le sprite suivant */
        oneSprite->currentFrame = oneSprite->currentFrame->nextFrame;

        /* On fixe la date du prochain changement */
        /* en fonction de la période du sprite    */
        oneSprite->nextUpdateTime = currentTime + oneSprite->speed;
    }

    /* Mise à jour de la position */
    oneSprite->destX += oneSprite->dispX;
    oneSprite->destY += oneSprite->dispY;

    bounce(oneSprite);
}

/* Affiche un sprite à partir de sa structure */
void display_animated_sprite(SDL_Surface* screen, AnimatedSprite* oneSprite)
{
    /* Pointeur sur la frame à afficher (pour faciliter la syntaxe) */
    Frame* frameToDisplay;

    /* Rectangle contenant les coordonnées du sprite à l'écran */
    SDL_Rect sdlDstSprite;

    /* La frame à afficher */
    frameToDisplay = oneSprite->currentFrame;

    /* Coordonnées à l'écran de la nouvelle position */
    sdlDstSprite.x = oneSprite->destX;
    sdlDstSprite.y = oneSprite->destY;

    /* On recopie le rectangle du sprite sur le buffer d'affichage */
    SDL_BlitSurface(frameToDisplay->source, &frameToDisplay->sourceRect,
                    screen, &sdlDstSprite);
}

void main_loop(SDL_Surface* screen)
{
    SDL_Surface* loadedPicture = load_picture("data/32sprites.bmp"); /* Charge l'image qui contient les sprites */
    SDL_SetColorKey(loadedPicture, SDL_SRCCOLORKEY, 0); /* Déclare la couleur noire transparente sur cette surface */

    srand(SDL_GetTicks()); /* Initialisation du générateur pseudo aléatoire */

    AnimatedSprite* arraySprites[NB_SPRITES]; /* Tableau de sprites */
    Frame* arrayFrames[10]; /* Tableau des frames */

    /* Chargement des 10 frames de l'image 32sprites.bmp */
    for (int i = 0; i < 10; i++)
    {
        /* Création d'une frame */
        arrayFrames[i] = create_frame(loadedPicture, (int16_t) (i * 32), 0, 32, 32);
    }

    for (int i = 0; i < 9; i++)
    {
        /* La frame suivante est par défaut celle d'après */
        arrayFrames[i]->nextFrame = arrayFrames[i + 1];
    }

    /* La frame suivante de la dernière frame est la première frame */
    arrayFrames[9]->nextFrame = arrayFrames[0];

    /* Construction des sprites, chacun initialisé avec la première frame */
    for (int i = 0; i < NB_SPRITES; i++)
    {
        arraySprites[i] = create_random_animated_sprite(arrayFrames[0]);
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

        SDL_FillRect(screen, NULL, 0); /* Effacer tout l'écran */

        /* Appel de la fonction de mise à jour d'un sprite */
        /* pour tout le tableau des sprites */
        for (int i = 0; i < NB_SPRITES; i++)
        {
            update_animated_sprite(arraySprites[i], currentTime);
        }

        /* Affichage de tous les sprites */
        for (int i = 0; i < NB_SPRITES; i++)
        {
            display_animated_sprite(screen, arraySprites[i]);
        }

        /* Échange des deux buffers */
        SDL_Flip(screen);
    }

    for (int i = 0; i < NB_SPRITES; i++)
    {
        free(arraySprites[i]);
    }
    for (int i = 0; i < 10; i++)
    {
        free(arrayFrames[i]);
    }

    SDL_FreeSurface(loadedPicture);
}

int main(int argc, char* argv[])
{
    SDL_Surface* screen = sdl_init();
    main_loop(screen);

    return 0;
}