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