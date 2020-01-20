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

//Ebentoak motetan sailkatu
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

//Main menuko ebentoak sailkatu eta aztertu
int inputMainMenu(SDL_Event e) {
	int zabalik = 1;
	switch (e.type) {
	case SDL_QUIT:
		zabalik = 0;
		break;
	case SDL_MOUSEBUTTONDOWN:
		if (e.button.button == SDL_BUTTON_LEFT) {
			int hovering = 0, i = 0, start = 0;
			//Pause menuko botoiak behatu
			if (!instructions) {
				//Menuko botoiak klikatzen diren behatu
				if (mousePos.x > 240 && mousePos.x < 400 && mousePos.y > 200 && mousePos.y < 261 && !start && player.status != LOAD) {
					start = 1;
					main_menu = 0;
					load(0);
				}
				if (mousePos.x > 280 && mousePos.x < 360 && mousePos.y > 271 && mousePos.y < 323 && player.status != LOAD) {
					instructions++;
				}
				else if (mousePos.x > 259 && mousePos.x < 259 + 120 && mousePos.y > 271 + 60 && mousePos.y < 271 + 112 && !start && player.status != LOAD) {
					player.status = LOAD;
				}
				else if (player.status == LOAD) {
					if (mousePos.x > 240 && mousePos.x < 400 && mousePos.y > 183 && mousePos.y < 245 && !start) {
						start = 1;
						main_menu = 0;
						player = createPlayer();
						load(1);
					}
					else if (mousePos.x > 240 && mousePos.x < 400 && mousePos.y > 250 && mousePos.y < 312 && !start) {
						start = 1;
						main_menu = 0;
						player = createPlayer();
						load(2);
					}
					else if (mousePos.x > 280 && mousePos.x < 360 && mousePos.y > 331 && mousePos.y < 360) {
						player.status = -1;
						menu(deltaTime);
					}

				}
				while (!hovering && i < 3) {
					if (mousePos.x > 150 + 128 * i && mousePos.x < 214 + 128 * i && mousePos.y > 432 && mousePos.y < 480) {
						language = i;
						hovering = 1;
						menu(deltaTime);
						SDL_FreeSurface(textures[saveSurface]);
						SDL_FreeSurface(textures[loadSurface]);
						SDL_FreeSurface(textures[pauseSurface]);
						if (language == ESP) {
							textures[saveSurface] = textures[saveSurfaceESP];
							textures[loadSurface] = textures[loadSurfaceESP];
							textures[pauseSurface] = textures[pauseSurfaceESP];
						}
						else if (language == ENG) {
							textures[saveSurface] = textures[saveSurfaceENG];
							textures[loadSurface] = textures[loadSurfaceENG];
							textures[pauseSurface] = textures[pauseSurfaceENG];
						}
						else if (language == EUS) {
							textures[saveSurface] = textures[saveSurfaceEUS];
							textures[loadSurface] = textures[loadSurfaceEUS];
							textures[pauseSurface] = textures[pauseSurfaceEUS];
						}
					}
					i++;
				}
			}
			else if (instructions) {
				if (mousePos.x > 0 && mousePos.x < 640 && mousePos.y > 0 && mousePos.y < 480) {
					instructions++;
				}
			}

			if (start) initGame();
		}
		break;
	}
	return zabalik;
}
//Arratoiarekin arrastratzen den item struct-a
struct Item hoveringItem = { 0 };

int keyDownW = 0;
int keyDownA = 0;
int keyDownS = 0;
int keyDownD = 0;

//Tekla ebentuak aztertu
void keyHandlerDown(SDL_Event e) {
	int open = 0, i, closed = 0;

	switch (e.key.keysym.scancode) {
		if (player.status == PLAYING || player.status == HOME) {
	case SDL_SCANCODE_A:
	case SDL_SCANCODE_LEFT:
		keyDownA = 1;
		player.facingDirection = DIR_LEFT;
		player.movingLeft = 1;
		player.movingRight = 0;
		break;
	case SDL_SCANCODE_D:
	case SDL_SCANCODE_RIGHT:
		keyDownD = 1;
		player.facingDirection = DIR_RIGHT;
		player.movingRight = 1;
		player.movingLeft = 0;
		break;
	case SDL_SCANCODE_W:
	case SDL_SCANCODE_UP:
		keyDownW = 1;
		player.facingDirection = DIR_UP;
		player.movingUp = 1;
		player.movingDown = 0;
		break;
	case SDL_SCANCODE_S:
	case SDL_SCANCODE_DOWN:
		keyDownS = 1;
		player.facingDirection = DIR_DOWN;
		player.movingDown = 1;
		player.movingUp = 0;
		break;
		//Hainbat inbentario edo menu itxi eta bestela pause menua zabaldu.
	case SDL_SCANCODE_ESCAPE:
		for (i = 1; i < 5; i++) open += inventories[i].open;
		if (open > 0) {
			closeInvs();
			closed = 1;
		}
		if (getQuestMenuState()) {
			setQuestMenuState(0);
			closed = 1;
		}
		if (!closed) {
			pause();
		}
		break;
		//Pausatu edo pausa itxi
	case SDL_SCANCODE_P:
		pause();
		break;
	case SDL_SCANCODE_C:
		getNextQuest();
		break;
	case SDL_SCANCODE_Q:
		if (player.status == HOME) {
			switch (player.canInteract) {
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
		}
		else if (player.status == PLAYING) {
			switch (player.canInteract) {
			case -1: break;
			case 0: //Putzutik ura hartu hotbarrean kuboa aukeratuta badago
				if (inventories[INV_HOTBAR].items[player.hotbarSlot].ID == 2 || inventories[INV_HOTBAR].items[player.hotbarSlot].ID == 3) {
					inventories[INV_HOTBAR].items[player.hotbarSlot] = itemPresets[3];
					inventories[INV_HOTBAR].items[player.hotbarSlot].quantity = 10;
					playWellWaterSFX();
				}
				break;
			case 2: //Misio kartela zabaldu / itxi
				if (!getQuestMenuState()) {
					setQuestMenuState(1);
				}
				else {
					setQuestMenuState(0);
				}
				break;
			case 3: //Kutxa zabaldu / itxi
				switch (inventories[INV_SHOP].open) {
				case 0: inventories[INV_SHOP].open = 1; break;
				case 1: inventories[INV_SHOP].open = 0; break;
				}
				break;
			case 4: //Behiari garia eman ongarria lortzeko
				if (inventories[INV_HOTBAR].items[player.hotbarSlot].ID == 12 && checkHowManyOfItem(12) >= 5)
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
	case SDL_SCANCODE_E: //Inbentarioa zabaldu / itxi
		switch (inventories[INV_PLAYER].open) {
		case 0:
			inventories[INV_PLAYER].open = 1;
			break;
		case 1:
			inventories[INV_PLAYER].open = 0;
			break;
		}
		break;
		//Itemak hotbarrera mugitu eta hotbarreko aukeratutako hutsunea aldatu
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
//Teklak askatzean gertatutako ebentoak aztertu / Jokalariaren mugimendua eta norantza zuzenak direla zihurtatzeko
void keyHandlerUp(SDL_Event e) {
	switch (e.key.keysym.scancode) {
	case SDL_SCANCODE_W:
	case SDL_SCANCODE_UP:
		keyDownW = 0;
		player.movingUp = 0;
		updateFacingDir();
		break;
	case SDL_SCANCODE_A:
	case SDL_SCANCODE_LEFT:
		keyDownA = 0;
		player.movingLeft = 0;
		updateFacingDir();
		break;
	case SDL_SCANCODE_S:
	case SDL_SCANCODE_DOWN:
		keyDownS = 0;
		player.movingDown = 0;
		updateFacingDir();
		break;
	case SDL_SCANCODE_D:
	case SDL_SCANCODE_RIGHT:
		keyDownD = 0;
		player.movingRight = 0;
		updateFacingDir();
		break;
	}
	return;
}

//Baserritarraren norantza eguneratu ibiltzen ari den noranzko berdinera marrazteko
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

//Klik egitean ebentoak aztertu
int mouseHandlerDown(SDL_Event e) {
	int mouseSlot = 0;
	int hoveringInv;
	int zabalik = 1;
	switch (e.button.button) {
	case SDL_BUTTON_LEFT:
		//Ze inbentariotan dagoen sagua begiratu
		hoveringInv = getHoveringInv();
		if (player.status == PAUSE || player.status == PAUSE_HOME) {
			if (mousePos.x > 240 && mousePos.x < 400 && mousePos.y > 183 && mousePos.y < 245) {
				if (player.status == PAUSE) player.status = SAVE;
				else if (player.status == PAUSE_HOME) player.status = SAVE_HOME;
			}
			/*	else if (mousePos.x > 240 && mousePos.x < 400 && mousePos.y > 250 && mousePos.y < 312) {
					instructions++;
					player.status = CONTROLS;
				}*/
			else if (mousePos.x > 240 && mousePos.x < 400 && mousePos.y > 250 && mousePos.y < 312) {
				if (player.status == PAUSE) player.status = LOAD;
				else if (player.status == PAUSE_HOME) player.status = LOAD_HOME;
			}
			else if (mousePos.x > 240 && mousePos.x < 400 && mousePos.y > 316 && mousePos.y < 375) {
				zabalik = 0;
			}
		}
		else if (player.status == SAVE || player.status == SAVE_HOME) {
			if (mousePos.x > 240 && mousePos.x < 400 && mousePos.y > 183 && mousePos.y < 245) {
				save(1);
			}
			else if (mousePos.x > 240 && mousePos.x < 400 && mousePos.y > 250 && mousePos.y < 312) {
				save(2);
			}
			else if (mousePos.x > 280 && mousePos.x < 360 && mousePos.y > 331 && mousePos.y < 360) {
				if (player.status == SAVE) player.status = PAUSE;
				else if (player.status == SAVE_HOME) player.status = PAUSE_HOME;
			}
		}
		else if (player.status == LOAD || player.status == LOAD_HOME) {
			if (mousePos.x > 240 && mousePos.x < 400 && mousePos.y > 183 && mousePos.y < 245) {
				load(1);
			}
			else if (mousePos.x > 240 && mousePos.x < 400 && mousePos.y > 250 && mousePos.y < 312) {
				load(2);
			}
			else if (mousePos.x > 280 && mousePos.x < 360 && mousePos.y > 331 && mousePos.y < 360) {
				if (player.status == LOAD) player.status = PAUSE;
				else if (player.status == LOAD_HOME) player.status = PAUSE_HOME;
			}
		}
		else if (hoveringInv != -1 && checkHover(hoveringInv)) {
			//Dendan badago, erosi
			if (inventories[INV_SHOP].open && hoveringInv == INV_SHOP) {
				buyItem(showingItem);
			}
			else if (!getQuestMenuState()) {
				//Arratoiareki objektua arrastratzen ari bada: 
				if (hoveringItem.ID != 0) {
					//Inbentarioko hutsunean utzi eta hutsune hartan objekturik bazegoen, hartu.
					if (inventories[hoveringInv].items[showingItem].ID != hoveringItem.ID) {
						struct Item tmpItem = hoveringItem;
						hoveringItem = inventories[hoveringInv].items[showingItem];
						inventories[hoveringInv].items[showingItem] = tmpItem;
					}
					else {
						//Item mota bera bada, bi 'stack'-ak batu
						inventories[hoveringInv].items[showingItem].quantity += hoveringItem.quantity;
						hoveringItem.ID = 0;
					}
				}
				else {
					//Ez badago item-ik saguan, hutsuneko item-a hartu.
					hoveringItem = pickHovering();
				}
			}
		}
		else {
			if (player.status == PLAYING) {
				if (getQuestMenuState()) {
					//Misioen karteleko menuarekin interakzioa
					interactQuestMenu();
				}
				else {
					int i = 0, soil = 0;
					//Jokalaria landatu daitekeen lursail parean dagoen begiratu
					while (i < 49 && !soil) {
						if (player.facingTile == plantable_ID[i]) soil = 1;
						i++;
					}
					//Landagarria den lursai parean badago
					if (soil) {
						if (tiles[player.facingTile].plant.arado) {
							if (tiles[player.facingTile].plant.seed == NONE) {
								//Saguan hazirik badago, bertatik hartuko da hazia landatzeko
								if (hoveringItem.ID != 0 && hoveringItem.seed != 0) {
									landatu(hoveringItem.seed);
									hoveringItem.quantity--;
									if (hoveringItem.quantity < 1) hoveringItem = itemPresets[0];
								}
								//Bestela, hotbarreko aukeratutako slot-etik hartuko da hazia, egoten bada
								else if (inventories[INV_HOTBAR].items[player.hotbarSlot].ID != 0 && inventories[INV_HOTBAR].items[player.hotbarSlot].seed != 0) {
									landatu(inventories[INV_HOTBAR].items[player.hotbarSlot].seed);
									inventories[INV_HOTBAR].items[player.hotbarSlot].quantity--;
									if (inventories[INV_HOTBAR].items[player.hotbarSlot].quantity < 1) inventories[INV_HOTBAR].items[player.hotbarSlot] = itemPresets[0];
								}
							}
							//Guztiz hazi den landare baten parean badago, uzta bildu.
							if (tiles[player.facingTile].plant.seed != 0 && tiles[player.facingTile].plant.stage == 2) {
								harvest(player.facingTile);
							}
							//Arratoian ura badago, lurra ureztatu.
							else if (hoveringItem.ID == 3) {
								water(player.facingTile);
								hoveringItem.quantity--;
								if (hoveringItem.quantity < 1) hoveringItem = itemPresets[2];
							}
							//Bestela hotbarretik hartu ura
							else if (inventories[INV_HOTBAR].items[player.hotbarSlot].ID == 3) {
								water(player.facingTile);
								inventories[INV_HOTBAR].items[player.hotbarSlot].quantity--;
								if (inventories[INV_HOTBAR].items[player.hotbarSlot].quantity < 1) inventories[INV_HOTBAR].items[player.hotbarSlot] = itemPresets[2];

							}
							//Ongarria arratoian badago bertatik erabili
							else if (hoveringItem.ID == 16 && tiles[player.facingTile].plant.seed != 0) {
								fertilize(player.facingTile);
								hoveringItem.quantity--;
								if (hoveringItem.quantity < 1) hoveringItem = itemPresets[0];
							}
							//Bestela hotbarretik erabiliko da
							else if (inventories[INV_HOTBAR].items[player.hotbarSlot].ID == 16 && tiles[player.facingTile].plant.seed != 0) {
								fertilize(player.facingTile);
								inventories[INV_HOTBAR].items[player.hotbarSlot].quantity--;
								if (inventories[INV_HOTBAR].items[player.hotbarSlot].quantity < 1) inventories[INV_HOTBAR].items[player.hotbarSlot] = itemPresets[0];
							}
						}
						//Aitzurra badauka aukeratuta, lurra goldatu
						else if (inventories[INV_HOTBAR].items[player.hotbarSlot].ID == 1) {
							arar(player.facingTile);
						}

					}
					else {
						//Ez bada inbentario baten klikatzen eta landatu daiteken lur-eremu parean ez badago arratoiko objektua lurrera bota.
						dropHoveringItem();
					}
				}
			}
			else if (player.status == HOME) {
				//Inbentarioan ez bada klik egiten lurrera bota arratoiko item-a
				dropHoveringItem();
			}
		}
		break;
	}
	return zabalik;
}

//Arratoiaren gurpila mugitzerakoan hotbarreko aukeratutako slot-a aldatu.
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

//Pausa menuan sartu eta bertatik irtetzeko
void pause() {
	if (player.status == PLAYING) player.status = PAUSE;
	else if (player.status == HOME)	player.status = PAUSE_HOME;
	else if (player.status == PAUSE) player.status = PLAYING;
	else if (player.status == PAUSE_HOME) player.status = HOME;
	else if (player.status == SAVE) player.status = PLAYING;
	else if (player.status == SAVE_HOME) player.status = HOME;
	else if (player.status == LOAD) player.status = PLAYING;
	else if (player.status == LOAD_HOME) player.status = HOME;
}
