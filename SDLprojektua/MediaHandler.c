#include "SDL.h"
#include "SDL_image.h"
#include "funtzioak.h"
#include "objektuak.h"
#include "SDL_mixer.h"
#include <stdio.h>

//enum SURFACE { SURFACE_SCREEN, SURFACE_BACKGROUND, SURFACE_PLAYER, SURFACE_EXTRA, SURFACE_ITEMS, SURFACE_LAND};
void loadFiles();
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

void loadFiles() {
	surface[bgSurface] = loadMedia("assets/images/bg.png");
	surface[playerSurface] = loadMedia("assets/images/player.png");
	surface[itemsSurface] = loadMedia("assets/images/items.png");
	surface[plantsSurface] = loadMedia("assets/images/sprite.png");
	surface[pauseSurface] = loadMedia("assets/images/pause.png");
	surface[homeSurface] = loadMedia("assets/images/home.png");
	surface[fenceSurface] = loadMedia("assets/images/fence.png");
	surface[cowSurface] = loadMedia("assets/images/cow.png");
	surface[pigSurface] = loadMedia("assets/images/pig.png");
	surface[signSurface] = loadMedia("assets/images/sign.png");
	surface[HUDSurface] = loadMedia("assets/images/HUD.png");
	surface[firmaSurface] = loadMedia("assets/images/FIRMA.png");
	surface[menuSurface] = loadMedia("assets/images/ANCIENTGARDEN.png");
}

void playPickUpSFX() {
	char* src = "assets/sounds/pickUpSFX0.wav";
	int random = rand() % 3;
	switch (random) {
	case 0: src = "assets/sounds/pickUpSFX0.wav"; break;
	case 1: src = "assets/sounds/pickUpSFX1.wav"; break;
	case 2: src = "assets/sounds/pickUpSFX2.wav"; break;
	}
	Mix_Chunk* pickUpSFX = Mix_LoadWAV_RW(SDL_RWFromFile(src, "rb"), 1);
	int x = Mix_PlayChannelTimed(-1, pickUpSFX, 0, -1);
	return;
}