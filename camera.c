#include "SDL.h"
#include "SDL_image.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>

SDL_Rect createCamera() {
	SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	return camera;
}

SDL_Rect centerCameraInPlayer(SDL_Rect camera) {	
	camera.x = (player.x + player.w / 2) - SCREEN_WIDTH / 2;
	camera.y = (player.y + player.h / 2) - SCREEN_HEIGHT / 2;

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
	return camera;
}