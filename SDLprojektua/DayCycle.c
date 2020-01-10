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
	if (dayTime >= 100) dayTime = 0;

	return;
}

void drawDayFilter() {
	int color = dayTime / 500 * 255;
	SDL_Rect rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, color);
	SDL_RenderFillRect(renderer, &rect);
	return;
}