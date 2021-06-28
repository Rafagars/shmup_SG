#ifndef _GAMECHARACTER_H
#define _GAMECHARACTER_H

#include <genesis.h>

typedef struct {
    s16 x;
    s16 y;
    s16 vel_x;
    s16 vel_y;
    u8 w;
    u8 h;
    u8 health;
    bool flip;
    Sprite* sprite;
    char name[6];
} GameCharacter;

extern GameCharacter player;

extern GameCharacter enemies[5];

extern GameCharacter bullets[3];

#endif