#define _CRT_SECURE_NO_WARNINGS
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>
#include <string.h>

struct Item empty = {
	0, "Empty", 128, 0, 1
};

struct Item calabaza = {
	1, "Calabaza", 128, 64, 1
};

struct Item tomate = {
	2, "Tomate", 128, 128, 1
};

void marraztuInv(int inv, SDL_Surface* spriteSheetSurface, SDL_Surface* screenSurface){
	SDL_Rect clip;
	clip.w = inventories[inv].slotSize;
	clip.h = inventories[inv].slotSize;
	int slots = inventories[inv].cols * inventories[inv].rows;
	for (int i = 0; i < slots; i++) {
		clip.x = inventories[inv].items[i].sheetPosX;
		clip.y = inventories[inv].items[i].sheetPosY;
		aplikatuSurface(i % inventories[inv].cols * inventories[inv].slotSize + inventories[inv].xPos, i / inventories[inv].cols * inventories[inv].slotSize + inventories[inv].yPos, spriteSheetSurface, screenSurface, &clip);
	}
}

void updateInv(int inv) {
	int slots = inventories[inv].cols * inventories[inv].rows;
	for (int i = 0; i < slots; i++) {
		inventories[inv].items[i] = empty;
	}
	inventories[inv].items[4] = tomate;
	inventories[inv].items[7] = tomate;
	inventories[inv].items[16] = tomate;
	inventories[inv].items[1] = tomate;
	inventories[inv].items[13] = calabaza;
}

void changeInv(int inv, int InvPos) {
	if (tiles[player.facingTile].plant.seed == CALABAZA) {
		inventories[inv].items[InvPos] = calabaza;
	}
	else if (tiles[player.facingTile].plant.seed == TOMATE) {
		inventories[inv].items[InvPos] = tomate;
	}
}

int showingItem;

int getHoveringInv() {
	int hovering = -1, i = 0;
	while (hovering < 0 && i < 3) {
		if (inventories[i].open) {
			if (mousePos.x > inventories[i].xPos
				&& mousePos.x < inventories[i].xPos + inventories[i].cols * inventories[i].slotSize
				&& mousePos.y > inventories[i].yPos
				&& mousePos.y < inventories[i].yPos + inventories[i].rows * inventories[i].slotSize) {
				hovering = i;
			}
		}
		i++;
	}
	return hovering;
}

int checkHover(int inv){
	int found = 0, slots = inventories[inv].cols * inventories[inv].rows, i = 0;
	while (!found && i < slots) {
		if (mousePos.x > i % inventories[inv].cols * inventories[inv].slotSize + inventories[inv].xPos
			&& mousePos.x < (i % inventories[inv].cols + 1) * inventories[inv].slotSize + inventories[inv].xPos
			&& mousePos.y > i / inventories[inv].cols * inventories[inv].slotSize + inventories[inv].yPos
			&& mousePos.y < (i / inventories[inv].cols + 1) * inventories[inv].slotSize + inventories[inv].yPos) {
			found = 1;
			showingItem = i;
		}
		i++;
		if (!found) showingItem = -1;
	}
	return found;
}

void marraztuInvTag(int inv, SDL_Surface* textua, SDL_Surface* screenSurface) {
	char* str[128], qty[128];
	if (showingItem > -1 && inventories[inv].items[showingItem].ID != 0) {
		TTF_Font* font;
		font = TTF_OpenFont("assets/fonts/y.n.w.u.a.y.ttf", 25);
		SDL_Color color = { 255, 255, 255 };

		strcpy(str, inventories[inv].items[showingItem].name);
		if (inventories[inv].items[showingItem].quantity > 1) {
			SDL_itoa(inventories[inv].items[showingItem].quantity, qty, 10);
			strcat(str, " x");
			strcat(str, qty);
		}
		textua = TTF_RenderText_Solid(font, str, color);
		aplikatuSurface(mousePos.x, mousePos.y, textua, screenSurface, NULL);
		SDL_FreeSurface(textua);
		TTF_CloseFont(font);
	}
}

void insertItem(int inv, struct Item item, int quantity, int pos) {
	int i = 0, found = 0;
	while (i < inventories[inv].cols * inventories[inv].rows && !found) {
		if (inventories[inv].items[i].ID == item.ID) {
			found = 1;
			pos = i;
		}
		i++; 
	}
	if (found) {
		inventories[inv].items[pos].quantity += quantity;
	}else{
		i = 0;
		while (i < inventories[inv].cols * inventories[inv].rows && pos < 0) {
			if (inventories[inv].items[i].ID == empty.ID) pos = i;
			i++;
		}

		inventories[inv].items[pos] = item;
	}
}

struct Item removeItemFromInv(int inv, int pos) {
	struct Item item;
	item = inventories[inv].items[pos];
	inventories[inv].items[pos] = empty;
	return item;
}

struct Item pickHovering() {
	struct Item item = empty;
	int inv = getHoveringInv();
	if (showingItem >= 0 && inv != -1) {
		item = removeItemFromInv(inv, showingItem);
	}
	return item;
}

void marraztuHoveringItem(int inv, SDL_Surface* spriteSheetSurface, SDL_Surface* textua, SDL_Surface* screenSurface) {
	SDL_Rect clip = {0, 0, inventories[inv].slotSize, inventories[inv].slotSize};
	clip.x = hoveringItem.sheetPosX;
	clip.y = hoveringItem.sheetPosY;
	aplikatuSurface(mousePos.x, mousePos.y, spriteSheetSurface, screenSurface, &clip);
	if (hoveringItem.quantity > 1) {
		char str[128];
		TTF_Font* font;
		font = TTF_OpenFont("assets/fonts/y.n.w.u.a.y.ttf", 25);
		SDL_Color color = { 255, 255, 255 };
		SDL_itoa(hoveringItem.quantity, str, 10);
		textua = TTF_RenderText_Solid(font, str, color);
		aplikatuSurface(mousePos.x, mousePos.y, textua, screenSurface, NULL);
		SDL_FreeSurface(textua);

		TTF_CloseFont(font);
	}
	return;
}

void showStackSize(int inv, SDL_Surface* textua, SDL_Surface* screenSurface) {
	char str[128];
	TTF_Font* font;
	font = TTF_OpenFont("assets/fonts/y.n.w.u.a.y.ttf", 25);
	SDL_Color color = { 255, 255, 255 };

	for (int i = 0; i < inventories[inv].rows * inventories[inv].cols; i++) {
		if(inventories[inv].items[i].ID != 0 && inventories[inv].items[i].quantity > 1){
			SDL_itoa(inventories[inv].items[i].quantity, str, 10);
			textua = TTF_RenderText_Solid(font, str, color);
			aplikatuSurface(i % inventories[inv].cols * inventories[inv].slotSize + inventories[inv].xPos, i / inventories[inv].cols * inventories[inv].slotSize + inventories[inv].yPos, textua, screenSurface, NULL);
			SDL_FreeSurface(textua);
		}
	}
	TTF_CloseFont(font);
}

void showInv(int inv, SDL_Surface* itemsSurface, SDL_Surface* screenSurface, SDL_Surface* textua) {
	if (inventories[inv].open) {
		marraztuInv(inv, itemsSurface, screenSurface);
		marraztuInvTag(inv, textua, screenSurface);
		showStackSize(inv, textua, screenSurface);
	}
	if (hoveringItem.ID != 0)marraztuHoveringItem(inv, itemsSurface, textua, screenSurface);

	
	return;
}

void closeInvs() {
	for (int i = 1; i < 3; i++) {
		inventories[i].open = 0;
	}
	return;
}