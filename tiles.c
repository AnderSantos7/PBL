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