#include "SDL.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>

struct Player createPlayer() {
	struct Player player;
	player.x = 0;
	player.y = 0;
	player.w = TILE_SIZE;
	player.h = 2 * TILE_SIZE;
	player.speed = 10;
	return player;
}

struct Player movePlayer(struct Player player) {
	if (isMovingRight())player.x += player.speed;
	if (isMovingLeft())player.x -= player.speed;
	if (isMovingDown())player.y += player.speed;
	if (isMovingUp())player.y -= player.speed;
	return player;
}

void drawPlayer(SDL_Renderer* renderer, struct Player player) {
	return;
}

