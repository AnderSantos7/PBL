#include "SDL.h"
#include "SDL_image.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>

void landatu()
{
	if (tiles[player.facingTile].plant.seed == SEED_NONE)
	{
		tiles[player.facingTile].plant.seed = SEED_ARADO;
	}
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

void marraztuTiles(SDL_Surface* aradoSurface , SDL_Surface* screenSurface)
{
	int i = 0;
	SDL_Rect rect;
	for (i = 0; i < 49 ; i++)
	{
		rect.x = tiles[plantable_ID[i]].x * TILE_SIZE;
		rect.y = tiles[plantable_ID[i]].y * TILE_SIZE;
		if (tiles[plantable_ID[i]].plant.seed == SEED_ARADO)
		{
			aplikatuSurface(rect.x - camera.x, rect.y - camera.y, aradoSurface, screenSurface, NULL);
		}
	}
}

void updateTiles(double deltaTime) {
	for (int i = 0; i < 256; i++) {
		if (tiles[i].plant.seed != (SEED_ARADO && SEED_NONE)) {
			tiles[i].plant.time += deltaTime;
			if (tiles[i].plant.time > 1000) {
				if (tiles[i].plant.time > 3000) {
					tiles[i].plant.stage = 2;
				}
				else {
					tiles[i].plant.stage = 1;
				}
			}
			if (tiles[i].plant.water) {
				tiles[i].plant.lastWater += deltaTime;
				if (tiles[i].plant.lastWater > 500) {
					//Secar
				}
			}
		}
	}
}