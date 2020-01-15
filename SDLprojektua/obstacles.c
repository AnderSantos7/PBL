#define _CRT_SECURE_NO_WARNINGS
#include "SDL.h"
#include "SDL_image.h"
#include "funtzioak.h"
#include "objektuak.h"

struct Obstacle obstaclesOutside[] = {
	{1, 896, 864, 100, 34 } , //Pozo
	{0, 0, 0, 256, 192}, //Casa
	{1, 836, 132, 60, 62}, //Cartel
	{1, 500, 132, 120, 0}, //Shop
	{1, 0, 385, 320, 380} //Vaca
};

struct Obstacle obstaclesInside[] = {
	{1, 0, 0, 100, 160}, //Cama
	{1, 340, 0, 100, 78}, //Cofre tmp
};
