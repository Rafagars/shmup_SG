#include "functions.h"

u8 i;
const int scrollsped = 2;
bool onTitle;

//HUD
u16 score = 0;
char label_score[6] = "SCORE\0";
char str_score[6] = "0";
u8 lives = 3;
char label_lives[6] = "LIVES\0";
char str_lives[3] = "X3";

// Constants for the game
const u16 max_speed = 4;
const u8 max_enemies = 5;
const u8 max_bullets = 3;

u8 bulletOnScreen = 0;

//Screen messages for the player
const char msg_start[22] = "Press START to begin";
const char msg_reset[22] = "GAME OVER\tPress START";
const char msg_pause[6] = "PAUSED";
bool game_on = FALSE;

void showText(char s[]){
    VDP_drawText(s, 20 - strlen(s)/2, 12);
};

void clearText(){
    VDP_clearText(0, 12, 32);
}

void startGame(){
    if(game_on == FALSE){
        game_on = TRUE;
        clearText();
        VDP_drawText(label_score, 1, 0);
        VDP_drawText(str_score, 1, 1);
        VDP_drawText(label_lives, 32, 0);
        VDP_drawText(str_lives, 35, 1);
        updateScoreDisplay();
        updateLivesDisplay();
    }

}

void pauseGame(){
    if(game_on == TRUE){
        showText(msg_pause);
        game_on = FALSE;
    }
}

void endGame(){
    if(game_on == TRUE){
        showText(msg_reset);
        game_on = FALSE;
    }
    score = 0;
    lives = 3;
}

void myJoyHandler(u16 joy, u16 changed, u16 state){
    if(joy == JOY_1){
        if(game_on == TRUE){
            if(state & BUTTON_LEFT){
                player.vel_x = -max_speed;
            }
            else if(state & BUTTON_RIGHT){
                player.vel_x = max_speed;
            }
            else if(state & BUTTON_UP){
                player.vel_y = -max_speed;
            }
            else if(state & BUTTON_DOWN){
                player.vel_y = max_speed;
            }
            else {
                player.vel_x = 0;
                player.vel_y = 0;
            }
            if(state & BUTTON_A & changed){
                shootBullet();
            }
            if(state & BUTTON_START){
                pauseGame();
            }
        } else{
            if(state & BUTTON_START){
                startGame();
                if(onTitle == TRUE){
                    VDP_clearText(0, 23, 32);
                    level1();
                }
            }
        }
    }
}

void updateScoreDisplay(){
    sprintf(str_score, "%d", score);
    VDP_clearText(0, 1, 6);
    VDP_drawText(str_score, 1, 1);
}

void updateLivesDisplay(){
    sprintf(str_lives, "X%d", lives);
    VDP_clearText(35, 1, 3);
    VDP_drawText(str_lives, 35, 1);
}

void positionEnemies(){
    for(i = 0; i < max_enemies; i++){
        if(enemies[i].flip == FALSE){
            enemies[i].vel_x = 2;
        } else {
            enemies[i].vel_x = -2;
        }
        enemies[i].vel_y = 0;
       if(enemies[i].health > 0){
            if(enemies[i].x > 320){
                enemies[i].flip = TRUE;
                enemies[i].vel_y = 8; 
            } else if(enemies[i].x < 0){
                enemies[i].flip = FALSE;
                enemies[i].vel_y = 8;
            }
       } else {
           enemies[i].x = 64*i + 20;
           enemies[i].y = -20;
           reviveCharacter(&enemies[i]);
       }
        enemies[i].x += enemies[i].vel_x;
        enemies[i].y += enemies[i].vel_y;
        if(checkCollision(&player, &enemies[i]) == TRUE){
            player.x = 120;
            player.y = 160;
            lives--;
            updateLivesDisplay();
        }
        SPR_setPosition(enemies[i].sprite, enemies[i].x, enemies[i].y);
    }
}

void handleCollision(){
    for(i = 0; i < max_bullets; i++){
        if(bullets[i].health > 0){
            for(u8 j = 0; j < max_enemies; j++){
                if(enemies[j].health > 0){
                    if(checkCollision(&bullets[i], &enemies[j])){
                        killCharacter(&enemies[j]);
                        killCharacter(&bullets[i]);

                        XGM_startPlayPCM(SFX_EXPLOSION, 1, SOUND_PCM_CH2);
                        //enemiesOnScreen--;
                        bulletOnScreen--;

                        score += 10;
                        updateScoreDisplay();

                        break;
                    }
                }
            }
        }
    }
}

bool checkCollision(GameCharacter* one, GameCharacter* two){
    return((one->x >= two->x && one->x <= two->x + two->w) && (one->y >= two->y && one->y <= two->y + two->h)) || ((two->x >= one->x && two->x <= one->x + one->w) && (two->y >= one->y && two->y <= one->y + one->h));
}

void killCharacter(GameCharacter* en){
    en->health = 0;
    SPR_setVisibility(en->sprite, HIDDEN);
}

void reviveCharacter(GameCharacter* en){
    en->health = 1;
    SPR_setVisibility(en->sprite, VISIBLE);
}

void shootBullet(){
    if(bulletOnScreen < max_bullets){
        for(i = 0; i < max_bullets; i++){
            if(bullets[i].health == 0){
                bullets[i].x = player.x + 8;
                bullets[i].y = player.y;
                reviveCharacter(&bullets[i]);
                bullets[i].vel_y = -3;

                SPR_setPosition(bullets[i].sprite, bullets[i].x, bullets[i].y);
                XGM_startPlayPCM(SFX_LASER, 1, SOUND_PCM_CH2);
                bulletOnScreen++;
                break;
            }
        }
    }
}

void positionBullet(){
    for(i = 0; i < max_bullets; i++){
        if(bullets[i].health > 0){
            bullets[i].y += bullets[i].vel_y;
            if(bullets[i].y + bullets[i].h < 0){
                killCharacter(&bullets[i]);
                bulletOnScreen--;
            } else {
                SPR_setPosition(bullets[i].sprite, bullets[i].x, bullets[i].y);
            }
        }
    }
}

u16 randomize(u16 n){
    u16 x;
    
    do{ x = random(); }
    while(x >= n);

    return x;
}