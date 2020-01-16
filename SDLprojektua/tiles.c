#include "SDL.h"
#include "SDL_image.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>
#include <stdlib.h>

int i = 0;

void updatePlants(int i, double deltaTime);
void updateWater(int i, double deltaTime);

void landatu(int seed) {
	if (tiles[player.facingTile].plant.arado && tiles[player.facingDirection].plant.seed == NONE) {
		tiles[player.facingTile].plant.seed = seed;
	}
	restaEnergy();

	checkQuestCompletion(PLANT, seed, 1);
	return;
}

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

void water(int tile) {
	if(!tiles[tile].plant.water) checkQuestCompletion(WATER, tiles[tile].plant.seed, 1);
	tiles[tile].plant.water = 1;
	tiles[tile].plant.lastWater = 0;
	restaEnergy();

	return;
}

void arar(int tile) {
	tiles[tile].plant.arado = 1;
	restaEnergy();

	checkQuestCompletion(ARAR, 0, 1);
	return;
}

void fertilize(int tile) {
	tiles[tile].plant.time += 100;

	checkQuestCompletion(FERTILIZAR, tiles[tile].plant.seed, 1);
	return;
}

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

struct posCoord getTilePosFromId(int ID) {
	struct posCoord pos;
	pos.x = ID % GRID_SIZE;
	pos.y = ID / GRID_SIZE;
	return pos;
}

int getTileFromPos(int x, int y) {
	int ID = 0;
	ID = (y / 64) * 16 + (x / 64);
	return ID;
}

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

void updateTiles(double deltaTime) {
	if (player.sleeping)
	{
		deltaTime = 250 - deltaTime + 100;
	}
	for (int i = 0; i < 49; i++) {
		if (tiles[plantable_ID[i]].plant.arado) {
			updatePlants(i, deltaTime * 10);
			updateWater(i, deltaTime);
		}
	}
	return;
}

void updatePlants(int i, double deltaTime) {
	if (tiles[plantable_ID[i]].plant.seed != NONE) {
		tiles[plantable_ID[i]].plant.time += deltaTime;
		if (tiles[plantable_ID[i]].plant.time > 50) {
			if (tiles[plantable_ID[i]].plant.time > 100) {
				tiles[plantable_ID[i]].plant.stage = 2;
			}
			else {
				tiles[plantable_ID[i]].plant.stage = 1;
			}
		}
	}
	return;
}

void updateWater(int i, double deltaTime) {
	if (tiles[plantable_ID[i]].plant.water) {
		tiles[plantable_ID[i]].plant.lastWater += deltaTime;
		if (tiles[plantable_ID[i]].plant.lastWater > 50) {
			tiles[plantable_ID[i]].plant.water = 0;
			tiles[plantable_ID[i]].plant.lastWater = 0;
		}
	}else if (!tiles[plantable_ID[i]].plant.water) {
		tiles[plantable_ID[i]].plant.lastWater += deltaTime;
		if (tiles[plantable_ID[i]].plant.lastWater >= 100) {
			tiles[plantable_ID[i]].plant.time = 0;
			tiles[plantable_ID[i]].plant.seed = NONE;
			tiles[plantable_ID[i]].plant.arado = 0;
			tiles[plantable_ID[i]].plant.lastWater = 0;
			tiles[plantable_ID[i]].plant.stage = 0;
		}
	}
	return;
}