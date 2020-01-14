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
struct Item droppedItems[512];

void save() {
	int i = 0;
	int j = 0;
	int day = getDayTime();
	fp = fopen("assets/data/GUARDADO.txt", "w");
	fprintf(fp, "%i %i %i %i %i\n", player.x, player.y, player.status, player.energy, day); //datos player
	for (i = 0; i < 9; i++) {
		fprintf(fp, "%i %i", inventories[0].items[i].ID, inventories[0].items[i].quantity); //datos hotbar
		if (i != 8) fprintf(fp, " ");
	}
	fprintf(fp, "\n");

	for (i = 0; i < 18; i++) {
		fprintf(fp, "%i %i", inventories[1].items[i].ID, inventories[1].items[i].quantity); //datos inventario
		if (i != 8 && i != 17) fprintf(fp, " ");
		else if (i == 8) fprintf(fp, "\n");
	}
	fprintf(fp, "\n");

	for (i = 0; i < 27; i++) {
		fprintf(fp, "%i %i", inventories[2].items[i].ID, inventories[2].items[i].quantity); //datos cofre
		if (i != 8 && i != 17 && i != 26) fprintf(fp, " ");
		else if (i == 8 || i == 17) fprintf(fp, "\n");
	}
	fprintf(fp, "\n");

	fprintf(fp, "%i ", droppedLength);
	for (i = 0; i < droppedLength; i++) {
		fprintf(fp, "%i %i %i %i", droppedItems[i].ID, droppedItems[i].quantity, droppedItems[i].xPos, droppedItems[i].yPos); //datos drop
		if (i != droppedLength - 1) fprintf(fp, " ");
	}
	fprintf(fp, "\n");

	int lastWater;
	int time;
	for (i = 0; i < 49; i++) {
		lastWater = tiles[plantable_ID[i]].plant.lastWater;
		time = tiles[plantable_ID[i]].plant.time;
		fprintf(fp, "%i %i %i %i %i", tiles[plantable_ID[i]].plant.arado, tiles[plantable_ID[i]].plant.water, lastWater, tiles[plantable_ID[i]].plant.seed, time); //datos plantado
		if (i != 6 && i != 13 && i != 20 && i != 27 && i != 34 && i != 41 && i != 48) fprintf(fp, " ");
		else if (i == 6 || i == 13 || i == 20 || i == 27 || i == 34 || i == 41) fprintf(fp, "\n");
	}
	fprintf(fp, "\n");

	struct Quest quest = getCurrentQuest();
	fprintf(fp, "%i %i %i %i %i %i %i %i %i", quest.ID, quest.action, quest.complete, quest.completion, quest.dialog_str, quest.requiredAmmount, quest.requiredItem, quest.rewardAmmount, quest.rewardItem);

	fclose(fp);
}

void load() {
	int i = 0;
	char buff[512];
	char* src = "assets/data/GUARDADO_DEFAULT.txt";
	if (player.load == 1) {
		src = "assets/data/GUARDADO.txt";
		player.x = 0;
		player.y = 0;
		player.energy = 0;
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
	while (buff[i] != ' ') {
		player.status = buff[i] - 48;
		if (player.status == PAUSE_HOME) {
			player.status = HOME;
			camera.x = 0;
			camera.y = 0;
		}
		else if (player.status == PAUSE) player.status = PLAYING;
		i++;
	}
	i++;
	while (buff[i] != ' ') {
		player.energy = player.energy * 10 + buff[i] - 48;
		i++;
	}
	i++;
	int time = 0;
	while (buff[i] != '\n') {
		time = time * 10 + buff[i] - 48;
		i++;
	}
	loadDayTime(time);
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
	if (player.load == 1) { //cargar items droppeados && cargar plantado
		for (i = 0; i < 512; i++) {
			droppedItems[i].ID = 0;
			droppedItems[i].quantity = 0;
			droppedItems[i].xPos = 0;
			droppedItems[i].yPos = 0;
		}
		fgets(buff, 512, fp);
		i = 0;
		j = 0;
		if (buff[i] > 0) {
			i += 2;
			while (buff[i] != '\n' && buff[i] != '\0') { //cargar items droppeados
				while (buff[i] != ' ') {
					droppedItems[j].ID = droppedItems[j].ID * 10 + buff[i] - 48;
					droppedItems[j].name = itemPresets[droppedItems[j].ID].name;
					droppedItems[j].seed = itemPresets[droppedItems[j].ID].seed;
					droppedItems[j].sheetPosX = itemPresets[droppedItems[j].ID].sheetPosX;
					droppedItems[j].sheetPosY = itemPresets[droppedItems[j].ID].sheetPosY;
					i++;
				}
				i++;
				while (buff[i] != ' ') {
					droppedItems[j].quantity = droppedItems[j].quantity * 10 + buff[i] - 48;
					i++;
				}
				i++;
				while (buff[i] != ' ') {
					droppedItems[j].xPos = droppedItems[j].xPos * 10 + buff[i] - 48;
					i++;
				}
				i++;
				while (buff[i] != ' ' && buff[i] != '\0' && buff[i] != '\n') {
					droppedItems[j].yPos = droppedItems[j].yPos * 10 + buff[i] - 48;
					i++;
				}
				j++;
				droppedLength++;
				if (buff[i] == ' ') i++;
			}
		}

		for (j = 0; j < 49; j++) { //cargar tiles
			if (j % 7 == 0) {
				i = 0;
				fgets(buff, 216, fp);
			}
			tiles[plantable_ID[j]].plant.arado = 0;
			tiles[plantable_ID[j]].plant.water = 0;
			tiles[plantable_ID[j]].plant.lastWater = 0;
			tiles[plantable_ID[j]].plant.seed = 0;
			tiles[plantable_ID[j]].plant.time = 0;
			tiles[plantable_ID[j]].ID = j;
			while (buff[i] != ' ') {
				tiles[plantable_ID[j]].plant.arado = tiles[plantable_ID[j]].plant.arado * 10 + buff[i] - 48;
				i++;
			}
			i++;
			while (buff[i] != ' ') {
				tiles[plantable_ID[j]].plant.water = tiles[plantable_ID[j]].plant.water * 10 + buff[i] - 48;
				i++;
			}
			i++;
			while (buff[i] != ' ') {
				tiles[plantable_ID[j]].plant.lastWater = tiles[plantable_ID[j]].plant.lastWater * 10 + buff[i] - 48;
				i++;
			}
			i++;
			while (buff[i] != ' ') {
				tiles[plantable_ID[j]].plant.seed = tiles[plantable_ID[j]].plant.seed * 10 + buff[i] - 48;
				i++;
			}
			i++;
			while (buff[i] != ' ' && buff[i] != '\n' && buff[i] != '\0') {
				tiles[plantable_ID[j]].plant.time = tiles[plantable_ID[j]].plant.time * 10 + buff[i] - 48;
				i++;
			}
			i++;
		}

		fgets(buff, 216, fp);
		struct Quest quest;
		i = 0;
		quest.ID = 0;
		quest.action = 0;
		quest.complete = 0;
		quest.completion = 0;
		quest.dialog_str = 0;
		quest.requiredAmmount = 0;
		quest.requiredItem = 0;
		quest.rewardAmmount = 0;
		quest.rewardItem = 0;
		while (buff[i] != ' ') {
			quest.ID = quest.ID * 10 + buff[i] - 48;
			i++;
		}
		i++;
		while (buff[i] != ' ') {
			quest.action = quest.action * 10 + buff[i] - 48;
			i++;
		}
		i++;
		while (buff[i] != ' ') {
			quest.complete = quest.complete * 10 + buff[i] - 48;
			i++;

		}
		i++;
		while (buff[i] != ' ') {
			quest.completion = quest.completion * 10 + buff[i] - 48;
			i++;

		}
		i++;
		while (buff[i] != ' ') {
			quest.dialog_str = quest.dialog_str * 10 + buff[i] - 48;
			i++;
		}
		i++;
		while (buff[i] != ' ') {
			quest.requiredAmmount = quest.requiredAmmount * 10 + buff[i] - 48;
			i++;
		}
		i++;
		while (buff[i] != ' ') {
			quest.requiredItem = quest.requiredItem * 10 + buff[i] - 48;
			i++;
		}
		i++;
		while (buff[i] != ' ') {
			quest.rewardAmmount = quest.rewardAmmount * 10 + buff[i] - 48;
			i++;
		}
		i++;
		while (buff[i] != '\0' && buff[i] != '\n') {
			quest.rewardItem = quest.rewardItem * 10 + buff[i] - 48;
			i++;
		}

		loadQuest(quest);
	}

	

	fclose(fp);
}