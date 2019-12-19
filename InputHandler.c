#include "SDL.h"
#include "SDL_image.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>
void keyHandlerDown(SDL_Event e);
void keyHandlerUp(SDL_Event e);

int inputHandler(SDL_Event e) {
	int mouseX = 0, mouseY = 0, zabalik = 1;
	switch (e.type) {
	case SDL_QUIT:
		zabalik = 0;
		break;
	case SDL_MOUSEBUTTONDOWN:
		SDL_GetMouseState(&mouseX, &mouseY);
		mouseX = mouseX / 64;
		mouseX = mouseX % 16;
		mouseY = mouseY / 64;
		landatu();
		break;
	case SDL_KEYDOWN:
		keyHandlerDown(e);
		break;
	case SDL_KEYUP:
		keyHandlerUp(e);
		break;
	}
	return zabalik;
}

void keyHandlerDown(SDL_Event e) {
	switch (e.key.keysym.scancode) {

	case SDL_SCANCODE_A:
		player.facingDirection = DIR_LEFT;
		player.movingLeft = 1;
		break;
	case SDL_SCANCODE_D:
		player.facingDirection = DIR_RIGHT;
		player.movingRight = 1;
		break;
	case SDL_SCANCODE_W:
		player.facingDirection = DIR_UP;
		player.movingUp = 1;
		break;
	case SDL_SCANCODE_S:
		player.facingDirection = DIR_DOWN;
		player.movingDown = 1;
		break;
	}
	return;
}

void keyHandlerUp(SDL_Event e) {
	switch (e.key.keysym.scancode) {
	case SDL_SCANCODE_W:
		player.movingUp = 0;
		break;
	case SDL_SCANCODE_A:
		player.movingLeft = 0;
		break;
	case SDL_SCANCODE_S:
		player.movingDown = 0;
		break;
	case SDL_SCANCODE_D:
		player.movingRight = 0;
		break;
	}
	return;
}