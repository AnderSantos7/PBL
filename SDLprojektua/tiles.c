#include "SDL.h"
#include "SDL_image.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>
#include <stdlib.h>

int i = 0;

void updatePlants(int i, double deltaTime);
void updateWater(int i, double deltaTime);

//Jokalariaren pareko lur-eremuan hazi bat landatu.
void landatu(int seed) {
	if (tiles[player.facingTile].plant.arado && tiles[player.facingDirection].plant.seed == NONE) {
		tiles[player.facingTile].plant.seed = seed;
	}
	restaEnergy();

	checkQuestCompletion(PLANT, seed, 1);
	return;
}

//Guztiz hazi den landare bat bildu eta landarearen datuak erreseteatu.
void harvest(int tile) {
	if (tiles[tile].plant.seed != 0 && tiles[tile].plant.stage == 2) {
		int seedAmmount = 0, harvest = 0, seed = 0;
		seedAmmount = rand() % 3 + 1;
		harvest = seedToItem(tiles[tile].plant.seed);
		seed = harvest + 1;
		dropItem(tile, seed, seedAmmount);
		dropItem(tile, harvest, 1);
		tiles[tile].plant.seed = NONE;
		tiles[tile].plant.stage = 0;
		tiles[tile].plant.time = 0;

		checkQuestCompletion(HARVEST, harvest, 1);
	}
}

//Jokalariaren pareko landarea ureztatu.
void water(int tile) {
	if(!tiles[tile].plant.water) checkQuestCompletion(WATER, tiles[tile].plant.seed, 1);
	tiles[tile].plant.water = 1;
	tiles[tile].plant.lastWater = 0;
	restaEnergy();

	return;
}

//Jokalariaren pareko lur-eremua goldatu
void arar(int tile) {
	tiles[tile].plant.arado = 1;
	restaEnergy();

	checkQuestCompletion(ARAR, 0, 1);
	return;
}

//Jokalariaren pareko landarea ongarritu
void fertilize(int tile) {
	tiles[tile].plant.time += 450;

	checkQuestCompletion(FERTILIZAR, tiles[tile].plant.seed, 1);
	return;
}

//Jokalaria zein 'tile'-en parean dagoen kalkulatzeko.
int getFacingTileId() {
	int ID = player.tile;
	switch (player.facingDirection) {
	case DIR_LEFT:
		ID = player.tile - 1;
		break;
	case DIR_RIGHT:
		ID = player.tile + 1;
		break;
	case DIR_UP:
		ID = player.tile - 16;
		break;
	case DIR_DOWN:
		ID = player.tile + 16;
		break;
	}
	return ID;
}

//ID zenbaki batekin tile baten kuadrikulako X eta Y posizioak kalkulatu
struct posCoord getTilePosFromId(int ID) {
	struct posCoord pos;
	pos.x = ID % GRID_SIZE;
	pos.y = ID / GRID_SIZE;
	return pos;
}

//X eta Y posizio batekin tile baten ID-a kalkulatu. Aurreko funtzioaren alderantzizkoa.
int getTileFromPos(int x, int y) {
	int ID = 0;
	ID = (y / 64) * 16 + (x / 64);
	return ID;
}

//Landatu daiteken eremuan dauden elementuak marraztu.
void marraztuTiles() {
	int i = 0;
	SDL_Rect pos;
	SDL_Rect clip;
	clip.w = TILE_SIZE;
	clip.h = TILE_SIZE;
	for (i = 0; i < 49; i++) {
		pos.x = tiles[plantable_ID[i]].x * TILE_SIZE;
		pos.y = tiles[plantable_ID[i]].y * TILE_SIZE;
		if (tiles[plantable_ID[i]].plant.arado) {
			switch (tiles[plantable_ID[i]].plant.seed) {
			case NONE:
				clip.x = tiles[plantable_ID[i]].plant.water * TILE_SIZE;
				clip.y = 0;
				break;
			default:
				clip.x = (tiles[plantable_ID[i]].plant.stage + 3 * tiles[plantable_ID[i]].plant.water) * TILE_SIZE;
				clip.y = tiles[plantable_ID[i]].plant.seed * TILE_SIZE;
			}
			aplikatuSurface(pos.x - camera.x, pos.y - camera.y, 64, 64, textures[plantsSurface], &clip);
		}
		if (plantable_ID[i] == player.facingTile) {
			clip.x = 2 * TILE_SIZE;
			clip.y = 0;
			aplikatuSurface(pos.x - camera.x, pos.y - camera.y, 64, 64, textures[plantsSurface], &clip);
		}
	}
}

//Landatu daitezken 'tile'-ak aktualizatu. Landareak hazi eta lehortu.
void updateTiles(double deltaTime) {
	for (int i = 0; i < 49; i++) {
		if (tiles[plantable_ID[i]].plant.arado) {
			updatePlants(i, deltaTime);
			updateWater(i, deltaTime);
		}
	}
	return;
}

//Landarearen 'stage'-a eguneratzeko funtzioa. Denboraren arabera 'stage' desberdina izango du. Bigarrenean guztiz hazita egongo da.
void updatePlants(int i, double deltaTime) {
	if (tiles[plantable_ID[i]].plant.seed != NONE) {
		tiles[plantable_ID[i]].plant.time += deltaTime;
		if (tiles[plantable_ID[i]].plant.time > 150) {
			if (tiles[plantable_ID[i]].plant.time > 450) {
				tiles[plantable_ID[i]].plant.stage = 2;
			}
			else {
				tiles[plantable_ID[i]].plant.stage = 1;
			}
		}
	}
	return;
}

//Lursail 'tile' baten lehortasuna eguneratu. Ureztatuta badago, denborarekin lehortu egingo da. Lehortuta badago, denborarekin goldatuta egoteari utziko dio.
void updateWater(int i, double deltaTime) {
	if (tiles[plantable_ID[i]].plant.water) {
		tiles[plantable_ID[i]].plant.lastWater += deltaTime;
		if (tiles[plantable_ID[i]].plant.lastWater > 250) {
			tiles[plantable_ID[i]].plant.water = 0;
			tiles[plantable_ID[i]].plant.lastWater = 0;
		}
	}else if (!tiles[plantable_ID[i]].plant.water) {
		tiles[plantable_ID[i]].plant.lastWater += deltaTime;
		if (tiles[plantable_ID[i]].plant.lastWater >= 350) {
			tiles[plantable_ID[i]].plant.time = 0;
			tiles[plantable_ID[i]].plant.seed = NONE;
			tiles[plantable_ID[i]].plant.arado = 0;
			tiles[plantable_ID[i]].plant.lastWater = 0;
			tiles[plantable_ID[i]].plant.stage = 0;
		}
	}
	return;
}