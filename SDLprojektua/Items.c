#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>
#include <math.h>

//Lurrean dauden itemak gordetzeko array-a
struct Item droppedItems[128];
int playerPickRange = 50;
droppedLength = 0;

//Lurrean dauden item-en array-a Y posizioaren arabera ordenatzeko. Horrela jokalariaren atzetik eta aurretik dauden item-ak bereizi daitezke.
void ordenatuDroppedItems() {
	for (int i = 0; i < droppedLength; i++) {
		for (int j = i + 1; j < droppedLength; j++) {
			if (droppedItems[i].yPos > droppedItems[j].yPos) {
				struct Item tmp;
				tmp = droppedItems[i];
				droppedItems[i] = droppedItems[j];
				droppedItems[j] = tmp;
			}
		}
	}
	return;
}

int lastItemDrawn = 0;
//Lurreko objektuak marrazteko funtzioa. Birritan deitzen da, behin jokalariaren gainetik dauden itemak marrazteko eta berriz jokalarian aurretik daudenentzat.
//lastItemDrawn kontagailuak bi zatitan banatzen du array-a: jokalariaren gainetik daudenak eta azpitik daudenak.
void marraztuDroppedItems(int ordena) { // ordena = 0 --> Player atzetik, ordena = 1 --> Player aurretik
	if (ordena == 0) {
		lastItemDrawn = 0;
		int i = 0, finished = 0;
		while (i < droppedLength && !finished) {
			if (droppedItems[i].yPos + 64 < player.y + player.h) {
				if(player.status == droppedItems[i].status) showDroppedItem(i);
			}
			else {
				finished = 1;
			}
			lastItemDrawn = i;
			i++;
		}
	}
	else {
		int i = lastItemDrawn;
		if (droppedItems[i].yPos + 64 < player.y + player.h) i++;
		for (i; i < droppedLength; i++) {
			if (player.status == droppedItems[i].status) showDroppedItem(i);
		}
	}
	return;
}

//Lurrean dauden item-ak pantailaratzeko funtzioa.
void showDroppedItem(int index) {
	SDL_Rect clip;
	clip.x = droppedItems[index].sheetPosX;
	clip.y = droppedItems[index].sheetPosY;
	clip.w = 64;
	clip.h = 64;

	aplikatuSurface(droppedItems[index].xPos - camera.x, droppedItems[index].yPos - camera.y, 64, 64, textures[itemsSurface], &clip);
	return;
}

//Arratoiko itema lurrera botatzeko funtzioa. Arratoitik ezabatu eta dropppedItems array-ra gehitzen da. droppedLength-ek array-aren tamaina gordetzen du.
void dropHoveringItem() {
	if (hoveringItem.ID != 0 && hoveringItem.quantity > 0) {
		droppedItems[droppedLength] = hoveringItem;
		droppedItems[droppedLength].xPos = player.x;
		droppedItems[droppedLength].yPos = player.y;
		droppedItems[droppedLength].status = player.status;
		droppedLength++;
		hoveringItem.ID = 0;
	}
	return;
}

//Uzta biltzean, adibidez, erabiltzen den funtzioa tile zehatz batean x item bateko y kopuru botatzeko.
void dropItem(int tile, int item, int ammount) {
	droppedItems[droppedLength] = itemPresets[item];
	droppedItems[droppedLength].xPos = tiles[tile].x * 64;
	droppedItems[droppedLength].yPos = tiles[tile].y * 64;
	droppedItems[droppedLength].quantity = ammount;
	droppedItems[droppedLength].status = player.status;
	droppedLength++;
	return;
}

//Jokalaria item batera gerturatzean item-a jasotzeko funtzioa.
int pickUpItems() {
	int picked = 0;
	for (int i = 0; i < droppedLength; i++) {
		if (checkInRange(droppedItems[i].xPos - player.x - player.w / 2, droppedItems[i].yPos - player.y - player.h / 2, playerPickRange) && player.status == droppedItems[i].status) {
			if (!insertItem(INV_HOTBAR, droppedItems[i], droppedItems[i].quantity, -1)) {
				if (insertItem(INV_PLAYER, droppedItems[i], droppedItems[i].quantity, -1)) {
					sweepFloor(i);
				}
			}
			else {
				sweepFloor(i);
			}
			picked = 1;
			i--;
		}
	}
	return picked;
}

//droppedItems array-tik posizio zehatz bateko item-a ezabatu eta array-ko hutsune hori ezabatzeko funtzioa.
void sweepFloor(int i) {
	droppedLength--;
	for (i; i < droppedLength; i++) {
		droppedItems[i] = droppedItems[i + 1];
	}
	return;
}

//Hazi eta item ID sistemen arteko konbertsioa egiteko funtzioa.
int seedToItem(int seed) {
	int item = 0;
	switch (seed) {
	case CALABAZA: item = 4;
		break;
	case TOMATE: item = 6;
		break;
	case PIMIENTO: item = 8;
		break;
	case BERENJENA: item = 10;
		break;
	case TRIGO: item = 12;
		break;
	}
	return item;
}