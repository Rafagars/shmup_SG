#include <genesis.h>
#include "functions.h"

u16 index;

int main(){

    //Input
    JOY_init();
    JOY_setEventHandler(&myJoyHandler);

    XGM_setPCM(SFX_LASER, sfx_laser, sizeof(sfx_laser));
    XGM_setPCM(SFX_EXPLOSION, sfx_explo, sizeof(sfx_explo));

    // Disable interrupts
    SYS_disableInts();

    index += TILE_USERINDEX;

    VDP_setPalette(PAL1, ship.palette->data);
    VDP_setPalette(PAL2, enemy.palette->data);

    VDP_drawImageEx(BG_B, &bg_b, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, index), 0, 0, FALSE, DMA);

    index += bg_b.tileset->numTile;
    startGame();

    SPR_init(0, 0, 0);
    player.sprite = SPR_addSprite(&ship, player.x, player.y, TILE_ATTR(PAL1, 1, FALSE, FALSE));

    //Create all obstacles sprites

    for(i = 0; i < max_enemies; i++){
        enemies[i].x = 64*i + 20;
        enemies[i].y = 20;
        enemies[i].h = 24;
        enemies[i].w = 24;
        enemies[i].health = 1;
        enemies[i].flip = FALSE;
        enemies[i].vel_x = 0;
        enemies[i].vel_y = 0;
        enemies[i].sprite = SPR_addSprite(&enemy, enemies[i].x, enemies[i].y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
        sprintf(enemies[i].name, "Enemy%d", i);
    }

    // Create bullets sprites
    for(i = 0; i < max_bullets; i++){
        bullets[i].x = 0;
        bullets[i].y = -10;
        bullets[i].w =  8;
        bullets[i].h = 8;
        bullets[i].health = 0;
        bullets[i].sprite = SPR_addSprite(&bullet, bullets[i].x, bullets[i].y, TILE_ATTR(PAL1, 1, FALSE, FALSE));
        sprintf(bullets[i].name, "Bu%d", i);
    }

    // VDP process done, we can re enable interrupts
    SYS_enableInts();

    //Plane scroll
    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);

    game_on = TRUE;

    u8 offset = 0;
    while(1){
        if(game_on == TRUE){
            VDP_setVerticalScroll(BG_B, offset -= 1);
            player.x += player.vel_x;
            player.y += player.vel_y;
            // Set player's boundaries
            if(player.x < LEFT_EDGE){
                player.x = LEFT_EDGE;
            } 
            if( player.x > RIGHT_EDGE){
                player.x = RIGHT_EDGE;
            }
            if(player.y < TOP_EDGE){
                player.y = TOP_EDGE;
            }
            if(player.y > BOTTON_EDGE){
                player.y = BOTTON_EDGE;
            }
            SPR_setPosition(player.sprite, player.x , player.y);
            positionBullet();
            positionEnemies();
            handleCollision();
            if(lives < 1){
                endGame();
            }
            SPR_update();
        }
        SYS_doVBlankProcess();
    }
    MEM_free(&player);
    return 0;
}