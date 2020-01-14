#include "SDL.h"
#include "funtzioak.h"
#include "SDL_image.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>
void marraztuEnergy()
{
	if (player.energy <= 0)
	{
		player.energy = 0;
	}
	SDL_Rect bRect = { 610 , 140, 30, 210 };
	SDL_SetRenderDrawColor(renderer, 110, 77, 43, 255);
	SDL_RenderFillRect(renderer, &bRect);
	SDL_Rect rect = {615, 345 - player.energy , 20, player.energy};
	SDL_SetRenderDrawColor(renderer, 75, 216, 235, 255);
	SDL_RenderFillRect(renderer, &rect);
	SDL_Rect eRect = { 615, 145, 20, 200 - player.energy };
	SDL_SetRenderDrawColor(renderer, 64, 52, 40, 255);
	SDL_RenderFillRect(renderer, &eRect);
	return;
}

void restaEnergy()
{
	player.energy -= 20;
	
	return;
}

void resetEnergy()
{
	player.energy = 200;
	return;
}

void checkEnergy()
{
	if (player.energy == 0)
	{
		sleep();
	}
}