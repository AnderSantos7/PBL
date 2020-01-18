#include "SDL.h"
#include "SDL_image.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>

int mouseHandlerDown(SDL_Event e);
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
		zabalik = mouseHandlerDown(e);
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
			int hovering = 0, i = 0, start = 0;

			if (mousePos.x > 259 && mousePos.x < 379 && mousePos.y > 331 && mousePos.y < 383) zabalik = 0;

			if (mousePos.x > 240 && mousePos.x < 400 && mousePos.y > 200 && mousePos.y < 261 && !start) {
				start = 1;
				main_menu = 0;
			}
			while (!hovering && i < 3) {
				if (mousePos.x > 150 + 128 * i && mousePos.x < 214 + 128 * i && mousePos.y > 432 && mousePos.y < 480) {
					language = i;
					hovering = 1;
				}
				i++;
			}

			if (start) initGame();
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
	int open = 0, i, closed = 0;

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
	case SDL_SCANCODE_ESCAPE:
		for (i = 1; i < 4; i++) open += inventories[i].open;
		if (open > 0) {
			closeInvs();
			closed = 1;
		}
		if (getQuestMenuState()) {
			setQuestMenuState(0);
			closed = 1;
		}
		if(!closed){
			pause();
		}
		break;
	case SDL_SCANCODE_P:
		pause();
		break;
	case SDL_SCANCODE_L:
		player.load = 1;
		load();
		break;
	case SDL_SCANCODE_C:
		getNextQuest();
		break;
	case SDL_SCANCODE_Q:
		if (player.status == HOME) {
			switch(player.canInteract) {
			case 0: 
				sleep();
				break; //cama

			case 1:
				switch (inventories[INV_CHEST].open) {
				case 0: inventories[INV_CHEST].open = 1; break;
				case 1: inventories[INV_CHEST].open = 0; break;
				}
				break;
			}
		}else if (player.status == PLAYING) {
			switch (player.canInteract) {
			case -1: break;
			case 0:
				if (inventories[INV_HOTBAR].items[player.hotbarSlot].ID == 2 || inventories[INV_HOTBAR].items[player.hotbarSlot].ID == 3) {
					inventories[INV_HOTBAR].items[player.hotbarSlot] = itemPresets[3];
					inventories[INV_HOTBAR].items[player.hotbarSlot].quantity = 10;
					playWellWaterSFX();
				}
				break;
			case 2:
				if (!getQuestMenuState()) {
					setQuestMenuState(1);
				}else {
					setQuestMenuState(0);
				}
				break;
			case 3:
				switch (inventories[INV_SHOP].open) {
				case 0:
					inventories[INV_SHOP].open = 1;
					inventories[INV_PLAYER].open = 1;
					break;
				case 1:
					inventories[INV_SHOP].open = 0;
					inventories[INV_PLAYER].open = 0;
					break;
			}
				break;
			case 4:
				if (inventories[INV_HOTBAR].items[player.hotbarSlot].ID == 12 && checkHowManyOfItem(12)>=5)
				{
					removeCertainItem(12, 5);
					if (!insertItem(INV_HOTBAR, itemPresets[16], 1, -1))
					{
						if (!insertItem(INV_PLAYER, itemPresets[16], 1, -1))
						{
							dropItem(player.facingTile, 16, 1);
						}
					}
				}
				break;
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
	case SDL_SCANCODE_1:
		player.hotbarSlot = 0;
		moveItemToHotbar(0);
		break;
	case SDL_SCANCODE_2:
		player.hotbarSlot = 1;
		moveItemToHotbar(1);
		break;
	case SDL_SCANCODE_3:
		player.hotbarSlot = 2;
		moveItemToHotbar(2);
		break;
	case SDL_SCANCODE_4:
		player.hotbarSlot = 3;
		moveItemToHotbar(3);
		break;
	case SDL_SCANCODE_5:
		player.hotbarSlot = 4;
		moveItemToHotbar(4);
		break;
	case SDL_SCANCODE_6:
		player.hotbarSlot = 5;
		moveItemToHotbar(5);
		break;
	case SDL_SCANCODE_7:
		player.hotbarSlot = 6;
		moveItemToHotbar(6);
		break;
	case SDL_SCANCODE_8:
		player.hotbarSlot = 7;
		moveItemToHotbar(7);
		break;
	case SDL_SCANCODE_9:
		player.hotbarSlot = 8;
		moveItemToHotbar(8);
		break;
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

int mouseHandlerDown(SDL_Event e) {
	int mouseSlot = 0;
	int hoveringInv;
	int zabalik = 1;
	switch (e.button.button) {
	case SDL_BUTTON_LEFT:
		hoveringInv = getHoveringInv();
		if (player.status == PAUSE || player.status == PAUSE_HOME) {
			if (mousePos.x > 240 && mousePos.x < 400 && mousePos.y > 183 && mousePos.y < 245) {
				save();
			}
			else if (mousePos.x > 240 && mousePos.x < 400 && mousePos.y > 316 && mousePos.y < 375) {
				zabalik = 0;
			}
		}else if(hoveringInv != -1 && checkHover(hoveringInv)){
			if (inventories[INV_SHOP].open && hoveringInv == INV_SHOP) {
				buyItem(showingItem);
			}else if(!getQuestMenuState()){
				if (hoveringItem.ID != 0) {
					if (inventories[hoveringInv].items[showingItem].ID != hoveringItem.ID) {
						struct Item tmpItem = hoveringItem;
						hoveringItem = inventories[hoveringInv].items[showingItem];
						inventories[hoveringInv].items[showingItem] = tmpItem;
					}else {
						inventories[hoveringInv].items[showingItem].quantity += hoveringItem.quantity;
						hoveringItem.ID = 0;
					}
				}else {
					hoveringItem = pickHovering();
				}
			}
		}
		else{
			if (player.status == PLAYING) {
				if (getQuestMenuState()) {
					interactQuestMenu();
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
									if (hoveringItem.quantity < 1) hoveringItem = itemPresets[0];
								}
								else if (inventories[INV_HOTBAR].items[player.hotbarSlot].ID != 0 && inventories[INV_HOTBAR].items[player.hotbarSlot].seed != 0) {
									landatu(inventories[INV_HOTBAR].items[player.hotbarSlot].seed);
									inventories[INV_HOTBAR].items[player.hotbarSlot].quantity--;
									if (inventories[INV_HOTBAR].items[player.hotbarSlot].quantity < 1) inventories[INV_HOTBAR].items[player.hotbarSlot] = itemPresets[0];
								}
							}
							if (tiles[player.facingTile].plant.seed != 0 && tiles[player.facingTile].plant.stage == 2) {
								harvest(player.facingTile);
							}
							else if (hoveringItem.ID == 3) {
								water(player.facingTile);
								hoveringItem.quantity--;
								if (hoveringItem.quantity < 1) hoveringItem = itemPresets[2];
							}
							else if (inventories[INV_HOTBAR].items[player.hotbarSlot].ID == 3) {
								water(player.facingTile);
								inventories[INV_HOTBAR].items[player.hotbarSlot].quantity--;
								if (inventories[INV_HOTBAR].items[player.hotbarSlot].quantity < 1) inventories[INV_HOTBAR].items[player.hotbarSlot] = itemPresets[2];

							}
							else if (hoveringItem.ID == 16 && tiles[player.facingTile].plant.seed != 0) {
								fertilize(player.facingTile);
								hoveringItem.quantity--;
								if (hoveringItem.quantity < 1) hoveringItem = itemPresets[0];
							}
							else if (inventories[INV_HOTBAR].items[player.hotbarSlot].ID == 16 && tiles[player.facingTile].plant.seed != 0) {
								fertilize(player.facingTile);
								inventories[INV_HOTBAR].items[player.hotbarSlot].quantity--;
								if (inventories[INV_HOTBAR].items[player.hotbarSlot].quantity < 1) inventories[INV_HOTBAR].items[player.hotbarSlot] = itemPresets[0];
							}
						}
						else if (inventories[INV_HOTBAR].items[player.hotbarSlot].ID == 1) {
							arar(player.facingTile);
						}

					}
					else {
						dropHoveringItem();
					}
				}
			}
			else if (player.status == HOME) {
				dropHoveringItem();
			}
		}
		break;
	case SDL_BUTTON_RIGHT:
		break;
	}
	return zabalik;
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