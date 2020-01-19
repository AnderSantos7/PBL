#define _CRT_SECURE_NO_WARNINGS
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

int prices[] = {1,2,3,4,5,6,5};
int currency[] = { 4, 4, 6, 8, 10 };
void drawShop() {
	SDL_Rect seller = { 121, 0, 48, 48 };
	aplikatuSurface(500 - camera.x, 35 - camera.y, 80, 80, textures[obstacleSurface], &seller);
	return;
}

int payCoins = 0;
void unlockShopItem(int ID) {
	if (ID >= 1) inventories[INV_SHOP].items[0] = itemPresets[5];
	if (ID >= 4) inventories[INV_SHOP].items[1] = itemPresets[7];
	if (ID >= 6) inventories[INV_SHOP].items[2] = itemPresets[9];
	if (ID >= 8) inventories[INV_SHOP].items[3] = itemPresets[11];
	if (ID >= 10) inventories[INV_SHOP].items[4] = itemPresets[13];
	if (ID >= 25) {
		inventories[INV_SHOP].items[5] = itemPresets[15];
		payCoins = 1;
	}
	return;
}

void buyItem(int pos) {
	if (pos <= 5) {
		int currencyToPay = currency[pos];
		if (payCoins) currencyToPay = 17;
		if (checkHowManyOfItem(currencyToPay) >= prices[pos])
		{
			removeCertainItem(currencyToPay, prices[pos]);
			prices[pos]++;
			if (!insertItem(INV_HOTBAR, inventories[INV_SHOP].items[pos], 1, -1)) {
				if (!insertItem(INV_HOTBAR, inventories[INV_SHOP].items[pos], 1, -1)) {
					dropItem(player.facingTile, inventories[INV_SHOP].items[pos].ID, 1);
				}
			}
			if (pos == 5) upgradeQuests();
		}
	}
	return;
}

void showPrice(int item) {
	char str[128] = {'x', '\0'}, qty[128];
	TTF_Font* font;
	font = TTF_OpenFont("assets/fonts/y.n.w.u.a.y.ttf", 24);
	SDL_Color color = { 255, 255, 255 };
	SDL_itoa(prices[item], qty, 10);
	strcat(str, qty);
	SDL_Surface* s = TTF_RenderText_Solid(font, str, color);
	SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
	int w, h;
	int offSet = 16;
	SDL_QueryTexture(t, NULL, NULL, &w, &h);
	SDL_Rect clip = { itemPresets[currency[item]].sheetPosX, itemPresets[currency[item]].sheetPosY, 64, 64 };
	int posX = mousePos.x, posY = mousePos.y;
	if (mousePos.x + w + 40 >= SCREEN_WIDTH) posX = mousePos.x - w - 84;
	if (mousePos.y + h + 40 >= SCREEN_WIDTH) posY = mousePos.y - h - 84;
	SDL_Rect hClip = { 201, 477, 200, 50 };
	aplikatuSurface(posX + offSet - 10, posY + offSet - 10, w + 96, h + 54, textures[HUDSurface], &hClip);
	aplikatuSurface(posX + offSet, posY + offSet, 64, 64, textures[itemsSurface], &clip);
	aplikatuSurface(posX + offSet + 72, posY + offSet + 16, w, h, t, NULL);
	SDL_FreeSurface(s);
	SDL_DestroyTexture(t);
	TTF_CloseFont(font);
	return;
}