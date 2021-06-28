#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include <genesis.h>
#include <maths.h>
#include <resources.h>
#include <string.h>
#include "gameCharacter.h"

#define LEFT_EDGE  0
#define RIGHT_EDGE 300
#define TOP_EDGE 10
#define BOTTON_EDGE 200

#define SFX_LASER  64
#define SFX_EXPLOSION 65

extern u8 i;
extern const u8 scrollspeed;

extern u16 score;
extern char label_score[6];
extern char str_score[6];
extern u8 lives;
extern char label_lives[6];
extern char str_lives[3];

extern const u16 max_speed;
extern const u8 max_enemies;
extern const u8 max_bullets;

extern u8 bulletOnScreen;

extern const char msg_start[22];
extern const char msg_reset[22];
extern const char msg_pause[6];
extern bool game_on;

extern void showText(char s[]);
extern void clearText();

extern void startGame();
extern void pauseGame();
extern void endGame();

extern void myJoyHandler(u16 joy, u16 changed, u16 state);

extern void moveEnemies();

extern void updateScoreDisplay();

extern void updateLivesDisplay();

extern void positionEnemies();

extern void handleCollision();

extern bool checkCollision(GameCharacter* one, GameCharacter* two);

extern void killCharacter(GameCharacter* en);

extern void reviveCharacter(GameCharacter* en);

extern void shootBullet();

extern void positionBullet();

extern u16 randomize(u16 n);

#endif