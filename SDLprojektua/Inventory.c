#define _CRT_SECURE_NO_WARNINGS
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>
#include <string.h>

void marraztuInv(int inv) {

	SDL_Rect clip;
	clip.w = inventories[inv].slotSize;
	clip.h = inventories[inv].slotSize;
	int slots = inventories[inv].cols * inventories[inv].rows;
	for (int i = 0; i < slots; i++) {
		if (inventories[inv].items[i].ID != 0) {
			clip.x = inventories[inv].items[i].sheetPosX;
			clip.y = inventories[inv].items[i].sheetPosY;
			aplikatuSurface(i % inventories[inv].cols * inventories[inv].slotSize + inventories[inv].xPos, i / inventories[inv].cols * inventories[inv].slotSize + inventories[inv].yPos, 64, 64, textures[itemsSurface], &clip);
		}
	}
}

void updateInv(int inv) {
	int slots = inventories[inv].cols * inventories[inv].rows;
	inventories[inv].items[0] = itemPresets[1];
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

int checkHover() {
	int inv = getHoveringInv(), found = 0;
	if (inv > -1) {
		int slots = inventories[inv].cols * inventories[inv].rows, i = 0;
		while (!found && i < slots) {
			if (mousePos.x > i% inventories[inv].cols* inventories[inv].slotSize + inventories[inv].xPos
				&& mousePos.x < (i % inventories[inv].cols + 1) * inventories[inv].slotSize + inventories[inv].xPos
				&& mousePos.y > i / inventories[inv].cols * inventories[inv].slotSize + inventories[inv].yPos
				&& mousePos.y < (i / inventories[inv].cols + 1) * inventories[inv].slotSize + inventories[inv].yPos) {
				found = 1;
				showingItem = i;
			}
			i++;
			if (!found) showingItem = -1;
		}
	}
	else {
		showingItem = -1;
	}
	return found;
}

void marraztuInvTag(int inv) {
	char str[128], qty[128];
	if (inventories[inv].open && showingItem > -1 && inventories[inv].items[showingItem].ID != 0) {
		TTF_Font* font;
		font = TTF_OpenFont("assets/fonts/y.n.w.u.a.y.ttf", 16);
		SDL_Color color = { 255, 255, 255 };
		strcpy(str, inventories[inv].items[showingItem].name);
		if (inventories[inv].items[showingItem].quantity > 1) {
			SDL_itoa(inventories[inv].items[showingItem].quantity, qty, 10);
			strcat(str, " x");
			strcat(str, qty);
		}
		SDL_Surface* s = TTF_RenderText_Solid(font, str, color);
		SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
		int w, h;
		SDL_QueryTexture(t, NULL, NULL, &w, &h);
		aplikatuSurface(mousePos.x, mousePos.y, w, h, t, NULL);
		SDL_FreeSurface(s);
		SDL_DestroyTexture(t);
		TTF_CloseFont(font);
	}
}

int insertItem(int inv, struct Item item, int quantity, int pos) {
	int i = 0, found = 0, success = 0, slots = inventories[inv].cols * inventories[inv].rows;
	while (i < slots && !found) {
		if (inventories[inv].items[i].ID == item.ID) {
			found = 1;
			pos = i;
		}
		i++;
	}
	if (found) {
		inventories[inv].items[pos].quantity += quantity;
		success = 1;
	}
	else {
		i = 0;
		while (i < slots && pos < 0) {
			if (inventories[inv].items[i].ID == 0) pos = i;
			i++;
		}
		if (i != slots) {
			inventories[inv].items[pos] = item;
			success = 1;
		}
	}
	return success;
}

struct Item removeItemFromInv(int inv, int pos) {
	struct Item item;
	item = inventories[inv].items[pos];
	inventories[inv].items[pos] = itemPresets[0];
	return item;
}

struct Item pickHovering() {
	struct Item item = itemPresets[0];
	int inv = getHoveringInv();
	if (showingItem >= 0 && inv != -1) {
		item = removeItemFromInv(inv, showingItem);
	}
	return item;
}

void marraztuHoveringItem() {
	SDL_Rect clip = { 0, 0, 64, 64 };
	clip.x = hoveringItem.sheetPosX;
	clip.y = hoveringItem.sheetPosY;
	aplikatuSurface(mousePos.x, mousePos.y, 64, 64, textures[itemsSurface], &clip);
	if (hoveringItem.quantity > 1) {
		int offset = 42;
		char str[128];
		TTF_Font* font;
		font = TTF_OpenFont("assets/fonts/y.n.w.u.a.y.ttf", 16);
		SDL_Color color = { 255, 255, 255 };
		SDL_itoa(hoveringItem.quantity, str, 10);
		SDL_Surface* s = TTF_RenderText_Solid(font, str, color);
		SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
		int w, h;
		SDL_QueryTexture(t, NULL, NULL, &w, &h);
		aplikatuSurface(mousePos.x + offset, mousePos.y + offset, w, h, t, NULL);
		SDL_FreeSurface(s);
		SDL_DestroyTexture(t);

		TTF_CloseFont(font);
	}
	return;
}

void showStackSize(int inv) {
	int offset = 42;
	char str[128];
	TTF_Font* font;
	font = TTF_OpenFont("assets/fonts/y.n.w.u.a.y.ttf", 16);
	SDL_Color color = { 255, 255, 255 };

	for (int i = 0; i < inventories[inv].rows * inventories[inv].cols; i++) {
		if (inventories[inv].items[i].ID != 0 && inventories[inv].items[i].quantity > 1) {
			SDL_itoa(inventories[inv].items[i].quantity, str, 10);

			SDL_Surface* s = TTF_RenderText_Solid(font, str, color);
			SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
			int w, h;
			SDL_QueryTexture(t, NULL, NULL, &w, &h);
			aplikatuSurface(i % inventories[inv].cols * inventories[inv].slotSize + inventories[inv].xPos + offset, i / inventories[inv].cols * inventories[inv].slotSize + inventories[inv].yPos + offset, w, h, t, NULL);
			SDL_FreeSurface(s);
			SDL_DestroyTexture(t);
		}
	}
	TTF_CloseFont(font);
	return;
}

void showInv(int inv) {
	if (inventories[inv].open) {
		SDL_Rect clip;
		clip.x = inventories[inv].sheetPosX;
		clip.y = inventories[inv].sheetPosY;
		clip.w = inventories[inv].cols * inventories[inv].slotSize + 6;
		clip.h = inventories[inv].rows * inventories[inv].slotSize + inventories[inv].headerSize + 3;
		int w = 64 * inventories[inv].cols + 6;
		int h = 64 * inventories[inv].rows + inventories[inv].headerSize + 3;
		aplikatuSurface(inventories[inv].xPos - 3, inventories[inv].yPos - inventories[inv].headerSize, w, h, textures[HUDSurface], &clip);
		marraztuInv(inv);
		if (inv == INV_HOTBAR) {
			clip.x = 582;
			clip.y = 0;
			clip.w = 64;
			clip.h = 64;
			aplikatuSurface(inventories[inv].xPos + inventories[inv].slotSize * player.hotbarSlot, inventories[inv].yPos, 64, 64, textures[HUDSurface], &clip);
		}
		showStackSize(inv);
	}
	return;
}

void closeInvs() {
	for (int i = 1; i < 3; i++) {
		inventories[i].open = 0;
	}
	return;
}