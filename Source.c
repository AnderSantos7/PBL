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
struct Inventory inventory = {
	0, 100, 100, 3, 9, 64
};

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
SDL_Surface* pauseSurface = NULL;
SDL_Surface* homeSurface = NULL;
SDL_Surface* itemsSurface = NULL;
SDL_Surface* spriteSheetTest = NULL;

int main(int argc, char* argv[]){
	int zabalik = init();

	if (zabalik) {
		renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

		player = createPlayer();
		camera = createCamera();
		inventory = updateInv(inventory);

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
		if (player.status == PLAYING) {
			update(deltaTime);
			marraztu();
			SDL_UpdateWindowSurface(win);
			getDeltaTime();
		}
		else if (player.status == HOME) {
			update(deltaTime);
			aplikatuSurface(0, 0, homeSurface, screenSurface, NULL);
			drawPlayer(camera, playerSurface, screenSurface);
			SDL_UpdateWindowSurface(win);
		}
		else if (player.status == PAUSE || player.status == PAUSE_HOME) {
			aplikatuSurface(0, 0, pauseSurface, screenSurface, NULL);
			SDL_UpdateWindowSurface(win);
			inputHandler(e);
		}
		else if (player.status == COLLOCATING) {
			player.timer += deltaTime;
			reset();
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
	updateTiles(deltaTime);
	movePlayer(deltaTime);
	if(player.status == PLAYING)camera = centerCameraInPlayer(camera);
	player.facingTile = getFacingTileId();
}

void marraztu() {
	aplikatuSurface(0, 0, bgSurface, screenSurface, &camera);
	marraztuTiles(spriteSheetTest, screenSurface);
	if (player.y > 64 - 5) {
		aplikatuSurface(0, 0, signSurface, screenSurface, &camera);
		drawPlayer(camera, playerSurface, screenSurface);
	}
	else {
		drawPlayer(camera, playerSurface, screenSurface);
		aplikatuSurface(0, 0, signSurface, screenSurface, &camera);
	}

	aplikatuSurface(0, 0, fenceSurface, screenSurface, &camera);
	aplikatuSurface(TILE_SIZE, 9 * TILE_SIZE, cowSurface, bgSurface, NULL);
	aplikatuSurface(TILE_SIZE * 2, 13 * TILE_SIZE, pigSurface, bgSurface, NULL);
	marraztuInv(inventory, itemsSurface, screenSurface);
	
	return;
}

void loadFiles() {
	bgSurface = loadMedia("assets/images/bg.png");
	signSurface = loadMedia("assets/images/sign.png");
	fenceSurface = loadMedia("assets/images/fence.png");
	playerSurface = loadMedia("assets/images/player.png");
	cowSurface = loadMedia("assets/images/cow.png");
	pigSurface = loadMedia("assets/images/pig.png");
	pauseSurface = loadMedia("assets/images/pause.png");
	homeSurface = loadMedia("assets/images/home.png");
	itemsSurface = loadMedia("assets/images/items.png");
	spriteSheetTest = loadMedia("assets/images/sprite.png");
}

void getDeltaTime() {
	start = end;
	end = clock();
	deltaTime = ((double)(end - start)) / CLOCKS_PER_SEC;
}

void reset() {
	player.x = 2 * TILE_SIZE;
	player.y = TILE_SIZE;
	player.movingDown = 0;
	player.movingRight = 0;
	player.movingLeft = 0;
	if (player.timer > 1000) {
		marraztu();
		player.timer = 0;
		player.status = PLAYING;
	}
}
