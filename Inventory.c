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



void marraztuInv(struct Inventory inventory, SDL_Surface* spriteSheetSurface, SDL_Surface* screenSurface){
	SDL_Rect clip;
	clip.w = inventory.slotSize;
	clip.h = inventory.slotSize;
	int slots = inventory.cols * inventory.rows;
	for (int i = 0; i < slots; i++) {
		clip.x = inventory.items[i].sheetPosX;
		clip.y = inventory.items[i].sheetPosY;
		aplikatuSurface(i % inventory.cols * inventory.slotSize, i / inventory.cols * inventory.slotSize, spriteSheetSurface, screenSurface, &clip);
	}
}

struct Inventory updateInv(struct Inventory inventory) {
	int slots = inventory.cols * inventory.rows;
	for (int i = 0; i < slots; i++) {
		inventory.items[i] = empty;
	}
	return inventory;
}

void changeInv(struct Inventory inventory, int InvPos) {

	if (tiles[player.facingTile].plant.seed == CALABAZA) {
		inventory.items[InvPos] = calabaza;
	}
	else if (tiles[player.facingTile].plant.seed == TOMATE) {
		inventory.items[InvPos] = tomate;
	}

	return inventory;
}