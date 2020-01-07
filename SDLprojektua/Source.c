#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
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
void initGame();

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int TILE_SIZE = 64;
const int GRID_SIZE = 16;
const int LEVEL_SIZE = 1024; //GRID_SIZE * TILE_SIZE

const int camino_ID[33] = { 50,54,55,56,57,58,59,60,61,62,63,66,70,82,83,84,85,86,102,118,134,150,166,182,198,214,215,216,217,218,219,220,221 };
const int plantable_ID[49] = { 88,89,90,91,92,93,94,104,105,106,107,108,109,110,120,121,122,123,124,125,126,136,137,138,139,140,141,142,152,153,154,155,156,157,158,168,169,170,171,172,173,174,184,185,186,187,188,189,190 };


clock_t start, end = 0;
double deltaTime = 0;

struct posCoord mousePos = { 0, 0 };
int main_menu = 1, language = EUS;

struct Player player;
SDL_Rect camera;
struct Inventory inventories[3] = { {
	1, 35, 3, 1, 9, 64, 0, 0, 3
	},
	{
	0, 35, 90, 2, 9, 64, 0, 70, 20
	},
	{
	0, 35, 20, 3, 9, 64, 0, 221, 20
	}
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
SDL_Surface* textua = NULL;
SDL_Surface* HUDSurface = NULL;

int main(int argc, char* argv[]){
	int zabalik = init();

	if (zabalik) {	
		loadFiles();
		renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
		/*for (int i = 0; i < 49; i++) {
			tiles[plantable_ID[i]].plant.arado = 1;
		}*/
	}
	else {
		printf("Ezin izan da hasieratu.");
	}
	while (zabalik) {
		getDeltaTime();
		SDL_Event e;
		SDL_GetMouseState(&mousePos.x, &mousePos.y);
		if (main_menu) {
			while (SDL_PollEvent(&e) > 0 && e.type) {
				zabalik = inputMainMenu(e);
			}
			SDL_Rect clip = { 582, 64, 64, 48 };
			for (int i = 0; i < 3; i++) {
				clip.y = 64 + 48 * i;
				aplikatuSurface(64 + 128 * i, 120, HUDSurface, screenSurface, &clip);
				SDL_UpdateWindowSurface(win);
			}
		}else {
			while (SDL_PollEvent(&e) > 0 && e.type) {
				zabalik = inputHandler(e);
			}
			switch (player.status) {
			case PLAYING:
				update(deltaTime);
				marraztu();
				SDL_UpdateWindowSurface(win);
				break;
			case HOME:
				update(deltaTime);
				aplikatuSurface(0, 0, homeSurface, screenSurface, NULL);
				drawPlayer(camera, playerSurface, screenSurface, HUDSurface);
				for (int i = 0; i < 3; i++) showInv(i, itemsSurface, screenSurface, textua, HUDSurface);
				marraztuInvTag(getHoveringInv(), textua, screenSurface);
				if (hoveringItem.ID != 0)marraztuHoveringItem(itemsSurface, textua, screenSurface);
				SDL_UpdateWindowSurface(win);
				break;
			case PAUSE:
			case PAUSE_HOME:
				aplikatuSurface(0, 0, pauseSurface, screenSurface, NULL);
				SDL_UpdateWindowSurface(win);
				break;
			case COLLOCATING:
				player.timer += deltaTime;
				reset();
				break;
			}
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

	if (TTF_Init() < 0) {
		printf("SDL_ttf ez da hasieratu. SDL_ttf Error: %s\n", TTF_GetError());
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
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
} 

void initGame() {
	for (int i = 0; i < 256; i++) {
		tiles[i].ID = i;
		struct posCoord tilePos = getTilePosFromId(i);
		tiles[i].x = tilePos.x;
		tiles[i].y = tilePos.y;
		tiles[i].plant.arado = 0;
	}

	player = createPlayer();
	camera = createCamera();
	startDic(language);
	startPresests();
	updateInv(INV_PLAYER);
	updateInv(INV_CHEST);
	return;
}

void update(double deltaTime) {
	updateTiles(deltaTime);
	movePlayer(deltaTime);
	checkPosibleInteraction();
	
	ordenatuDroppedItems();
	pickUpItems();
	if(player.status == PLAYING) camera = centerCameraInPlayer(camera);
	player.facingTile = getFacingTileId();
	checkHover();
	animatePlayer(deltaTime);
}

void marraztu() {
	aplikatuSurface(0, 0, bgSurface, screenSurface, &camera);
	marraztuTiles(spriteSheetTest, screenSurface);
	marraztuDroppedItems(0);
	if (player.y > 64 - 5) {
		aplikatuSurface(0, 0, signSurface, screenSurface, &camera);
		drawPlayer(camera, playerSurface, screenSurface, HUDSurface);
	}
	else {
		drawPlayer(camera, playerSurface, screenSurface, HUDSurface);
		aplikatuSurface(0, 0, signSurface, screenSurface, &camera);
	}
	marraztuDroppedItems(1);
	aplikatuSurface(0, 0, fenceSurface, screenSurface, &camera);
	aplikatuSurface(TILE_SIZE, 9 * TILE_SIZE, cowSurface, bgSurface, NULL);
	aplikatuSurface(TILE_SIZE * 2, 13 * TILE_SIZE, pigSurface, bgSurface, NULL);
	for (int i = 0; i < 2; i++) showInv(i, itemsSurface, screenSurface, textua, HUDSurface);
	marraztuInvTag(getHoveringInv(), textua, screenSurface);
	if(hoveringItem.ID != 0)marraztuHoveringItem(itemsSurface, textua, screenSurface);
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
	HUDSurface = loadMedia("assets/images/HUD.png");
}

void getDeltaTime() {
	start = end;
	end = clock();
	deltaTime = (double)(end - start) / CLOCKS_PER_SEC;
}

void reset() {
	if (player.timer > 0.5) {
		player.x = 2 * TILE_SIZE;
		player.y = TILE_SIZE;
		player.movingDown = 0;
		player.movingRight = 0;
		player.movingLeft = 0;
		marraztu();
		player.timer = 0;
		player.status = PLAYING;
		inventories[INV_HOTBAR].yPos = 0;
		inventories[INV_PLAYER].yPos = inventories[INV_PLAYER].slotSize + inventories[INV_PLAYER].headerSize + 3;
	}
}