#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL/SDL_image.h>

#include "../07-gcn_sprites-4/common.h"

enum
{
    NB_SPRITES = 100
};

const char* helicopterFiles[] = {
        "data/helico_BAS_001.png", "data/helico_BAS_002.png",
        "data/helico_BAS_003.png", "data/helico_BAS_004.png",
        "data/helico_GAUCHE_001.png", "data/helico_GAUCHE_002.png",
        "data/helico_GAUCHE_003.png", "data/helico_GAUCHE_004.png",
        "data/helico_HAUT_001.png", "data/helico_HAUT_002.png",
        "data/helico_HAUT_003.png", "data/helico_HAUT_004.png",
        "data/helico_DROIT_001.png", "data/helico_DROIT_002.png",
        "data/helico_DROIT_003.png", "data/helico_DROIT_004.png"
};

typedef struct Frame_t
{
    SDL_Surface* source;
    SDL_Rect sourceRect;
    struct Frame_t* nextFrame;
} Frame;

typedef struct Anim_st
{
    Frame* firstDownFrame;
    Frame* firstLeftFrame;
    Frame* firstUpFrame;
    Frame* firstRightFrame;
} Animation;

typedef struct AnimatedSprite_t
{
    int type;
    Frame* currentFrame;
    Animation* associatedAnimation;
    int16_t destX;
    int16_t destY;
    int dispX;
    int dispY;
    int speed;
    int nextUpdateTime;

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

/* Création d'une animation */
Animation* create_animation(Frame* up, Frame* down, Frame* left, Frame* right)
{
    Animation* oneAnim;
    oneAnim = malloc(sizeof(Animation));

    oneAnim->firstDownFrame = down;
    oneAnim->firstLeftFrame = left;
    oneAnim->firstUpFrame = up;
    oneAnim->firstRightFrame = right;

    return oneAnim;
}

/* Création d'un sprite placé aléatoirement sur l'écran */
/* et ayant une vitesse donnée                          */
AnimatedSprite* create_random_animated_sprite_with_speed(Frame* firstFrame, int speed)
{
    /* Allocation dynamique de la structure du sprite */
    AnimatedSprite* oneSprite;
    oneSprite = malloc(sizeof(AnimatedSprite));

    oneSprite->currentFrame = firstFrame;
    oneSprite->destX = (int16_t) get_random(0, 608);
    oneSprite->destY = (int16_t) get_random(0, 448);
    oneSprite->speed = speed;
    oneSprite->nextUpdateTime = oneSprite->speed;
    oneSprite->dispX = get_random(-10, 10);
    oneSprite->dispY = get_random(-10, 10);
    oneSprite->type = 0;
    oneSprite->associatedAnimation = NULL;

    return oneSprite;
}

/* Création d'un sprite aléatoirement placé sur l'écran */
/* et ayant une période d'évolution aléatoire           */
AnimatedSprite* create_random_animated_sprite(Frame* firstFrame)
{
    uint16_t speed = (uint16_t) get_random(1, 1000);
    return create_random_animated_sprite_with_speed(firstFrame, speed);
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
    switch (oneSprite->type)
    {
        case 0:
            oneSprite->destX += oneSprite->dispX;
            oneSprite->destY += oneSprite->dispY;
            break;
        case 1:
            if (oneSprite->associatedAnimation == NULL)
            {
                break;
            }
            if (abs(oneSprite->dispX) > abs(oneSprite->dispY))
            {
                oneSprite->destX += oneSprite->dispX;
            }
            else
            {
                oneSprite->destY += oneSprite->dispY;
            }

            if (get_random(0, 100) < 10)
            {
                Animation* spriteAnim = oneSprite->associatedAnimation;
                oneSprite->dispX = get_random(-10, 10);
                oneSprite->dispY = get_random(-10, 10);
                if (abs(oneSprite->dispX) > abs(oneSprite->dispY))
                {
                    if (oneSprite->dispX > 0)
                    {
                        oneSprite->currentFrame = spriteAnim->firstRightFrame;
                    }
                    else
                    {
                        oneSprite->currentFrame = spriteAnim->firstLeftFrame;
                    }
                }
                else
                {
                    if (oneSprite->dispY > 0)
                    {
                        oneSprite->currentFrame = spriteAnim->firstDownFrame;
                    }
                    else
                    {
                        oneSprite->currentFrame = spriteAnim->firstUpFrame;
                    }
                }
            }
            break;
        default:
            break;
    }

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

/* Affiche le nombre de FPS à l'écran */
void display_fps(SDL_Surface* screen, AnimatedSprite** spriteOne, int FPS)
{
    int16_t destX = 620;
    int16_t destY = 450;

    while (FPS != 0)
    {
        int digit = FPS % 10;

        if (digit == 0)
        {
            digit = 10;
        }

        digit--;

        spriteOne[digit]->destX = destX;
        spriteOne[digit]->destY = destY;
        display_animated_sprite(screen, spriteOne[digit]);
        destX -= 16;

        FPS = FPS / 10;
    }
}

/* Calcul le nombre de Frames par Seconde en fonction des deux dates */
int compute_fps(int currentDate, int oldDate)
{
    int diff = currentDate - oldDate;
    if (diff != 0)
    {
        float fFPS = 1000.f / (float) diff;
        return (int) fFPS;
    }

    return 0;
}

SDL_Surface* create_digit_frames(Frame** digitFrames)
{
    SDL_Surface* loadedPicture = load_picture("data/32sprites.bmp"); /* Charge l'image qui contient les sprites */
    SDL_SetColorKey(loadedPicture, SDL_SRCCOLORKEY, 0); /* Déclare la couleur noire transparente sur cette surface */

    /* Chargement des 10 frames de l'image 32sprites.bmp */
    for (int i = 0; i < 10; i++)
    {
        /* Création d'une frame */
        digitFrames[i] = create_frame(loadedPicture, (int16_t) (i * 32), 0, 32, 32);
    }

    for (int i = 0; i < 9; i++)
    {
        /* La frame suivante est par défaut celle d'après */
        digitFrames[i]->nextFrame = digitFrames[i + 1];
    }

    /* La frame suivante de la dernière frame est la première frame */
    digitFrames[9]->nextFrame = digitFrames[0];

    return loadedPicture;
}

void main_loop(SDL_Surface* screen)
{
    Frame* digitFrames[10]; /* Tableau des frames pour les chiffres*/
    SDL_Surface* loadedDigitPicture = create_digit_frames(digitFrames);

    srand(SDL_GetTicks()); /* Initialisation du générateur pseudo aléatoire */

    SDL_Surface* loadedHelicopterPictures[16]; /* Tableau de surfaces pour les images contenant l'hélico */
    AnimatedSprite* sprites[NB_SPRITES * 2]; /* Tableau de sprites */

    /* Charge les images de l'hélico */
    for (int i = 0; i < 16; i++)
    {
        loadedHelicopterPictures[i] = IMG_Load(helicopterFiles[i]);
        if (loadedHelicopterPictures[i] == NULL)
        {
            printf("Can't load Helicopter picture number %i\n", i);
            exit(3);
        }
    }

    /* Chargement des frames de l'helico */
    Frame* helicopterFrames[16];  /* Tableau des frames pour l'helicoptère */
    for (int i = 0; i < 16; i++)
    {
        helicopterFrames[i] = create_frame(loadedHelicopterPictures[i], 0, 0, 64, 64);
    }
    for (int i = 0; i < 15; i++)
    {
        helicopterFrames[i]->nextFrame = helicopterFrames[i + 1];
    }

    helicopterFrames[3]->nextFrame = helicopterFrames[0];
    helicopterFrames[7]->nextFrame = helicopterFrames[4];
    helicopterFrames[11]->nextFrame = helicopterFrames[8];
    helicopterFrames[15]->nextFrame = helicopterFrames[12];

    /* Construction de l'animation de l'hélicoptère */
    Animation* animHelicopter = create_animation(helicopterFrames[8],   /* Up */
                                                 helicopterFrames[0],   /* Down */
                                                 helicopterFrames[4],   /* Left */
                                                 helicopterFrames[12]); /* Right */

    /* Construction des sprites avec les chiffres, chacun initialisé avec la première frame */
    for (int i = 0; i < NB_SPRITES; i++)
    {
        sprites[i] = create_random_animated_sprite(digitFrames[0]);
        sprites[i]->type = 0;
    }
    /* Construction des sprites avec les hélicoptères, chacun initialisé avec la première frame */
    for (int i = 0; i < NB_SPRITES; i++)
    {
        sprites[i + NB_SPRITES] = create_random_animated_sprite_with_speed(helicopterFrames[0], 10);
        sprites[i + NB_SPRITES]->type = 1;
        sprites[i + NB_SPRITES]->associatedAnimation = animHelicopter;
    }

    /* Tableau des sprites pour afficher des chiffres */
    AnimatedSprite* arrayDigits[10];
    for (int i = 0; i < 10; i++)
    {
        arrayDigits[i] = create_random_animated_sprite(digitFrames[i]);
    }

    int currentTime = SDL_GetTicks(); /* "Date" de référence */
    int oldTime; /* "Date" de la boucle précédente */
    int FPS; /* Calcul des Frames par Seconde */

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

        oldTime = currentTime;
        currentTime = SDL_GetTicks(); /* On prend la "date" actuelle */

        /* On en déduit les FPS */
        FPS = compute_fps(currentTime, oldTime);

        SDL_FillRect(screen, NULL, 0); /* Effacer tout l'écran */

        /* Affichage les FPS à l'écran */
        display_fps(screen, arrayDigits, FPS);

        /* Appel de la fonction de mise à jour d'un sprite */
        /* pour tout le tableau des sprites */
        for (int i = 0; i < NB_SPRITES * 2; i++)
        {
            update_animated_sprite(sprites[i], currentTime);
        }

        /* Affichage de tous les sprites */
        for (int i = 0; i < NB_SPRITES * 2; i++)
        {
            display_animated_sprite(screen, sprites[i]);
        }

        /* Échange des deux buffers */
        SDL_Flip(screen);
    }

    /* Libération de la mémoire */
    for (int i = 0; i < NB_SPRITES * 2; i++)
    {
        free(sprites[i]);
    }
    for (int i = 0; i < 10; i++)
    {
        free(digitFrames[i]);
        free(arrayDigits[i]);
    }
    for (int i = 0; i < 16; i++)
    {
        free(helicopterFrames[i]);
        SDL_FreeSurface(loadedHelicopterPictures[i]);
    }
    free(animHelicopter);

    SDL_FreeSurface(loadedDigitPicture);
}

int main(int argc, char* argv[])
{
    SDL_Surface* screen = sdl_init();
    main_loop(screen);

    return 0;
}