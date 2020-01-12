#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>
#include <math.h>

struct Item droppedItems[128];
int playerPickRange = 50;
droppedLength = 0;

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
void marraztuDroppedItems(int ordena) { // ordena = 0 --> Player atzetik, ordena = 1 --> Player aurretik
	if (ordena == 0) {
		lastItemDrawn = 0;
		int i = 0, finished = 0;
		while (i < droppedLength && !finished) {
			if (droppedItems[i].yPos + 64 < player.y + player.h) {
				showDroppedItem(i);
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
			showDroppedItem(i);
		}
	}
	return;
}

void showDroppedItem(int index) {
	SDL_Rect clip;
	clip.x = droppedItems[index].sheetPosX;
	clip.y = droppedItems[index].sheetPosY;
	clip.w = 64;
	clip.h = 64;

	aplikatuSurface(droppedItems[index].xPos - camera.x, droppedItems[index].yPos - camera.y, 64, 64, textures[itemsSurface], &clip);
	return;
}

void dropHoveringItem() {
	if (hoveringItem.ID != 0 && hoveringItem.quantity > 0) {
		droppedItems[droppedLength] = hoveringItem;
		droppedItems[droppedLength].xPos = player.x;
		droppedItems[droppedLength].yPos = player.y;
		droppedLength++;
		hoveringItem.ID = 0;
	}
	return;
}

void dropItem(int tile, int item, int ammount) {
	droppedItems[droppedLength] = itemPresets[item];
	droppedItems[droppedLength].xPos = tiles[tile].x * 64;
	droppedItems[droppedLength].yPos = tiles[tile].y * 64;
	droppedItems[droppedLength].quantity = ammount;
	droppedLength++;
	return;
}

int pickUpItems() {
	int picked = 0;
	for (int i = 0; i < droppedLength; i++) {
		if (checkInRange(droppedItems[i].xPos - player.x - player.w / 2, droppedItems[i].yPos - player.y - player.h / 2, playerPickRange)) {
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

void sweepFloor(int i) {
	droppedLength--;
	for (i; i < droppedLength; i++) {
		droppedItems[i] = droppedItems[i + 1];
	}
	return;
}

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