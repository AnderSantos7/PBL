#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct posCoord getTilePosFromId(int ID);
void marraztu();
int init();
void close();
void update();
void getDeltaTime();


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
	1, 35, 413, 1, 9, 64, 0, 0, 3
	},
	{
	0, 35, 279, 2, 9, 64, 0, 70, 20
	},
	{
	0, 35, 241,3, 9, 64, 0, 221, 20
	},
	{
	0, 35, 241, 3, 9, 64, 0, 221, 20
	}
};

SDL_Window* win = NULL;
SDL_Renderer* renderer = NULL;

int main(int argc, char* argv[]) {
	int zabalik = init();

	if (zabalik) {
		loadFiles();
		
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
			menu(deltaTime);
			SDL_RenderPresent(renderer);
			if (!main_menu)
			{
				Mix_Music* music = Mix_LoadMUS("assets/sounds/china.wav");
				Mix_PlayMusic(music, 1);
			}
		}
		
		else {
			while (SDL_PollEvent(&e) > 0 && e.type) {
				zabalik = inputHandler(e);
			}
			SDL_RenderPresent;
			switch (player.status) {
			case PLAYING:
				update(deltaTime);
				marraztu();
				
				SDL_RenderPresent(renderer);
				break;
			case HOME:
				update(deltaTime);
				aplikatuSurface(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, textures[homeSurface], NULL);
				SDL_Rect clip = { 170 , 0 , 55 , 90 };
				clip.y = 0;
				clip.w = 55;
				clip.h = 90;
				aplikatuSurface(390, 70, 55, 90, textures[obstacleSurface], &clip);
				if (player.sleeping)
				{
					SDL_Rect klap = { 0, 0, 64, 64 };
					aplikatuSurface(20, 71, 64, 64, textures[playerSurface], &klap);
				}
				checkPosibleInteraction();

				if(player.sleeping==0)drawPlayer();
				else paintSleep();
				drawClock();
				for (int i = 0; i < 3; i++) showInv(i);
				marraztuInvTag(getHoveringInv());
				if (hoveringItem.ID != 0)marraztuHoveringItem();
				SDL_RenderPresent(renderer);	
				break;
			case PAUSE:
			case PAUSE_HOME:
				aplikatuSurface(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, textures[pauseSurface], NULL);
				SDL_RenderPresent(renderer);
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
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
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
			renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC);
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
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
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		printf("SDL_mixer ez da hasieratu. SDL_mixer Error: %s\n", Mix_GetError());
		success = 0;
	}
	srand((int)time(0));
	return success;
}

void close() {
	for (int i = 0; i < 16; i++) SDL_DestroyTexture(textures[i]);

	SDL_DestroyWindow(win);
	win = NULL;
	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
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
	getNextQuest();
	//load();
	return;
}

void update() {
	updateDay(deltaTime);
	updateTiles(deltaTime);
	movePlayer(deltaTime);
	checkEnergy();
	if(player.sleeping)chronoSleep(deltaTime);
	checkPosibleInteraction();
	ordenatuDroppedItems();
	if (pickUpItems()) playPickUpSFX();
	if (player.status == PLAYING) camera = centerCameraInPlayer(camera);
	player.facingTile = getFacingTileId();
	checkHover();
	animatePlayer(deltaTime);
	return;
}

void marraztu() {
	aplikatuSurface(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, textures[bgSurface], &camera);
	marraztuTiles();
	marraztuDroppedItems(0);
	SDL_Rect clip = { 0, 126, 60, 61 };
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
	for (int i = 0; i < 2; i++) showInv(i);
	marraztuInvTag(getHoveringInv());
	if (hoveringItem.ID != 0) marraztuHoveringItem();
	if(!getQuestMenuState())showCurrentQuest();
	showQuestMenu();
	questCompleteAnim(deltaTime);
	return;
}

void getDeltaTime() {
	start = end;
	end = clock();
	deltaTime = (double)(end - start) / CLOCKS_PER_SEC;
	return;
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
		inventories[INV_HOTBAR].yPos = 413;
		inventories[INV_PLAYER].yPos = 407 - (inventories[INV_PLAYER].slotSize  * inventories[INV_PLAYER].rows);
	}
	return;
}