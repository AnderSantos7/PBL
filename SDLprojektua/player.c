#include "SDL.h"
#include "SDL_image.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>

struct Player createPlayer() {
	struct Player player;
	player.x = 64*2;
	player.y = 64;
	player.w = TILE_SIZE;
	player.h = 2 * TILE_SIZE;
	player.speed = 300;
	player.movingRight = 0;
	player.movingLeft = 0;
	player.movingUp = 0;
	player.movingDown = 0;
	player.facingDirection = DIR_DOWN;
	player.status = PLAYING;
	player.timer = 0;
	return player;
}

void movePlayer(double deltaTime) {
	float multiplier = 1;
	int bidean = 0, i = 0;

	player.tile = getTileFromPos(player.x + player.w / 2, player.y + 2 * TILE_SIZE);
	if (player.status == PLAYING) {
		while (!bidean && i < 33){
			if (tiles[camino_ID[i]].x * TILE_SIZE < player.x + player.w - 20 &&
				tiles[camino_ID[i]].x * TILE_SIZE + TILE_SIZE > player.x + 20 &&
				tiles[camino_ID[i]].y * TILE_SIZE < player.y + 2 * TILE_SIZE &&
				tiles[camino_ID[i]].y * TILE_SIZE + TILE_SIZE > player.y + 2 * TILE_SIZE)
			{
				bidean = 1;
				multiplier = 1.2;
			}
			i++;
		}

		if (player.movingRight) {
			player.x += (int)(player.speed * multiplier * deltaTime);
			if (player.x > LEVEL_SIZE - player.w) {
				player.x = LEVEL_SIZE - player.w;
			}
		}
		if (player.movingLeft) {
			player.x -= (int)(player.speed * multiplier * deltaTime);
			if (player.x < 0) {
				player.x = 0;
			}

			if (player.y > 6 * 64 - 8 && player.x < 3 * 64 + 32) {
				player.x = 3 * 64 + 32;
			}
			else if (player.y < 64 && player.x < 4 * 64) {
				player.x = 4 * 64;
			}
		}
		if (player.movingUp) {
			player.y -= (int)(player.speed * multiplier * deltaTime);
			if (player.y < 0) {
				player.y = 0;
			}
			if (player.y + 64 * 2 < 64 * 3 && player.y + 64 * 2 > 64 * 3 - 5 && player.x > 12 * 64 && player.x < 14 * 64) {
				player.y = 64;
			}
			else if (player.y < 64 && player.x < 4 * 64) {
				player.y = 64;
				if (player.x + 32 > 64 * 2 && player.x + 32 < 64 * 3) {
					player.status = HOME;
					player.y = 480 - 64 * 2;
					player.x = 375;
				}
			}
		}
		if (player.movingDown) {
			player.y += (int)(player.speed * multiplier * deltaTime);
			if (player.y > LEVEL_SIZE - player.h) {
				player.y = LEVEL_SIZE - player.h;
			}
			if (player.y > 6 * 64 - 8 && player.x < 3 * 64 + 32 && player.x >= 0) {
				player.y = 6 * 64 - 8;
			}
			else if (player.y + 64 * 2 < 64 * 3 && player.y + 64 * 2 > 64 * 3 - 5 && player.x > 12 * 64 && player.x < 14 * 64) {
				player.y = 64 - 5;
			}
		}
	}
	//DENTRO DE CASA
	else if (player.status == HOME) {

		if (player.movingRight) {
			player.x += player.speed * deltaTime * 0.8;

			if (player.x > 640 - 64) {
				player.x = 640 - 64;
			}
			else if (player.x > 449 - 64 && player.y > 445 - 2 * 64) {
				player.x = 449 - 64;
			}
		
		}

		if (player.movingLeft) {
			player.x -= player.speed * deltaTime * 0.8;

			if (player.y < 64 && player.x < 100) {
				player.x = 100;
			}
			else if (player.x < 0) {
				player.x = 0;
			}
			else if (player.x < 352 && player.y > 445 - 2 * 64) {
				player.x = 352;
			}
		}

		if (player.movingUp) {
			player.y -= player.speed * deltaTime * 0.8;

			if (player.y < 64 && player.x < 100) {
				player.y = 64;
			}
			else if (player.x >= 100 && player.x <= 640 - 64 && player.y < 0) {
				player.y = 0;
			}
		}

		if (player.movingDown) {
			player.y += player.speed * deltaTime * 0.8;

			if (player.y > 445 - 64 * 2 && player.x > 352 - 20 && player.x < 449 - 44) {
				if (player.y > 480 - 64 * 2) {
					player.status = COLLOCATING;
				}
			}
			else if (player.y > 445 - 64*2) {
				player.y = 445 - 64*2;
			}
		}
	}
	return;
}

void drawPlayer(SDL_Rect camera, SDL_Surface* surface, SDL_Surface* screenSurface) {
	aplikatuSurface(player.x - camera.x, player.y - camera.y, surface, screenSurface, NULL);
	return;
}


