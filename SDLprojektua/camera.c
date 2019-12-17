#include "SDL.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>

SDL_Rect createCamera() {
	SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	return camera;
}

void centerCameraInPlayer(SDL_Rect camera, struct Player player) {
	
	const int LEVEL_SIZE = 1024;
	camera.x = (player.x + player.w / 2) - 640;
	camera.y = (player.y + player.h / 2) - 480;

	if (camera.x < 0) {
		camera.x = 0;
	}
	if (camera.y < 0) {
		camera.y = 0;
	}
	if (camera.x > LEVEL_SIZE - camera.w) {
		camera.x = LEVEL_SIZE - camera.w;
	}
	if (camera.y > LEVEL_SIZE - camera.h) {
		camera.y = LEVEL_SIZE - camera.h;
	}
	return;
}