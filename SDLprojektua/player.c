#include "SDL.h"
#include "SDL_image.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>

struct Player createPlayer() {
	struct Player player;
	player.x = 0;
	player.y = 0;
	player.w = TILE_SIZE;
	player.h = 2 * TILE_SIZE;
	player.speed = 250;
	player.movingRight = 0;  
	player.movingLeft = 0;
	player.movingUp = 0;
	player.movingDown = 0;
	player.facingDirection = DIR_DOWN;
	return player;
}

void movePlayer(double deltaTime) {
	float multiplier = 1;
	int bidean = 0, i = 0;

	player.tile = getTileFromPos(player.x, player.y + TILE_SIZE);
	int x;
	int y;
	while (!bidean && i < 33)
	{
		if (tiles[camino_ID[i]].x * TILE_SIZE < player.x + player.w &&
			tiles[camino_ID[i]].x * TILE_SIZE + TILE_SIZE > player.x &&
			tiles[camino_ID[i]].y * TILE_SIZE < player.y + TILE_SIZE &&
			tiles[camino_ID[i]].y * TILE_SIZE + TILE_SIZE > player.y + TILE_SIZE)
		{
			bidean = 1;
			multiplier = 1.3;
		}
		i++;
	}

	if (player.movingRight) {
		player.x += player.speed * deltaTime * multiplier;
		if (player.x > LEVEL_SIZE - player.w) {
			player.x = LEVEL_SIZE - player.w;
		}
	}
	if (player.movingLeft) {
		player.x -= player.speed * deltaTime * multiplier;
		if (player.x < 0) {
			player.x = 0;
		}
	}
	if (player.movingUp) {
		player.y -= player.speed * deltaTime * multiplier;
		if (player.y < 0) {
			player.y = 0;
		}
	}
	if (player.movingDown) {
		player.y += player.speed * deltaTime * multiplier;
		if (player.y > LEVEL_SIZE - player.h) {
			player.y = LEVEL_SIZE - player.h;
		}
	}
	return;
}

void drawPlayer(SDL_Rect camera, SDL_Surface* surface, SDL_Surface* screenSurface) {
	aplikatuSurface(player.x - camera.x, player.y - camera.y, surface, screenSurface, NULL);
	return;
}

