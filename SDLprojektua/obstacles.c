#define _CRT_SECURE_NO_WARNINGS
#include "SDL.h"
#include "SDL_image.h"
#include "funtzioak.h"
#include "objektuak.h"

struct Obstacle obstaclesOutside[] = {
	{1, 896, 864, 100, 34 }, //Pozo
	{1, 836, 132, 60, 62}, //Cartel
	{0, 0, 0, 256, 192}, //Casa
	{1, 0, 365, 300, 80}, //vaca
	{1, 0, 765, 260, 80}, //cerdo
	
};

struct Obstacle obstaclesInside[] = {
	{1, 0, 0, 100, 160}, //Cama
	{1, 340, 0, 100, 78}, //Cofre tmp
};
