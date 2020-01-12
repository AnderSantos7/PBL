#define _CRT_SECURE_NO_WARNINGS
#include "SDL.h"
#include "SDL_image.h"
#include "funtzioak.h"
#include "objektuak.h"

struct Obstacle obstaclesOutside[] = {
	{ 896, 864, 100, 34 } , //Pozo
	{0, 0, 256, 192}, //Casa
	{836, 132, 60, 62} //Cartel
};

struct Obstacle obstaclesInside[] = {
	{0, 0, 100, 160}, //Cama
	{340, 0, 100, 78}, //Cofre tmp
};