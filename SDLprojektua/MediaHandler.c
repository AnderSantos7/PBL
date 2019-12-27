#include "SDL.h"
#include "SDL_image.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>

//enum SURFACE { SURFACE_SCREEN, SURFACE_BACKGROUND, SURFACE_PLAYER, SURFACE_EXTRA, SURFACE_ITEMS, SURFACE_LAND};

SDL_Surface* loadMedia(char* src) {	
	SDL_Surface* surface = NULL;
	SDL_RWops* rwop;
	rwop = SDL_RWFromFile(src, "rb");
	surface = IMG_LoadPNG_RW(rwop);
	if (surface == NULL) {
		printf("Ezin izan da %s irudia kargatu. SDL_Error: %s\n", src, SDL_GetError());
	}
	return surface;
}

void aplikatuSurface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) {
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;

	SDL_BlitSurface(source, clip, destination, &offset);
}

//void loadFiles() {
//	surfaces[SURFACE_BACKGROUND] = loadMedia("assests/images/bg.png");
//	//bgSurface = loadMedia("assets/images/bg.png");
//	//signSurface = loadMedia("assets/images/sign.png");
//	//fenceSurface = loadMedia("assets/images/fence.png");
//	//playerSurface = loadMedia("assets/images/player.png");
//	//cowSurface = loadMedia("assets/images/cow.png");
//	//pigSurface = loadMedia("assets/images/pig.png");
//	//sartuSurface = loadMedia("assets/images/sartu.png");
//	//aradoSurface = loadMedia("assets/images/ARADO.png");
//}
