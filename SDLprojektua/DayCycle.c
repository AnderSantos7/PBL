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
//Update funtzioa deitzen den bakoitzean eguneko denbora gehitu.
void updateDay(double deltaTime) {
	dayTime += deltaTime;
	if (dayTime >= 250) {
		sleep();
	}
	return;
}

//Eguneko orduaren arabera transparentziadun koloretako karratuak marraztu pantailan. Hasieran gorria eta gero gorritasuna gutxitzen da eta iluntzen hasten da.
void drawDayFilter() {
	if (dayTime >= 150) {
		int alpha = (int)(dayTime - 150) / 100 * 128;
		if (dayTime >= 200) {
			alpha -= (int)(dayTime - 200) / 50 * 128;
		}
		SDL_SetRenderDrawColor(renderer, 255, 63, 0, alpha);
		SDL_RenderFillRect(renderer, NULL);
	}

	if (dayTime >= 200) {
		int alpha = (int)(dayTime - 200) / 50 * 192;
		SDL_SetRenderDrawColor(renderer, 0, 0, 32, alpha);
		SDL_RenderFillRect(renderer, NULL);
	}
	return;
}

//Eguneko zein momentu den erakusten duen erlojua irudikatu. Honetarako lehenik eta behin erlojuaren irudiari maskara bat ezartzen zaio.
//Honen gainetik gero apaingarri bat marrazten da.
void drawClock() {
	int xOffset = 16;
	//Eguneko momentuaren arabera handitzen da maskararen x balioa.
	SDL_Rect clip = { (int)(dayTime * 2), 436, 200, 40 };
	aplikatuSurface(SCREEN_WIDTH - 200 - xOffset, 16, clip.w, clip.h, textures[HUDSurface], &clip);
	clip.x = 0;
	clip.y = 476;
	clip.w = 200;
	clip.h = 57;
	aplikatuSurface(SCREEN_WIDTH - 200 - xOffset, 11, clip.w, clip.h, textures[HUDSurface], &clip);
	return;
}

//Eguna berriz hasieratu. Lursailak eguneratzen dira gaueko 100s + lo egindako momentutik ilundu arteko denbora tartearekin.
void resetDay() {
	updateTiles(250 - dayTime + 100);
	dayTime = 0;
	return;
}

//Eguneko zein momentu den lortzeko 'getter'-a
double getDayTime() {
	return (int)dayTime;
}

//Eguneko ordua aldatzeko 'setter'-a
void setDayTime(int time) {
	dayTime = time;
}