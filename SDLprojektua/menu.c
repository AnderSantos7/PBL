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
		aplikatuSurface(0, 0, surface[firmaSurface], surface[screenSurface], &firma);

	} else{

		SDL_Rect background = { 0,0,SCREEN_WIDTH - 2, SCREEN_HEIGHT };
		aplikatuSurface(0, 0, surface[menuSurface], surface[screenSurface], &background);

		SDL_Rect button = { 638,0,160,61 };
		aplikatuSurface(239, 200, surface[menuSurface], surface[screenSurface], &button);

		for (int i = 0; i < 2; i++)
		{
			SDL_Rect button2 = { 638,62,120,52 };
			aplikatuSurface(259, 271 + 60 * i, surface[menuSurface], surface[screenSurface], &button2);
		}

		SDL_Rect clip = { 638, 114, 64, 48 };
		for (int i = 0; i < 3; i++) {
			clip.y = 114 + 48 * i;
			aplikatuSurface(150 + 128 * i, 432, surface[menuSurface], surface[screenSurface], &clip);
		}
	}

	SDL_UpdateWindowSurface(win);

	return;
}