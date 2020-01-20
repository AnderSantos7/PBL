#define _CRT_SECURE_NO_WARNINGS
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

void idatzimenu();
void readmenu(int botoia, char* str);

double tiempof = 0;
instructions = 0, anterior = 0;
//Hasierako menua erakutzi eta honekin interaktuatzeko funtzioa.
void menu(double deltaTime)
{
	//Aplikazioaren hasieran 'MORHACKER' logoa erakutsi
	if (tiempof < 2) {
		tiempof += deltaTime;
		SDL_Rect firma = { 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };
		aplikatuSurface(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, textures[firmaSurface], &firma);

	}else if (instructions > anterior){
			drawInstructions();
			anterior++;
	}else if (instructions > 5 || instructions == 0) {
			anterior = 0;
			SDL_Rect background = { 0,0,SCREEN_WIDTH - 2, SCREEN_HEIGHT };
			aplikatuSurface(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, textures[menuSurface], &background);

			SDL_Rect button = { 638,0,160,61 };
			aplikatuSurface(239, 200, 160, 61, textures[menuSurface], &button);

			for (int i = 0; i < 2; i++)
			{
				SDL_Rect button2 = { 638,62,120,52 };
				aplikatuSurface(259, 271 + 60 * i, 120, 52, textures[menuSurface], &button2);
			}

			SDL_Rect clip = { 638, 114, 64, 48 };
			for (int i = 0; i < 3; i++) {
				clip.y = 114 + 48 * i;
				aplikatuSurface(150 + 128 * i, 432, 64, 48, textures[menuSurface], &clip);
			}
			idatzimenu();

	}

	return;
}

//Pantailako karratu zehatz batean arratoia dagoen ala ez itzultzen duen funtzioa.
int checkIfClicking(SDL_Rect* button) {
	int success = 0;
	if (mousePos.x > button->x&&
		mousePos.x < button->x + button->w &&
		mousePos.y > button->y&&
		mousePos.y < button-> y + button->h) {
		success = 1;
	}
	return success;
}
SDL_Texture* inst = NULL;
int longitud = 0;
int altura = 0;
int currentLang = -1;
void drawInstructions() {
	SDL_Rect clip = { 0 + 544 * longitud, 0 + 308 * altura, 544, 308 };
	if (currentLang != language){
		currentLang = language;
		SDL_FreeSurface(inst);
		char* src[3] = { "assets/images/INSTRUKZIOAK.png", "assets/images/INSTRUCCIONES.png", "assets/images/INSTRUCTIONS.png" };
		inst = loadMedia(src[currentLang]);
	}
	aplikatuSurface(50, 100, 544, 308, inst, &clip);
	longitud++;

	if (longitud > 2 && altura == 0) {
		longitud = 0;
		altura = 1;
	}
	if (altura >= 1 && longitud > 2) {
		instructions = 0;
		longitud = 0;
		altura = 0;
	}
	return;
}

void idatzimenu() {
	char str[128];
	TTF_Font* font;
	SDL_Color color = { 255, 255, 255 };
	SDL_Surface* s;
	SDL_Texture* t;
	int w, h, i;

	//Menuko hiru botoietan idatzi
	for (i = 1; i < 4; i++) {
		readmenu(i, str);
		if (i == 1) { //Jolastea
			font = TTF_OpenFont("assets/fonts/y.n.w.u.a.y.ttf", 20);
		}
		else { //Partida kargatzea eta kontrolak ikustea
			font = TTF_OpenFont("assets/fonts/y.n.w.u.a.y.ttf", 13);
		}
		s = TTF_RenderText_Solid(font, str, color);
		t = SDL_CreateTextureFromSurface(renderer, s);

		SDL_QueryTexture(t, NULL, NULL, &w, &h);

		if (i == 1) aplikatuSurface(SCREEN_WIDTH / 2 - w / 2, 230 - h / 2, w, h, t, NULL);
		else aplikatuSurface(SCREEN_WIDTH / 2 - w / 2, 295 + 60 * (i - 2) - h / 2, w, h, t, NULL);
		SDL_FreeSurface(s);
		SDL_DestroyTexture(t);
		TTF_CloseFont(font);
	}
	return;
}

//Textu dokumentu batetik irakurtzea botoietan jarri behar dena
void readmenu(int botoia, char* str) {
	FILE* fp;
	char buff[64] = { 0 };
	char* src = "assets/data/EUS_menus.txt";
	if (language == ENG) src = "assets/data/ENG_menus.txt";
	else if (language == ESP) src = "assets/data/ESP_menus.txt";;
	fp = fopen(src, "r");
	for (int j = 0; j < botoia; j++) {
		fgets(buff, 64, fp);
	}
	int i = 0;
	while (buff[i] != '\n' && buff[i] != '\0') {
		str[i] = buff[i];
		i++;
	}
	
	str[i] = '\0';
	fclose(fp);
	return;
}