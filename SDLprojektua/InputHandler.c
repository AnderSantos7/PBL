#include "SDL.h"
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
		tiles[mouseX + mouseY * 16].farmable = 1;
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
int movingRight = 0;
int movingLeft = 0;
int movingUp = 0;
int movingDown = 0;

void keyHandlerDown(SDL_Event e) {
	switch (e.key.keysym.scancode) {
	case SDL_SCANCODE_W:
		movingRight = 1;
		break;
	case SDL_SCANCODE_A:
		movingLeft = 1;
		break;
	case SDL_SCANCODE_S:
		movingUp = 1;
		break;
	case SDL_SCANCODE_D:
		movingDown = 1;
		break;
	}
	return;
}

void keyHandlerUp(SDL_Event e) {
	switch (e.key.keysym.scancode) {
	case SDL_SCANCODE_W:
		movingRight = 0;
		break;
	case SDL_SCANCODE_A:
		movingLeft = 0;
		break;
	case SDL_SCANCODE_S:
		movingUp = 0;
		break;
	case SDL_SCANCODE_D:
		movingDown = 0;
		break;
	}
	return;
}

int isMovingRight() {
	return movingRight;
}

int isMovingLeft() {
	return movingLeft;
}

int isMovingUp() {
	return movingUp;
}

int isMovingDown() {
	return movingDown;
}