#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

double dayTime = 0;
void updateDay(double deltaTime) {
	dayTime += deltaTime;
	if (dayTime >= 250) {
		sleep();
	}

	return;
}

void drawDayFilter() {
	if (dayTime >= 150) {
		int alpha = (dayTime - 150) / 100 * 128;
		if (dayTime >= 200) {
			alpha -= (dayTime - 200) / 50 * 128;
		}
		SDL_SetRenderDrawColor(renderer, 255, 63, 0, alpha);
		SDL_RenderFillRect(renderer, NULL);
	}

	if (dayTime >= 200) {
		int alpha = (dayTime - 200) / 50 * 192;
		SDL_SetRenderDrawColor(renderer, 0, 0, 32, alpha);
		SDL_RenderFillRect(renderer, NULL);
	}
	return;
}
	
void drawClock() {
	int xOffset = 16;
	SDL_Rect clip = {dayTime * 2, 436, 200, 40};
	aplikatuSurface(SCREEN_WIDTH - 200 - xOffset, 16, clip.w, clip.h, textures[HUDSurface], &clip);
	clip.x = 0;
	clip.y = 476;
	clip.w = 200;
	clip.h = 57;
	aplikatuSurface(SCREEN_WIDTH - 200 - xOffset, 11, clip.w, clip.h, textures[HUDSurface], &clip);
	return;
}

void resetDay() {
	updateTiles(250 - dayTime + 100);
	dayTime = 0;
	return;
}

double getDayTime() {
	return (int)dayTime;
}

void setDayTime(int time) {
	dayTime = time;
}