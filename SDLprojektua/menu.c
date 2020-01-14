#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>
#include <time.h>

double tiempof = 0;
void menu(double deltaTime)
{
	if (tiempof < 2) {
		tiempof += deltaTime;
		SDL_Rect firma = { 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };
		aplikatuSurface(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, textures[firmaSurface], &firma);

	} else{

		SDL_Rect background = { 0,0,SCREEN_WIDTH - 2, SCREEN_HEIGHT };
		aplikatuSurface(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, textures[menuSurface], &background);

		SDL_Rect button = { 638,0,160,61 };
		aplikatuSurface(239, 200, 160, 61, textures[menuSurface], &button);

		for (int i = 0; i < 2; i++)
		{
			SDL_Rect button2 = { 638,62,120,52 };
			aplikatuSurface(259, 271 + 60 * i, 120, 52, textures[menuSurface], &button2);
		}

		SDL_Rect clip = { 638, 114, 64, 48 };
		for (int i = 0; i < 3; i++) {
			clip.y = 114 + 48 * i;
			aplikatuSurface(150 + 128 * i, 432, 64, 48, textures[menuSurface], &clip);
		}
	}
	return;
}

int checkIfClicking(SDL_Rect* button) {
	int success = 0;
	if (mousePos.x > button->x&&
		mousePos.x < button->x + button->w &&
		mousePos.y > button->y&&
		mousePos.y < button-> y + button->h) {
		success = 1;
	}
	return success;
}