#include "SDL.h"
#include "SDL_image.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>

struct Item seedCalabaza = {
	0, "Seed Calabaza", 0, 0
};

struct Item seedTomate = {
	1, "Seed Tomate", 0, 64
};

struct Item tomate = {
	2, "Tomate", 64, 128
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
		if (i % 5 == 0) { inventory.items[i] = tomate; }
		else if (i % 7 == 0) { inventory.items[i] = seedTomate; }
		else{ inventory.items[i] = seedCalabaza; }
	}
	return inventory;
}