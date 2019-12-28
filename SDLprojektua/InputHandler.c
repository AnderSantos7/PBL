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
			case SDL_SCANCODE_P:
				pause();
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
	switch (e.button.button) {
	case SDL_BUTTON_LEFT:
		if (checkHover(mousePos)) {
			if (hoveringItem.ID != 0) {
				if (inventory.items[showingItem].ID != hoveringItem.ID) {
					struct Item tmpItem = hoveringItem;
					hoveringItem = inventory.items[showingItem];
					inventory.items[showingItem] = tmpItem;
				}
				else {
					inventory.items[showingItem].quantity += hoveringItem.quantity;
					hoveringItem.ID = 0;
				}
			}else {
				hoveringItem = pickHovering();
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