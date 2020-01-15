#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

shopOk = 0;
int prices[] = {1,2,3,4,5,6};

void shop() {
	SDL_Rect seller = { 121, 0, 48,48 };
	aplikatuSurface(500 - camera.x, 50 - camera.y, 80, 80, textures[obstacleSurface], &seller);

	if (inventories[INV_SHOP].open) {
		unlockShopItem();
		currency();
	}
}

void currency() {
	inventories[INV_SHOP].currency[0].ID = itemPresets[4].ID;
	inventories[INV_SHOP].currency[1].ID = itemPresets[4].ID;
	inventories[INV_SHOP].currency[2].ID = itemPresets[6].ID;
	inventories[INV_SHOP].currency[3].ID = itemPresets[8].ID;
	inventories[INV_SHOP].currency[4].ID = itemPresets[10].ID;
}

void unlockShopItem() {
	if (shopOk >= 1) inventories[INV_SHOP].items[0] = itemPresets[5];
	if (shopOk >= 5) inventories[INV_SHOP].items[1] = itemPresets[7];
	if (shopOk >= 7) inventories[INV_SHOP].items[2] = itemPresets[9];
	if (shopOk >= 11) inventories[INV_SHOP].items[3] = itemPresets[11];
	if (shopOk >= 12) inventories[INV_SHOP].items[4] = itemPresets[13];
	if (shopOk >= 15) inventories[INV_SHOP].items[5] = itemPresets[16];
}

void buyItem(int pos) {

	if (pos < 5) {
		if (checkHowManyOfItem(inventories[INV_SHOP].currency[pos].ID) >= prices[pos])
		{
			removeCertainItem(inventories[INV_SHOP].currency[pos].ID, prices[pos]);
			marraztuHoveringItem();
			
			prices[pos]++;
			if (!insertItem(INV_HOTBAR, inventories[INV_SHOP].items[pos], 2, -1)) {
				if (!insertItem(INV_HOTBAR, inventories[INV_SHOP].items[pos], 2, -1)){
					dropItem(player.facingTile, inventories[INV_SHOP].items[pos].ID, 2);
				}
			}
		}
	}
}
