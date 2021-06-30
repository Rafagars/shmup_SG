#include "scenes.h"

u16 index;

void titleScreen(){
    onTitle = TRUE;
    index += TILE_USERINDEX;
    VDP_setTextPlane(BG_A);
    VDP_setTextPalette(PAL1);
    VDP_setPalette(PAL1, title.palette->data);
    VDP_drawImageEx(BG_B, &title, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, index),0, 0, FALSE, DMA);

    index += title.tileset->numTile;

    VDP_drawText(msg_start, 20 - strlen(msg_start)/2, 23);

}

void level1(){
    onTitle = FALSE;

    VDP_setPalette(PAL1, ship.palette->data);
    VDP_setPalette(PAL2, enemy.palette->data);

    VDP_drawImageEx(BG_B, &bg_b, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, index), 0, 0, FALSE, DMA);

    index += bg_b.tileset->numTile;

    reviveCharacter(&player);
}