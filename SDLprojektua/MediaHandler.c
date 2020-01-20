#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdlib.h>
#include <stdio.h>

SDL_Texture* loadMedia(char* src);

//PNG fitxategi batetik surface bat sortu eta bertatik textura bat itzuli.
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

//SDL_Rendercopy erabiltzeko funtzio ulergarria
void aplikatuSurface(int x, int y, int w, int h, SDL_Texture* texture, SDL_Rect* clip) {
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	offset.w = w;
	offset.h = h;

	SDL_RenderCopy(renderer, texture, clip, &offset);
	return;
}

//Erabiliko diren irudi guztiak kargatu
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
	textures[saveSurface] = loadMedia("assets/images/save.png");
	textures[loadSurface] = loadMedia("assets/images/load.png");
	startPresests();
	return;
}

//Item bat lurretik hartzerakoan ausazko SFX bat jarri SDL_mixer erabiliz
void playPickUpSFX() {
	char* src = "assets/sounds/pickUpSFX0.wav";
	int random = rand() % 3;
	switch (random) {
	case 0: src = "assets/sounds/pickUpSFX0.wav"; break;
	case 1: src = "assets/sounds/pickUpSFX1.wav"; break;
	case 2: src = "assets/sounds/pickUpSFX2.wav"; break;
	}
	Mix_Chunk* pickUpSFX = Mix_LoadWAV_RW(SDL_RWFromFile(src, "rb"), 1);
	Mix_PlayChannelTimed(-1, pickUpSFX, 0, -1);
	return;
}

//Putzutik ura hartzean SFX bat jarri SDL_mixer erabiliz
void playWellWaterSFX() {
	char* src = "assets/sounds/wellWaterSFX.wav";
	Mix_Chunk* waterSFX = Mix_LoadWAV_RW(SDL_RWFromFile(src, "rb"), 1);
	Mix_PlayChannelTimed(-1, waterSFX, 0, -1);
	//Mix_FreeChunk(waterSFX);
	return;
}

//SDL_mixer erabiliz ausazko abesti bat jarri jadanik musikarik ez badago. Fade in efektua du hasieran.
void playMusic() {
	if (!Mix_PlayingMusic()) {
		char* src[] = { "assets/sounds/Fondo1.wav", "assets/sounds/Fondo2.wav", "assets/sounds/Fondo3.wav", "assets/sounds/Fondo4.wav", "assets/sounds/Fondo5.wav", "assets/sounds/Fondo6.wav" };
		int track = rand() % 6;
		Mix_Music* music = Mix_LoadMUS(src[track]);
		Mix_FadeInMusic(music, 1, 2000);
	}
	return;
}

//Jokoko elementuak marrazteko funtzioa.
void marraztu() {
	aplikatuSurface(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, textures[bgSurface], &camera);
	marraztuTiles();
	marraztuDroppedItems(0);
	SDL_Rect clip = { 0, 126, 60, 61 };
	drawShop();
	if (player.y > 64 - 5) {
		aplikatuSurface(13 * TILE_SIZE - camera.x, 2 * TILE_SIZE - camera.y + 1, 60, 61, textures[obstacleSurface], &clip);
		drawPlayer();
	}
	else {
		drawPlayer();
		aplikatuSurface(13 * TILE_SIZE - camera.x, 2 * TILE_SIZE - camera.y + 1, 60, 61, textures[obstacleSurface], &clip);
	}

	marraztuDroppedItems(1);
	//Fence
	clip.y = 186;
	clip.w = 172;
	clip.h = 402;
	aplikatuSurface(0 - camera.x, 8 * TILE_SIZE - camera.y, 243, 574, textures[obstacleSurface], &clip);
	//Cow
	clip.y = 0;
	clip.w = 121;
	clip.h = 90;
	aplikatuSurface(TILE_SIZE - camera.x, 9 * TILE_SIZE - camera.y, 121, 90, textures[obstacleSurface], &clip);
	//Pig
	clip.y = 91;
	clip.w = 49;
	clip.h = 35;
	aplikatuSurface(TILE_SIZE * 2 - camera.x, 13 * TILE_SIZE - camera.y, 49, 35, textures[obstacleSurface], &clip);
	drawDayFilter();
	marraztuEnergy();
	drawClock();
	for (int i = 0; i < 4; i++) showInv(i);
	marraztuInvTag(getHoveringInv());
	if (hoveringItem.ID != 0) marraztuHoveringItem();
	if (!getQuestMenuState())showCurrentQuest();
	showQuestMenu();
	questCompleteAnim(deltaTime);
	return;
}