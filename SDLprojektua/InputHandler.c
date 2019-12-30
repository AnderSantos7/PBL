#include "SDL.h"
#include "SDL_image.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>
void mouseHandlerDown(SDL_Event e);
void keyHandlerDown(SDL_Event e);
void keyHandlerUp(SDL_Event e);
void pause();

int inputHandler(SDL_Event e) {
	int zabalik = 1;
	switch (e.type) {
	case SDL_QUIT:
		zabalik = 0;
		break;
	case SDL_MOUSEBUTTONDOWN:
		mouseHandlerDown(e);
		break;
	case SDL_KEYDOWN:
		keyHandlerDown(e);
		break;
	case SDL_KEYUP:
		keyHandlerUp(e);
		break;
	default:
		break;
	}
	return zabalik;
}

struct Item hoveringItem = { 0 };

void keyHandlerDown(SDL_Event e) {
	int open = 0, i;
	
	switch (e.key.keysym.scancode) {
		if (player.status == PLAYING || player.status == HOME) {
			case SDL_SCANCODE_A:
				player.facingDirection = DIR_LEFT;
				player.movingLeft = 1;
				playerSurface = loadMedia("assets/images/Player2.png");
				break;
			case SDL_SCANCODE_D:
				player.facingDirection = DIR_RIGHT;
				player.movingRight = 1;
				playerSurface = loadMedia("assets/images/Player.png");
				break;
			case SDL_SCANCODE_W:
				player.facingDirection = DIR_UP;
				player.movingUp = 1;
				break;
			case SDL_SCANCODE_S:
				player.facingDirection = DIR_DOWN;
				player.movingDown = 1;
				break;
			case SDL_SCANCODE_C:
				if (tiles[player.facingTile].plant.seed == NONE && tiles[player.facingTile].plant.water != NONE) {
					tiles[player.facingTile].plant.seed = CALABAZA;
				}
				break;
			case SDL_SCANCODE_T:
				if (tiles[player.facingTile].plant.seed == NONE && tiles[player.facingTile].plant.water != NONE) {
					tiles[player.facingTile].plant.seed = TOMATE;
				}
				break;
			case SDL_SCANCODE_ESCAPE:
				for (i = 1; i < 3; i++) open += inventories[i].open;
				if (open > 0) {
					closeInvs();
				}
				else {
					pause();
				}
				break;
			case SDL_SCANCODE_P:
				pause();
			case SDL_SCANCODE_E:
				if (player.status == HOME) {
					switch (inventories[INV_CHEST].open) {
					case 0:
						inventories[INV_CHEST].open = 1;
						break;
					case 1:
						inventories[INV_CHEST].open = 0;
						break;
					}
				}
				break;
			case SDL_SCANCODE_I:
				switch (inventories[INV_PLAYER].open) {
				case 0:
					inventories[INV_PLAYER].open = 1;
					break;
				case 1:
					inventories[INV_PLAYER].open = 0;
					break;
				}
				break;
		}
	}
	return;
}

void keyHandlerUp(SDL_Event e) {
	switch (e.key.keysym.scancode) {
	case SDL_SCANCODE_W:
		player.movingUp = 0;
		break;
	case SDL_SCANCODE_A:
		player.movingLeft = 0;
		break;
	case SDL_SCANCODE_S:
		player.movingDown = 0;
		break;
	case SDL_SCANCODE_D:
		player.movingRight = 0;
		break;
	}
	return;
}

void mouseHandlerDown(SDL_Event e) {
	int mouseSlot = 0;
	int hoveringInv;
	switch (e.button.button) {
	case SDL_BUTTON_LEFT:
		hoveringInv = getHoveringInv();
		if (hoveringInv != -1 && checkHover(hoveringInv)) {
			if (hoveringItem.ID != 0) {
				if (inventories[hoveringInv].items[showingItem].ID != hoveringItem.ID) {
					struct Item tmpItem = hoveringItem;
					hoveringItem = inventories[hoveringInv].items[showingItem];
					inventories[hoveringInv].items[showingItem] = tmpItem;
				}
				else {
					inventories[hoveringInv].items[showingItem].quantity += hoveringItem.quantity;
					hoveringItem.ID = 0;
				}
			}else {
				hoveringItem = pickHovering();
			}
		}else {
			if (hoveringItem.ID != 0) {
				dropItem();
			}
		}
		break;
	case SDL_BUTTON_RIGHT:
		break;
	}
	return;
}

void pause() {
	if (player.status == PLAYING) {
		player.status = PAUSE;
	}
	else if (player.status == HOME) {
		player.status = PAUSE_HOME;
	}
	else if (player.status == PAUSE) player.status = PLAYING;
	else if (player.status == PAUSE_HOME) player.status = HOME;
}