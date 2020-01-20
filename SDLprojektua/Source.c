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
int init();
void close();
void update();
void getDeltaTime();


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int TILE_SIZE = 64;
const int GRID_SIZE = 16;
const int LEVEL_SIZE = 1024; //GRID_SIZE * TILE_SIZE

//Camino_ID = Mapako bide 'tile'-ak. Abiadura handiagoa ibiltzerakoan
const int camino_ID[33] = { 50,54,55,56,57,58,59,60,61,62,63,66,70,82,83,84,85,86,102,118,134,150,166,182,198,214,215,216,217,218,219,220,221 };
//plantable_ID = Landatu daiteken lursaila
const int plantable_ID[49] = { 88,89,90,91,92,93,94,104,105,106,107,108,109,110,120,121,122,123,124,125,126,136,137,138,139,140,141,142,152,153,154,155,156,157,158,168,169,170,171,172,173,174,184,185,186,187,188,189,190 };


clock_t start, end = 0;
double deltaTime = 0;

struct posCoord mousePos = { 0, 0 };
int main_menu = 1, language = EUS;

struct Player player;
SDL_Rect camera;

//Inbentarioen informazioa. {Zabalik, xPos, yPos, lerroak, zutabeak, hutsune tamaina, spritesheeteko x eta y posizioak eta goiko bordearen tamaina
struct Inventory inventories[4] = { {
	1, 35, 413, 1, 9, 64, 0, 0, 3
	},
	{
	0, 35, 279, 2, 9, 64, 0, 70, 20
	},
	{
	0, 35, 241,3, 9, 64, 0, 221, 20
	},
	{
	0, 35, 20, 1, 9, 64, 0, 221, 20
	}
};

SDL_Window* win = NULL;
SDL_Renderer* renderer = NULL;
double kargatu = 0;

int main(int argc, char* argv[]) {
	int zabalik = init();

	if (zabalik) {
		loadFiles();
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
			//Ez denez beharrezkoa menua behin eta berriz marraztea, ahalik eta gutxien marrazten da eta horrela errendimendua hobetzen da 
			//if (kargatu < 2.3) {
				menu(deltaTime);
				kargatu += deltaTime;
			//}
			if (player.status == LOAD) {
				aplikatuSurface(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, textures[loadSurface], NULL);
			}
			SDL_RenderPresent(renderer);
		}
		else {
			while (SDL_PollEvent(&e) > 0 && e.type) {
				zabalik = inputHandler(e);
			}
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
				marraztuDroppedItems(0);
				if (player.sleeping)
				{
					SDL_Rect klap = { 0, 0, 64, 64 };
					aplikatuSurface(20, 71, 64, 64, textures[playerSurface], &klap);
				}
				checkPosibleInteraction();
				if(player.sleeping == 0) drawPlayer();
				else paintSleep();
				marraztuDroppedItems(1);
				for (int i = 0; i < 3; i++) showInv(i);
				marraztuInvTag(getHoveringInv());
				if (hoveringItem.ID != 0) marraztuHoveringItem();
				drawClock();
				SDL_RenderPresent(renderer);	
				reset();
				break;
			case PAUSE:
			case PAUSE_HOME:
				aplikatuSurface(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, textures[pauseSurface], NULL);
				SDL_RenderPresent(renderer);
				break;
			case SAVE:
			case SAVE_HOME:
				aplikatuSurface(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, textures[saveSurface], NULL);
				SDL_RenderPresent(renderer);
				break;
			case LOAD:
			case LOAD_HOME:
				aplikatuSurface(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, textures[loadSurface], NULL);
				SDL_RenderPresent(renderer);
			}
		}
	}
	close();
	return 0;
}

//Liburutegiak abiarazi eta pantaila & render-a sortu
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
	//rand() erabiltzeko ausazko seed berria sortu
	srand((int)time(0));
	return success;
}

//Jokua itxi
void close() {
	for (int i = 0; i < 16; i++) SDL_DestroyTexture(textures[i]);

	SDL_DestroyWindow(win);
	win = NULL;
	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
}

//Jokuaren beharrezko datu eta struct-ak hasieratu
void initGame() {
	for (int i = 0; i < 256; i++) {
		tiles[i].ID = i;
		struct posCoord tilePos = getTilePosFromId(i);
		tiles[i].x = tilePos.x;
		tiles[i].y = tilePos.y;
		tiles[i].plant.arado = 0;
	}

	camera = createCamera();
	startDic(language);
	updateInv(INV_PLAYER);
	updateInv(INV_CHEST);
	getNextQuest();
	return;
}

//Mapa osoa eta datu guztiak aktualizatzea
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
	playMusic();
	return;
}

//Frame batetik bestera pasatzen den denbora kalkulatzea
void getDeltaTime() {
	start = end;
	end = clock();
	deltaTime = ((double)end - start) / CLOCKS_PER_SEC;
	return;
}

//Etxetik ateratzeko animazioa
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