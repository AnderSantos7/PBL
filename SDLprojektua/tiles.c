#include "SDL.h"
#include "SDL_image.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>

int i = 0;

void landatu(){
	if (!tiles[player.facingTile].plant.arado){
		tiles[player.facingTile].plant.arado = 1;
	}
	else if (tiles[player.facingTile].plant.stage == 2) {
		//changeInv(i);
		i++;
		tiles[player.facingTile].plant.time = 0;
		tiles[player.facingTile].plant.stage = NONE;
		tiles[player.facingTile].plant.seed = NONE;
	}
	else if (tiles[player.facingTile].plant.arado && !tiles[player.facingTile].plant.water) {
		tiles[player.facingTile].plant.lastWater = NONE;
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

void marraztuTiles(SDL_Surface* spriteSheetSurface, SDL_Surface* screenSurface){
	int i = 0;
	SDL_Rect pos;
	SDL_Rect clip;
	clip.w = TILE_SIZE;
	clip.h = TILE_SIZE;
	for (i = 0; i < 49; i++){
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
			aplikatuSurface(pos.x - camera.x, pos.y - camera.y, spriteSheetSurface, screenSurface, &clip);
		}
		if (plantable_ID[i] == player.facingTile) {
			clip.x = 2 * TILE_SIZE;
			clip.y = 0;
			aplikatuSurface(pos.x - camera.x, pos.y - camera.y, spriteSheetSurface, screenSurface, &clip);
		}
	}
}

void updateTiles(double deltaTime) {
	for (int i = 0; i < 49; i++) {
		if (tiles[plantable_ID[i]].plant.seed != NONE) {
			tiles[plantable_ID[i]].plant.time += deltaTime;
			if (tiles[plantable_ID[i]].plant.time > 5) {
				if (tiles[plantable_ID[i]].plant.time > 10) {
					tiles[plantable_ID[i]].plant.stage = 2;
				}
				else {
					tiles[plantable_ID[i]].plant.stage = 1;
				}
			}
		}
		if (tiles[plantable_ID[i]].plant.water) {
			tiles[plantable_ID[i]].plant.lastWater += deltaTime;
			if (tiles[plantable_ID[i]].plant.lastWater > 5) {
				tiles[plantable_ID[i]].plant.water = NONE;
			}
		}
		else if (!tiles[plantable_ID[i]].plant.water) {
			tiles[plantable_ID[i]].plant.lastWater += deltaTime;
			if (tiles[plantable_ID[i]].plant.lastWater > 10) {
				tiles[plantable_ID[i]].plant.time = NONE;
				tiles[plantable_ID[i]].plant.seed = NONE;
				tiles[plantable_ID[i]].plant.arado = NONE;
				tiles[plantable_ID[i]].plant.lastWater = NONE;
				tiles[plantable_ID[i]].plant.stage = NONE;
			}
		}
	}
}