#define _CRT_SECURE_NO_WARNINGS
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>
#include <string.h>

//Inbentarioko itemak irudikatu
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

//Inbentarioak hasieratzeko funtzioa. Debuggeatu eta gauzak probatzeko.
void updateInv(int inv) {
	int slots = inventories[inv].cols * inventories[inv].rows;
	inventories[inv].items[0] = itemPresets[1];
}

//showingItem = ibentario bateko item baten gainean arratoia jarri ezkero, item honen inbentario array-ko posizioa.
int showingItem;

//Ea inbentario batean barruan dagoen arratoia
int getHoveringInv() {
	int hovering = -1, i = 0;
	while (hovering < 0 && i < 4) {
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

//showingItem eguneratzeko funtzioa
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

//showingItem ez bada -1, arratoia item baten gainean dago. Orduan objektuari buruzko informazioa erakusten da. (Izena eta kopurua)
void marraztuInvTag(int inv) {
	if (!getQuestMenuState() && inventories[inv].open && showingItem > -1 && inventories[inv].items[showingItem].ID != 0 && inv != 3) {
		char str[128], qty[128];
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
		int offSet = 16;
		SDL_QueryTexture(t, NULL, NULL, &w, &h);
		SDL_Rect clip = {201, 477, 200, 50};
		int posX = mousePos.x, posY = mousePos.y;
		if (mousePos.x + w + 40 >= SCREEN_WIDTH) posX = mousePos.x - w -20;
		if (mousePos.y + h + 40 >= SCREEN_HEIGHT) posY = mousePos.y - h - 20;
		aplikatuSurface(posX + offSet - 10, posY + offSet - 10, w + 20, h + 20, textures[HUDSurface], &clip);
		aplikatuSurface(posX + offSet, posY + offSet, w, h, t, NULL);
		SDL_FreeSurface(s);	
		SDL_DestroyTexture(t);
		TTF_CloseFont(font);
	}
	else if (inv == 3 && inventories[inv].open && showingItem > -1 && inventories[inv].items[showingItem].ID != 0) {
		showPrice(showingItem);
	}
}

//Item bat inbentario batean sartzeko funtzioa. Pos = -1 bada, lehenik eta behin ea mota bereko itemik dagoen begiratuko da. Egoten bada, batu egingo dira.
//Ez badago halako itemik, hutsik dagoen lehen posizioan sartuko da.
//Misioen sariak eman eta arratoiarekin arrastratutako item bat inbentarioan uzteko erabiltzen da.
int insertItem(int inv, struct Item item, int quantity, int pos) {
	int i = 0, found = 0, success = 0, slots = inventories[inv].cols * inventories[inv].rows;
	//Mota berdineko itemik dagoen ala ez begiratu
	while (i < slots && !found) {
		if (inventories[inv].items[i].ID == item.ID) {
			found = 1;
			pos = i;
		}
		i++;
	}
	//Egoten bada, stack kopuruak batu
	if (found) {
		inventories[inv].items[pos].quantity += quantity;
		success = 1;
	}
	//Bestela utzik dagoen posizio batean sartu. Ez badago posiziorik libre, 0 itzuliko da. Itema inbentarioan sartu bada berriz, 1.
	else {
		i = 0;
		while (i < slots && pos < 0) {
			if (inventories[inv].items[i].ID == 0) {
				pos = i;
			}else i++;
		}
		if (i != slots) {
			inventories[inv].items[pos] = item;
			inventories[inv].items[pos].quantity = quantity;
			success = 1;
		}
	}
	struct Inventory inb = inventories[inv];
	return success;
}

//Posizio bateko item bat kendu eta itzuli. Arratoiarekin item bat hartzeko erabiltzen da.
struct Item removeItemFromInv(int inv, int pos) {
	struct Item item;
	item = inventories[inv].items[pos];
	inventories[inv].items[pos] = itemPresets[0];
	return item;
}

//Edozein inbentariotik arratoia item baten gainean jarri eta 1-9 teklak erabiliz item hori hotbarreko posizio horretara mugitu.
//Posizio horretan itemik badago, item horrekin posizioak trukatuko ditu.
void moveItemToHotbar(int slot) {
	int inv = getHoveringInv();
	if (showingItem != -1 && inventories[inv].items[showingItem].ID != 0) {
		struct Item tmp;
		tmp = inventories[inv].items[showingItem];
		inventories[inv].items[showingItem] = inventories[INV_HOTBAR].items[slot];
		inventories[INV_HOTBAR].items[slot] = tmp;
	}
	return;
}

//Arratoiarekin erakusten den item-a hartu eta arrastratu.
struct Item pickHovering() {
	struct Item item = itemPresets[0];
	int inv = getHoveringInv();
	if (showingItem >= 0 && inv != -1) {
		item = removeItemFromInv(inv, showingItem);
	}
	return item;
}

//Arratoiarekin arrastratzen den item-a pantailan irudikatu.
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

//Inbentarioko item bakoitzaren kopurua pantailaratu.
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

//Inbentario bakoitzeko elementu desberdinak irudikatzeko erabiltzen diren funtzio desberdinak deitzeko funtzioa. Apaingarriak diren textura batzuk ere irudikatu.
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
		else if (inv == INV_SHOP && getHoveringInv == INV_SHOP) {
			showPrice(showingItem);
		}
		showStackSize(inv);
	}
	return;
}

//Inbentario guztiak ixteko funtzioa. Esc tekla sakatu edo etxean sartu / etxetik ateratzean erabiltzen da.
void closeInvs() {
	for (int i = 1; i < 5; i++) {
		inventories[i].open = 0;
	}
	return;
}

//Jokalariaren inbentarioan (inbentario + hotbar) item mota bateko zenbat dagoen kontatzen duen funtzioa.
int checkHowManyOfItem(int item) {
	int ammount = 0;
	for (int i = 0; i < 2; i++) {
		int slots = inventories[i].cols * inventories[i].rows;
		for (int j = 0; j < slots; j++) {
			if (inventories[i].items[j].ID == item) ammount += inventories[i].items[j].quantity;
		}
	}
	return ammount;
}

//Jokalariaren inbentariotik (inbentario + hotbar) item mota baten kopuru zehatz bat kendu. Lehendabizi hotbarretik kentzen da, gero inbentariotik.
//Item-a stack desberdinetan egonda ere kenduko da kopuru zuzena.
int removeCertainItem(int item, int ammount) {
	int success = 0;
	int i = 0;
	int removedAmmount = 0;
	int toBeRemoved = ammount;
	while (i < 2 && removedAmmount < ammount) {
		int slots = inventories[i].cols * inventories[i].rows;
		int j = 0;
		while (j < slots && removedAmmount < ammount) {
			if (inventories[i].items[j].ID == item) {
				if (inventories[i].items[j].quantity >= toBeRemoved) {
					removedAmmount = toBeRemoved;
					toBeRemoved -= removedAmmount;
					inventories[i].items[j].quantity -= removedAmmount;
					if (inventories[i].items[j].quantity < 1) inventories[i].items[j] = itemPresets[0];
				}else {
					removedAmmount = inventories[i].items[j].quantity;
					toBeRemoved -= removedAmmount;
					inventories[i].items[j] = itemPresets[0];
				}
			}
			j++;
		}
		i++;
	}
	if (!toBeRemoved) success = 1;
	return success;
}