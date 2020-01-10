#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdlib.h>
#include <stdio.h>

//enum SURFACE { SURFACE_SCREEN, SURFACE_BACKGROUND, SURFACE_PLAYER, SURFACE_EXTRA, SURFACE_ITEMS, SURFACE_LAND};
SDL_Texture* loadMedia(char* src);

SDL_Texture* loadMedia(char* src) {
	SDL_Surface* s = NULL;
	SDL_RWops* rwop;
	rwop = SDL_RWFromFile(src, "rb");
	s = IMG_LoadPNG_RW(rwop);
	if (s == NULL) {
		printf("Ezin izan da %s irudia kargatu. SDL_Error: %s\n", src, SDL_GetError());
	}
	SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);

	SDL_FreeSurface(s);
	return t;
}

void aplikatuSurface(int x, int y, int w, int h, SDL_Texture* texture, SDL_Rect* clip) {
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	offset.w = w;
	offset.h = h;

	SDL_RenderCopy(renderer, texture, clip, &offset);
	return;
}

void loadFiles() {
	textures[bgSurface] = loadMedia("assets/images/bg.png");
	textures[playerSurface] = loadMedia("assets/images/player.png");
	textures[itemsSurface] = loadMedia("assets/images/items.png");
	textures[plantsSurface] = loadMedia("assets/images/sprite.png");
	textures[pauseSurface] = loadMedia("assets/images/pause.png");
	textures[homeSurface] = loadMedia("assets/images/home.png");
	textures[obstacleSurface] = loadMedia("assets/images/obstacles.png");
	textures[HUDSurface] = loadMedia("assets/images/HUD.png");
	textures[firmaSurface] = loadMedia("assets/images/FIRMA.png");
	textures[menuSurface] = loadMedia("assets/images/ANCIENTGARDEN.png");
	textures[cofreSurface] = loadMedia("assets/images/cofre.png");
	
	return;
}

void playPickUpSFX() {
	char* src = "assets/sounds/pickUpSFX0.wav";
	int random = rand() % 3;
	switch (random) {
	case 0: src = "assets/sounds/pickupsfx0.wav"; break;
	case 1: src = "assets/sounds/pickupsfx1.wav"; break;
	case 2: src = "assets/sounds/pickupsfx2.wav"; break;
	}
	Mix_Chunk* pickUpSFX = Mix_LoadWAV_RW(SDL_RWFromFile(src, "rb"), 1);
	Mix_PlayChannelTimed(-1, pickUpSFX, 0, -1);
	Mix_FreeChunk(pickUpSFX);
	return;
}

void playWellWaterSFX() {
	char* src = "assets/sounds/wellWaterSFX.wav";
	Mix_Chunk* waterSFX = Mix_LoadWAV_RW(SDL_RWFromFile(src, "rb"), 1);
	Mix_PlayChannelTimed(-1, waterSFX, 0, -1);
	//Mix_FreeChunk(waterSFX);
	return;
}