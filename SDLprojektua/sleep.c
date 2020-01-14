#define _CRT_SECURE_NO_WARNINGS
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>
#include <string.h>

void sleep()
{
	player.sleeping = 1;
	player.status = HOME;
	player.x = 130;
	player.y = 100;
	camera.x = 0;
	camera.y = 0;
	inventories[INV_HOTBAR].yPos = 3;
	inventories[INV_PLAYER].yPos = 90;
	return;
}

double chronometro = 0;
void chronoEnergy(double deltaTime)
{
	chronometro += deltaTime;
	if (chronometro > 10 && player.sleeping)
	{
		player.sleeping = 0;
		resetEnergy();
		resetDay();
		chronometro = 0;
	}
}

void paintSleep()
{
	int alpha = chronometro / 10 * 255;
	printf("%d ", alpha);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha);
	SDL_RenderFillRect(renderer, NULL);
}