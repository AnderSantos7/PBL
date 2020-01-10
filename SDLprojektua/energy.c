#include "SDL.h"
#include "funtzioak.h"
#include "SDL_image.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>
void marraztuEnergy()
{
	if (player.energy < 0)
	{
		player.energy = 0;
	}
	SDL_Rect rect = {620 , 150, 20, player.energy};
	SDL_RenderFillRect(renderer, &rect);
	return;
}

void restaEnergy()
{
	player.energy = player.energy - 5;
	return;
}

void resetEnergy()
{
	player.energy = 200;
}