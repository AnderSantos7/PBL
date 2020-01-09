#include "SDL.h"
#include "funtzioak.h"
#include "SDL.h"
#include "SDL_image.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>
void marraztuEnergy()
{
	if (player.energia<0)
	{
		player.energia = 0;
	}
	SDL_Surface* rect;
	rect = SDL_CreateRGBSurface(0, 20, player.energia, 32, 0, 0, 0, 0);
	SDL_FillRect(rect, NULL, SDL_MapRGB(rect->format, 255, 255, 255));
	aplikatuSurface(620, 150, rect , surface[screenSurface], NULL);
}

void restaEnergy()
{
	player.energia = player.energia - 5;
}