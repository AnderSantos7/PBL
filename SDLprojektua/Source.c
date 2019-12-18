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

	int mouseX = 0;
	int mouseY = 0;
	while (zabalik) {
		SDL_Event e;
		while (SDL_PollEvent(&e) > 0) {
			zabalik = inputHandler(e);
		}
		update(deltaTime);
		marraztu();
		marraztu2(camera);
		SDL_UpdateWindowSurface(win);
		getDeltaTime();
	}
	close();
	return 0;
}

struct posCoord getTilePosFromId(int ID) {
	struct posCoord pos;
	pos.x = ID % GRID_SIZE;
	pos.y = ID / GRID_SIZE;
	return pos;
}

int getTileFromPos(int x, int y) {
	int ID = 0;
	ID = (y / 64) * 16 + (x / 64);
	return ID;
}

int getFacingTileId() {
	int ID = player.tile;
	switch (player.facingDirection) {
	case DIR_LEFT:
		ID = player.tile - 1;
		break;
	case DIR_RIGHT:
		ID = player.tile + 1;
		break;
	case DIR_UP:
		ID = player.tile - 16;
		break;
	case DIR_DOWN:
		ID = player.tile + 16;
		break;
	}
	return ID;
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
	drawPlayer(camera, playerSurface, screenSurface);
	return;
}

void loadFiles() {
	bgSurface = loadMedia("assets/images/bg.png");
	playerSurface = loadMedia("assets/images/player.png");
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
	for (int i = 0; i < 33; i++) {
		rect.x = tiles[camino_ID[i]].x * TILE_SIZE;
		rect.y = tiles[camino_ID[i]].y * TILE_SIZE;
		SDL_FillRect(tilesS, &rect, color);
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