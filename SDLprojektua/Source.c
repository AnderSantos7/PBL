#include "SDL.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>

struct posCoord getTilePosFromId(int ID);
void marraztu(SDL_Renderer* renderer);
void init(SDL_Renderer* renderer);
void update();

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int TILE_SIZE = 64;
const int GRID_SIZE = 16;

struct Player player;
SDL_Rect camera;

int main(int argc, char* argv[]){
	const int LEVEL_SIZE = GRID_SIZE * TILE_SIZE;
	SDL_Window* win = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Init(SDL_INIT_EVERYTHING);
	win = SDL_CreateWindow("Kuadrikula Magikoa", 500, 60, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
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

	init(renderer);

	int zabalik = 1;
	int mouseX = 0;
	int mouseY = 0;
	while (zabalik) {
		SDL_Event e;
		while (SDL_PollEvent(&e) > 0) {
			zabalik = inputHandler(e);
		}
		update();
		marraztu(renderer);
		SDL_UpdateWindowSurface(win);
	}
	return 0;
}

struct posCoord getTilePosFromId(int ID) {
	struct posCoord pos;
	pos.x = ID % GRID_SIZE;
	pos.y = ID / GRID_SIZE;
	return pos;
}

void init(SDL_Renderer* renderer) {
	centerCameraInPlayer(camera, player);
	marraztu(renderer);
}

void update() {
	centerCameraInPlayer(camera, player);
	player = movePlayer(player);
}

void marraztu(SDL_Renderer* renderer) {
	SDL_Rect rect;

	for (int i = 0; i < 256; i++) {
		rect.x = tiles[i].x * TILE_SIZE;
		rect.y = tiles[i].y * TILE_SIZE;
		rect.w = TILE_SIZE;
		rect.h = TILE_SIZE;

		if (!tiles[i].farmable) {
			SDL_SetRenderDrawColor(renderer, i, 255-i, i-255, 255);
		}
		else {
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		}
		SDL_RenderFillRect(renderer, &rect);
	}

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
	SDL_Rect jokalari;
	jokalari.x = player.x;
	jokalari.y = player.y;
	jokalari.w = player.w;
	jokalari.h = player.h;
	SDL_RenderFillRect(renderer, &jokalari);

	/*SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	for (int x = 0; x < GRID_SIZE * TILE_SIZE; x += TILE_SIZE) {
		SDL_RenderDrawLine(renderer, x, 0, x, GRID_SIZE * TILE_SIZE);
	}
	for (int y = 0; y < GRID_SIZE * TILE_SIZE; y += TILE_SIZE) {
		SDL_RenderDrawLine(renderer, 0, y, GRID_SIZE * TILE_SIZE, y);
	}*/

	SDL_RenderPresent(renderer);
	return;
}
