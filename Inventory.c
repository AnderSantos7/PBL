#define _CRT_SECURE_NO_WARNINGS
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>

struct Item empty = {
	0, "Empty", 128, 0, 1
};

struct Item calabaza = {
	1, "Calabaza", 128, 64, 1
};

struct Item tomate = {
	2, "Tomate", 128, 128, 1
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
		if (i % 5 == 0) inventory.items[i] = calabaza;
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

int showingItem;
void checkHover(struct posCoord mousePos){
	int found = 0, slots = inventory.cols * inventory.rows, i = 0;
	while (!found && i < slots) {
		if (mousePos.x > i % inventory.cols * inventory.slotSize + inventory.xPos
			&& mousePos.x < (i % inventory.cols + 1) * inventory.slotSize + inventory.xPos
			&& mousePos.y > i / inventory.cols * inventory.slotSize + inventory.yPos
			&& mousePos.y < (i / inventory.cols + 1) * inventory.slotSize + inventory.yPos) {
			found = 1;
			showingItem = i;
		}
		i++;
		if (!found) showingItem = -1;
	}
}

void marraztuInvTag(SDL_Surface* textua, SDL_Surface* screenSurface) {
	if (showingItem > -1) {
		TTF_Font* font;
		font = TTF_OpenFont("assets/fonts/y.n.w.u.a.y.ttf", 25);
		SDL_Color color = { 255, 0, 0 };
		textua = TTF_RenderText_Solid(font, inventory.items[showingItem].name, color);
		aplikatuSurface(100, 100, textua, screenSurface, NULL);
		SDL_FreeSurface(textua);
		TTF_CloseFont(font);
	}
}

void insertItem(struct Item item, int quantity, int pos) {
	int i = 0, found = 0;
	while (i < inventory.cols * inventory.rows && !found) {
		if (inventory.items[i].ID == item.ID) {
			found = 1;
			pos = i;
		}
		i++; 
	}
	if (found) {
		inventory.items[pos].quantity += quantity;
	}else{
		i = 0;
		while (i < inventory.cols * inventory.rows && pos < 0) {
			if (inventory.items[i].ID == empty.ID) pos = i;
			i++;
		}

		inventory.items[pos] = item;
	}
}

struct Item removeItemFromInv(int pos) {
	struct Item item;
	item = inventory.items[pos];
	inventory.items[pos] = empty;
	return item;
}

struct Item pickHovering() {
	struct Item item = empty;
	if (showingItem >= 0) {
		item = removeItemFromInv(showingItem);
	}
	return item;
}

void showStackSize(SDL_Surface* textua, SDL_Surface* screenSurface) {
	char str[128];
	TTF_Font* font;
	font = TTF_OpenFont("assets/fonts/y.n.w.u.a.y.ttf", 25);
	SDL_Color color = { 100, 255, 0 };

	for (int i = 0; i < inventory.rows * inventory.cols; i++) {
		if(inventory.items[i].ID != 0){
			SDL_itoa(inventory.items[i].quantity, str, 10);
			textua = TTF_RenderText_Solid(font, str, color);
			aplikatuSurface(i % inventory.cols * inventory.slotSize + inventory.xPos, i / inventory.cols *  inventory.slotSize + inventory.yPos, textua, screenSurface, NULL);
			SDL_FreeSurface(textua);
		}
	}
	TTF_CloseFont(font);
}