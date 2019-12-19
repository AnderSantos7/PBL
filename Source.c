#include "SDL.h"
#include "SDL_image.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>
#include <time.h>

struct posCoord getTilePosFromId(int ID);
void marraztu();
int init();
void close();
void update(double deltaTime);
void getDeltaTime();
void loadFiles();

//temp
void marraztu2(SDL_Rect camera);

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int TILE_SIZE = 64;
const int GRID_SIZE = 16;
const int LEVEL_SIZE = 1024; //GRID_SIZE * TILE_SIZE

const int camino_ID[33] = { 50,54,55,56,57,58,59,60,61,62,63,66,70,82,83,84,85,86,102,118,134,150,166,182,198,214,215,216,217,218,219,220,221 };

const int plantable_ID[49] = { 88,89,90,91,92,93,94,104,105,106,107,108,109,110,120,121,122,123,124,125,126,136,137,138,139,140,141,142,152,153,154,155,156,157,158,168,169,170,171,172,173,174,184,185,186,187,188,189,190 };

clock_t start, end = 0;
double deltaTime = 0;

struct Player player;
SDL_Rect camera;

SDL_Window* win = NULL;
SDL_Renderer* renderer = NULL;
//player + GUI surface
SDL_Surface* playerSurface = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Surface* bgSurface = NULL;
SDL_Surface* tilesS = NULL;
SDL_Surface* signSurface = NULL;
SDL_Surface* fenceSurface = NULL;
SDL_Surface* cowSurface = NULL;
SDL_Surface* pigSurface = NULL;
SDL_Surface* sartuSurface = NULL;
SDL_Surface* aradoSurface = NULL;


int main(int argc, char* argv[]){
	

	int zabalik = init();

	if (zabalik) {
		renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

		player = createPlayer();
		camera = createCamera();

		for (int i = 0; i < 256; i++) {
			tiles[i].ID = i;
			struct posCoord tilePos = getTilePosFromId(i);
			tiles[i].x = tilePos.x;
			tiles[i].y = tilePos.y;
			tiles[i].farmable = 0;
		}
		loadFiles();
	}
	else {
		printf("Ezin izan da hasieratu.");
	}
	while (zabalik) {
		SDL_Event e;
		while (SDL_PollEvent(&e) > 0) {
			zabalik = inputHandler(e);
		}
		if (player.home == 0) {
			update(deltaTime);
			marraztu();
			marraztu2(camera);
			SDL_UpdateWindowSurface(win);
			getDeltaTime();
		}
		if (player.home == 1) {
		}
	}
	close();
	return 0;
}

int init() {
	int success = 1;
	int imgFlags = IMG_INIT_PNG;

	//Hasieratu SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
		printf("SDL ez da hasieratu. SDL_Error: %s\n", SDL_GetError());
		success = 0;
	}
	else {
		//Zabaldu leihoa
		win = SDL_CreateWindow("Ancient Garden", 500, 60, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
		if (win == NULL) {
			printf("Ez da leihoa sortu. SDL_Error: %s\n", SDL_GetError());
			success = 0;
		}
		else {
			screenSurface = SDL_GetWindowSurface(win);
		}
	}

	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image ez da hasieratu. SDL_image Error: %s\n", IMG_GetError());
		success = 0;
	}
	return success;
}

void close() {
	SDL_FreeSurface(playerSurface);
	playerSurface = NULL;
	SDL_FreeSurface(bgSurface);
	bgSurface = NULL;

	SDL_DestroyWindow(win);
	win = NULL;

	SDL_Quit();
} 

void update(double deltaTime) {
	movePlayer(deltaTime);
	camera = centerCameraInPlayer(camera);
	player.facingTile = getFacingTileId();
}

void marraztu() {
	aplikatuSurface(0, 0, bgSurface, screenSurface, &camera);
	if (player.y > 64 - 5) {
		aplikatuSurface(0, 0, signSurface, screenSurface, &camera);
		drawPlayer(camera, playerSurface, screenSurface);
	}
	else {
		drawPlayer(camera, playerSurface, screenSurface);
		aplikatuSurface(0, 0, signSurface, screenSurface, &camera);
	}
	
	aplikatuSurface(0, 0, fenceSurface, screenSurface, &camera);
	aplikatuSurface(TILE_SIZE, 9*TILE_SIZE, cowSurface, bgSurface, NULL);
	aplikatuSurface(TILE_SIZE*2, 13*TILE_SIZE, pigSurface, bgSurface, NULL);
	return;
}

void loadFiles() {
	bgSurface = loadMedia("assets/images/bg.png");
	signSurface = loadMedia("assets/images/sign.png");
	fenceSurface = loadMedia("assets/images/fence.png");
	playerSurface = loadMedia("assets/images/player.png");
	cowSurface = loadMedia("assets/images/cow.png");
	pigSurface = loadMedia("assets/images/pig.png");
	sartuSurface = loadMedia("assets/images/sartu.png");
	aradoSurface = loadMedia("assets/images/ARADO.png");
}

void getDeltaTime() {
	start = end;
	end = clock();
	deltaTime = ((double)(end - start)) / CLOCKS_PER_SEC;
}
//Temp
void marraztu2(SDL_Rect camera) {
	SDL_Rect rect;
	rect.w = TILE_SIZE;
	rect.h = TILE_SIZE;
	tilesS = SDL_CreateRGBSurface(0, LEVEL_SIZE, LEVEL_SIZE, 32, 0, 0, 0, 0);

	Uint32 color = SDL_MapRGB(tilesS->format, 0, 255, 255);
	//DrawTiles
	for (int i = 0; i < 49; i++) {
		rect.x = tiles[plantable_ID[i]].x * TILE_SIZE;
		rect.y = tiles[plantable_ID[i]].y * TILE_SIZE;
		if (tiles[plantable_ID[i]].plant.seed==SEED_ARADO)
		{
			aplikatuSurface(rect.x-camera.x, rect.y-camera.y, aradoSurface, screenSurface, NULL);
		}
		else
		{
			color = SDL_MapRGB(tilesS->format, 0, 255, 255);
		}
		//SDL_FillRect(tilesS, &rect, color);
	}

	rect.x = tiles[player.tile].x * TILE_SIZE;
	rect.y = tiles[player.tile].y * TILE_SIZE;
	color = SDL_MapRGB(tilesS->format, 255, 0, 0);
	SDL_FillRect(tilesS, &rect, color);
	rect.x = tiles[player.facingTile].x * TILE_SIZE;
	rect.y = tiles[player.facingTile].y * TILE_SIZE;
	color = SDL_MapRGB(tilesS->format, 0, 255, 0);
	SDL_FillRect(tilesS, &rect, color);
	SDL_SetSurfaceBlendMode(tilesS, SDL_BLENDMODE_ADD);
	aplikatuSurface(0, 0, tilesS, screenSurface, &camera);
	SDL_FreeSurface(tilesS);
}
//void marraztu(SDL_Renderer* renderer) {
//	SDL_Rect rect;
//	//DrawTiles
//	for (int i = 0; i < 256; i++) {
//		rect.x = tiles[i].x * TILE_SIZE;
//		rect.y = tiles[i].y * TILE_SIZE;
//		rect.w = TILE_SIZE;
//		rect.h = TILE_SIZE;
//
//		if (!tiles[i].farmable) {
//			SDL_SetRenderDrawColor(renderer, i, 255-i, i-255, 255);
//		}
//		else {
//			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
//		}
//		SDL_RenderFillRect(renderer, &rect);
//	}
//
//	drawPlayer(renderer, player, camera);
//
//	/*SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//	for (int x = 0; x < GRID_SIZE * TILE_SIZE; x += TILE_SIZE) {
//		SDL_RenderDrawLine(renderer, x, 0, x, GRID_SIZE * TILE_SIZE);
//	}
//	for (int y = 0; y < GRID_SIZE * TILE_SIZE; y += TILE_SIZE) {
//		SDL_RenderDrawLine(renderer, 0, y, GRID_SIZE * TILE_SIZE, y);
//	}*/
//
//	SDL_RenderPresent(renderer);
//	return;
//}
