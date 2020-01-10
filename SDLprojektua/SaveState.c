#define _CRT_SECURE_NO_WARNINGS
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>
#include <time.h>

FILE* fp;
struct Item droppedItems[128];

void save() {
	int i = 0;
	int j = 0;
	fp = fopen("assets/data/GUARDADO.txt", "w");
	fprintf(fp, "%i %i %i\n", player.x, player.y, player.status); //datos player
	for (i = 0; i < 9; i++) {
		fprintf(fp, "%i %i", inventories[0].items[i].ID, inventories[0].items[i].quantity); //datos hotbar
		if (i != 8) fprintf(fp, " ");
	}
	fprintf(fp, "\n");

	for (i = 0; i < 18; i++) {
		fprintf(fp, "%i %i", inventories[1].items[i].ID, inventories[1].items[i].quantity); //datos inventario
		if (i != 8 && i != 17) fprintf(fp, " ");
		else if (i == 8) fprintf(fp,"\n");
	}
	fprintf(fp, "\n");

	for (i = 0; i < 27; i++) {
		fprintf(fp, "%i %i", inventories[2].items[i].ID, inventories[2].items[i].quantity); //datos cofre
		if (i != 8 && i != 17 && i != 26) fprintf(fp, " ");
		else if (i == 8 || i == 17) fprintf(fp, "\n");
	}
	fprintf(fp, "\n");
	for (i = 0; i < droppedLength; i++) {
		fprintf(fp, "%i %i %i %i", droppedItems[i].ID, droppedItems[i].quantity, droppedItems[i].xPos, droppedItems[i].yPos);
		if (i != droppedLength - 1) fprintf(fp, " ");
	}
	fclose(fp);
} 

void load() {
	int i = 0;
	char buff[256];
	char* src = "assets/data/GUARDADO_DEFAULT.txt";
	if (player.load == 1) {
		src = "assets/data/GUARDADO.txt";
		player.x = 0;
		player.y = 0;
	}
	fp = fopen(src, "r");
	fgets(buff, 256, fp);
	while (buff[i] != ' ') {
		player.x = player.x * 10 + buff[i] - 48; //cargar player
		i++;
	}
	i++;
	while (buff[i] != ' ') {
		player.y = player.y * 10 + buff[i] - 48;
		i++;
	}
	i++;
	while (buff[i] != '\n') {
		player.status = buff[i] - 48;
		if (player.status == PAUSE_HOME) {
			player.status = HOME;
			camera.x = 0;
			camera.y = 0;
		}
		else if (player.status == PAUSE) player.status = PLAYING;
		i++;
	}
	fgets(buff, 256, fp);
	i = 0;
	int j;
	for (j = 0; j < 9; j++) { //cargar hotbar
		inventories[0].items[j].ID = 0;
		while (buff[i] != ' ') {
			inventories[0].items[j].ID = inventories[0].items[j].ID * 10 + buff[i] - 48;
			i++;
		}
		inventories[0].items[j].name = itemPresets[inventories[0].items[j].ID].name;
		inventories[0].items[j].xPos = itemPresets[inventories[0].items[j].ID].xPos;
		inventories[0].items[j].yPos = itemPresets[inventories[0].items[j].ID].yPos;
		inventories[0].items[j].seed = itemPresets[inventories[0].items[j].ID].seed;
		inventories[0].items[j].sheetPosX = itemPresets[inventories[0].items[j].ID].sheetPosX;
		inventories[0].items[j].sheetPosY = itemPresets[inventories[0].items[j].ID].sheetPosY;

		i++;
		inventories[0].items[j].quantity = 0;
		while (buff[i] != ' ' && buff[i] != '\n' && buff[i] != '\0') {
			inventories[0].items[j].quantity = inventories[0].items[j].quantity * 10 + buff[i] - 48;
			i++;
		}
		i++;
	}
	fgets(buff, 256, fp);
	i = 0;
	for (j = 0; j < 18; j++) { //cargar inventario
		inventories[1].items[j].ID = 0;
		while (buff[i] != ' ') {
			inventories[1].items[j].ID = inventories[1].items[j].ID * 10 + buff[i] - 48;
			i++;
		}
		inventories[1].items[j].name = itemPresets[inventories[1].items[j].ID].name;
		inventories[1].items[j].xPos = itemPresets[inventories[1].items[j].ID].xPos;
		inventories[1].items[j].yPos = itemPresets[inventories[1].items[j].ID].yPos;
		inventories[1].items[j].seed = itemPresets[inventories[1].items[j].ID].seed;
		inventories[1].items[j].sheetPosX = itemPresets[inventories[1].items[j].ID].sheetPosX;
		inventories[1].items[j].sheetPosY = itemPresets[inventories[1].items[j].ID].sheetPosY;

		i++;
		inventories[1].items[j].quantity = 0;
		while (buff[i] != ' ' && buff[i] != '\n' && buff[i] != '\0') {
			inventories[1].items[j].quantity = inventories[1].items[j].quantity * 10 + buff[i] - 48;
			i++;
		}
		i++;
		if (j == 8) {
			i = 0;
			fgets(buff, 256, fp);
		}
	}
	fgets(buff, 256, fp);
	i = 0;
	for (j = 0; j < 27; j++) { //cargar cofre
		inventories[2].items[j].ID = 0;
		while (buff[i] != ' ') {
			inventories[2].items[j].ID = inventories[2].items[j].ID * 10 + buff[i] - 48;
			i++;
		}
		inventories[2].items[j].name = itemPresets[inventories[2].items[j].ID].name;
		inventories[2].items[j].xPos = itemPresets[inventories[2].items[j].ID].xPos;
		inventories[2].items[j].yPos = itemPresets[inventories[2].items[j].ID].yPos;
		inventories[2].items[j].seed = itemPresets[inventories[2].items[j].ID].seed;
		inventories[2].items[j].sheetPosX = itemPresets[inventories[2].items[j].ID].sheetPosX;
		inventories[2].items[j].sheetPosY = itemPresets[inventories[2].items[j].ID].sheetPosY;
		i++;
		inventories[2].items[j].quantity = 0;
		while (buff[i] != ' ' && buff[i] != '\n' && buff[i] != '\0') {
			inventories[2].items[j].quantity = inventories[2].items[j].quantity * 10 + buff[i] - 48;
			i++;
		}
		i++;
		if (j == 8 || j == 17) {
			i = 0;
			fgets(buff, 256, fp);
		}
	}
	if (player.load == 1) {
		fgets(buff, 256, fp);
		i = 0;
		j = 0;
		int y;
		int id;
		while (buff[i] != '\n' && buff[i] != '\0') {
			droppedItems[j].ID = 0;
			id = 0;
			while (buff[i] != ' ') {
				droppedItems[j].ID = droppedItems[j].ID * 10 + buff[i] - 48;
				id = id * 10 + buff[i] - 48;
				i++;
			}
			droppedItems[j].name = itemPresets[droppedItems[j].ID].name;
			droppedItems[j].seed = itemPresets[droppedItems[j].ID].seed;
			droppedItems[j].sheetPosX = itemPresets[droppedItems[j].ID].sheetPosX;
			droppedItems[j].sheetPosY = itemPresets[droppedItems[j].ID].sheetPosY;

			i++;
			droppedItems[j].quantity = 0;
			while (buff[i] != ' ') {
				droppedItems[j].quantity = droppedItems[j].quantity * 10 + buff[i] - 48;
				i++;
			}
			i++;
			droppedItems[j].xPos = 0;
			while (buff[i] != ' ') {
				droppedItems[j].xPos = droppedItems[j].xPos * 10 + buff[i] - 48;
				i++;
			}
			i++;
			droppedItems[j].yPos = 0;
			while (buff[i] != ' ' && buff[i] != '\0') {
				droppedItems[j].yPos = droppedItems[j].yPos * 10 + buff[i] - 48;
				i++;
			}
			j++;
			if(buff[i] == ' ') i++;
			droppedLength++;
		}
	}
	
	fclose(fp);
}