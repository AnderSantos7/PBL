#include "SDL.h"
#include "SDL_image.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>
void mouseHandlerDown(SDL_Event e);
void keyHandlerDown(SDL_Event e);
void updateFacingDir();
void keyHandlerUp(SDL_Event e);
void hotbarScroll(SDL_Event e);
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
	case SDL_MOUSEWHEEL:
		hotbarScroll(e);
		break;
	case SDL_KEYDOWN:
		keyHandlerDown(e);
		break;
	case SDL_KEYUP:
		keyHandlerUp(e);
		break;
	}
	return zabalik;
}

int inputMainMenu(SDL_Event e) {
	int zabalik = 1;
	switch (e.type) {
	case SDL_QUIT:
		zabalik = 0;
		break;
	case SDL_MOUSEBUTTONDOWN:
		if (e.button.button == SDL_BUTTON_LEFT) {
			int hovering = 0, i = 0;

			while (!hovering && i < 3) {
				if (mousePos.x > 64 + 128 * i && mousePos.x < 128 + 128 * i && mousePos.y > 120 && mousePos.y < 168) {
					language = i;
					hovering = 1;
					main_menu = 0;
				}
				i++;
			}
			if (hovering) initGame();
		}
		break;
	}
	return zabalik;
}

struct Item hoveringItem = { 0 };

int keyDownW = 0;
int keyDownA = 0;
int keyDownS = 0;
int keyDownD = 0;


void keyHandlerDown(SDL_Event e) {
	int open = 0, i;
	
	switch (e.key.keysym.scancode) {
		if (player.status == PLAYING || player.status == HOME) {
			case SDL_SCANCODE_A:
				keyDownA = 1;
				player.facingDirection = DIR_LEFT;
				player.movingLeft = 1;
				player.movingRight = 0;
				break;
			case SDL_SCANCODE_D:
				keyDownD = 1;
				player.facingDirection = DIR_RIGHT;
				player.movingRight = 1;
				player.movingLeft = 0;
				break;
			case SDL_SCANCODE_W:
				keyDownW = 1;
				player.facingDirection = DIR_UP;
				player.movingUp = 1;
				player.movingDown = 0;
				break;
			case SDL_SCANCODE_S:
				keyDownS = 1;
				player.facingDirection = DIR_DOWN;
				player.movingDown = 1;
				player.movingUp = 0;
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
			case SDL_SCANCODE_Q:
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
				else if (player.canInteract) {
					if (inventories[INV_HOTBAR].items[player.hotbarSlot].ID == 2) {
						inventories[INV_HOTBAR].items[player.hotbarSlot] = itemPresets[3];
						inventories[INV_HOTBAR].items[player.hotbarSlot].quantity = 10;
					}
				}
				break;
			case SDL_SCANCODE_E:
				switch (inventories[INV_PLAYER].open) {
				case 0:
					inventories[INV_PLAYER].open = 1;
					break;
				case 1:
					inventories[INV_PLAYER].open = 0;
					break;
				}
				break;
			case SDL_SCANCODE_1: player.hotbarSlot = 0; break;
			case SDL_SCANCODE_2: player.hotbarSlot = 1; break;
			case SDL_SCANCODE_3: player.hotbarSlot = 2; break;
			case SDL_SCANCODE_4: player.hotbarSlot = 3; break;
			case SDL_SCANCODE_5: player.hotbarSlot = 4; break;
			case SDL_SCANCODE_6: player.hotbarSlot = 5; break;
			case SDL_SCANCODE_7: player.hotbarSlot = 6; break;
			case SDL_SCANCODE_8: player.hotbarSlot = 7; break;
			case SDL_SCANCODE_9: player.hotbarSlot = 8; break;
		}
	}
	return;
}

void keyHandlerUp(SDL_Event e) {
	switch (e.key.keysym.scancode) {
	case SDL_SCANCODE_W:
		keyDownW = 0;
		player.movingUp = 0;
		updateFacingDir();
		break;
	case SDL_SCANCODE_A:
		keyDownA = 0;
		player.movingLeft = 0;
		updateFacingDir();
		break;
	case SDL_SCANCODE_S:
		keyDownS = 0;
		player.movingDown = 0;
		updateFacingDir();
		break;
	case SDL_SCANCODE_D:
		keyDownD = 0;
		player.movingRight = 0;
		updateFacingDir();
		break;
	}
	return;
}

void updateFacingDir() {
	if (keyDownA) {
		player.movingLeft = 1;
		player.facingDirection = DIR_LEFT;
	}
	else if (keyDownD) {
		player.movingRight = 1;
		player.facingDirection = DIR_RIGHT;
	}
	else if (keyDownS) {
		player.movingDown = 1;
		player.facingDirection = DIR_DOWN;
	}
	else if (keyDownW) {
		player.movingUp = 1;
		player.facingDirection = DIR_UP;
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
			int i = 0, soil = 0;
			while (i < 49 && !soil) {
				if (player.facingTile == plantable_ID[i]) soil = 1;
				i++;
			}
			if (soil) {
				if (tiles[player.facingTile].plant.arado) {
					if (tiles[player.facingTile].plant.seed == NONE) {
						if (hoveringItem.ID != 0 && hoveringItem.seed != 0) {
							landatu(hoveringItem.seed);
							hoveringItem.quantity--;
							if (hoveringItem.quantity < 1) hoveringItem.ID = 0;
						}
						else if (inventories[INV_HOTBAR].items[player.hotbarSlot].ID != 0 && inventories[INV_HOTBAR].items[player.hotbarSlot].seed != 0) {
							landatu(inventories[INV_HOTBAR].items[player.hotbarSlot].seed);
							inventories[INV_HOTBAR].items[player.hotbarSlot].quantity--;
							if (inventories[INV_HOTBAR].items[player.hotbarSlot].quantity < 1) inventories[INV_HOTBAR].items[player.hotbarSlot].ID = 0;
						}
					}
					if (hoveringItem.ID == 3) {
						water(player.facingTile);
						hoveringItem.quantity--;
						if (hoveringItem.quantity < 1) hoveringItem = itemPresets[2];
					}else if (inventories[INV_HOTBAR].items[player.hotbarSlot].ID == 3) {
						water(player.facingTile);
						inventories[INV_HOTBAR].items[player.hotbarSlot].quantity--;
						if (inventories[INV_HOTBAR].items[player.hotbarSlot].quantity < 1) inventories[INV_HOTBAR].items[player.hotbarSlot] = itemPresets[2];
					}
				}else if(inventories[INV_HOTBAR].items[player.hotbarSlot].ID == 1){
					tiles[player.facingTile].plant.arado = 1;
				}
			}else {
				dropItem();
			}
		}
		break;
	case SDL_BUTTON_RIGHT:
		break;
	}
	return;
}

void hotbarScroll(SDL_Event e) {
	if (e.wheel.y < 0) {
		player.hotbarSlot++;
		if (player.hotbarSlot > 8) player.hotbarSlot = 0;
	}
	else if (e.wheel.y > 0) {
		player.hotbarSlot--;
		if (player.hotbarSlot < 0) player.hotbarSlot = 8;
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