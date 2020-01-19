#define _CRT_SECURE_NO_WARNINGS
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>
#include <string.h>

//Jokalaria lokartzeko funtzioa. Etxera eraman eta pantailaren animazio beltza hasten da.
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
//Lo egiten zenbat denbora egon behar den kontrolatzen duen funtzioa. Bukatutakoan jokalaria esnatu eta energia berrezarriko zaio.
void chronoSleep(double deltaTime)
{
	chronometro += deltaTime;
	if (chronometro > 5 && player.sleeping)
	{
		player.sleeping = 0;
		resetEnergy();
		resetDay();
		chronometro = 0;
	}
	return;
}

//Lo egitean agertzen den filtro beltza marrazten duen funtzioa. Iluntzean bezala, errektangelu bat margotzen da alfa balio aldakorrarekin.
void paintSleep()
{
	int alpha = (int)(chronometro / 5 * 255);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha);
	SDL_RenderFillRect(renderer, NULL);
	return;
}