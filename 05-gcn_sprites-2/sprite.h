#include <SDL/SDL.h>

typedef struct Sprite_t
{
    SDL_Surface* source;
    int nextUpdateTime;
    uint8_t step;
    int16_t destX;
    int16_t destY;
    uint16_t speed;
    int dispX;
    int dispY;
} Sprite;

Sprite* create_random_sprite(SDL_Surface* source);
void display_sprite(SDL_Surface* screen, Sprite* oneSprite);
