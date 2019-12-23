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
	else if(tiles[player.facingTile].plant.seed == SEED_ARADO && !tiles[player.facingTile].plant.water){
		tiles[player.facingTile].plant.water = 1;
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

void marraztuTiles(SDL_Surface* aradoSurface, SDL_Surface* screenSurface)
{
	int i = 0;
	SDL_Rect pos;
	SDL_Rect clip;
	clip.w = TILE_SIZE;
	clip.h = TILE_SIZE;
	for (i = 0; i < 49; i++){
		pos.x = tiles[plantable_ID[i]].x * TILE_SIZE;
		pos.y = tiles[plantable_ID[i]].y * TILE_SIZE;
		switch (tiles[plantable_ID[i]].plant.seed) {
		case SEED_ARADO:
			clip.y = 0;
			if (tiles[plantable_ID[i]].plant.water) {
				clip.x = TILE_SIZE;
			}else {
				clip.x = 0;
			}
			aplikatuSurface(pos.x - camera.x, pos.y - camera.y, aradoSurface, screenSurface, &clip);
			break;
		}
		if (plantable_ID[i] == player.facingTile) {
			clip.x = 2 * TILE_SIZE;
			clip.y = 0;
			aplikatuSurface(pos.x - camera.x, pos.y - camera.y, aradoSurface, screenSurface, &clip);
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