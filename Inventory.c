#include "SDL.h"
#include "SDL_image.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>

struct Item empty = {
	0, "Empty", 128, 0
};

struct Item calabaza = {
	1, "Calabaza", 128, 64
};

struct Item tomate = {
	2, "Tomate", 128, 128
};

void marraztuInv(SDL_Surface* spriteSheetSurface, SDL_Surface* screenSurface){
	SDL_Rect clip;
	clip.w = inventory.slotSize;
	clip.h = inventory.slotSize;
	int slots = inventory.cols * inventory.rows;
	for (int i = 0; i < slots; i++) {
		clip.x = inventory.items[i].sheetPosX;
		clip.y = inventory.items[i].sheetPosY;
		aplikatuSurface(i % inventory.cols * inventory.slotSize + inventory.xPos, i / inventory.cols * inventory.slotSize + inventory.yPos, spriteSheetSurface, screenSurface, &clip);
	}
}

struct Inventory updateInv() {
	int slots = inventory.cols * inventory.rows;
	for (int i = 0; i < slots; i++) {
		inventory.items[i] = empty;
	}
	return inventory;
}

void changeInv(int InvPos) {
	if (tiles[player.facingTile].plant.seed == CALABAZA) {
		inventory.items[InvPos] = calabaza;
	}
	else if (tiles[player.facingTile].plant.seed == TOMATE) {
		inventory.items[InvPos] = tomate;
	}
}

void checkHover(struct posCoord mousePos){
	int found = 0, slots = inventory.cols * inventory.rows, i = 0;
	while (!found && i < slots) {
		if (mousePos.x > i % inventory.cols * inventory.slotSize
			&& mousePos.x < (i % inventory.cols + 1) * inventory.slotSize
			&& mousePos.y > i / inventory.cols * inventory.slotSize
			&& mousePos.y < (i / inventory.cols + 1) * inventory.slotSize) {
			//showTag

		}
		i++;
	}
}