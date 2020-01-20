#define _CRT_SECURE_NO_WARNINGS
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>
#include <time.h>

//Textu motako dokumentua irakurtzeko bariablea sortzea
FILE* fp;
struct Item droppedItems[512];

//Jokuan dagoen informazio guztia .txt formatuko dokumentu batean gordetzea
void save(int slot) {
	int i = 0;
	int j = 0;
	int day = (int)getDayTime();

	//Bi partida gordetzeko ahalmena dago. Aukeratutakoaren arabera, dokumentu ezberdin bat irekiko du.
	if(slot == 1) fp = fopen("assets/data/GUARDADO.txt", "w");
	else if (slot == 2) fp = fopen("assets/data/GUARDADO2.txt", "w");

	//Datu guztiak gordetzea
	//Player-aren datuak
	fprintf(fp, "%i %i %i %i %i\n", player.x, player.y, player.status, player.energy, day);

	//Hotbar-aren datuak
	for (i = 0; i < 9; i++) {
		fprintf(fp, "%i %i", inventories[0].items[i].ID, inventories[0].items[i].quantity);
		if (i != 8) fprintf(fp, " ");
	}
	fprintf(fp, "\n");

	//Inbentarioaren datuak
	for (i = 0; i < 18; i++) {
		fprintf(fp, "%i %i", inventories[1].items[i].ID, inventories[1].items[i].quantity);
		if (i != 8 && i != 17) fprintf(fp, " ");
		else if (i == 8) fprintf(fp, "\n");
	}
	fprintf(fp, "\n");

	//Kutxaren datuak
	for (i = 0; i < 27; i++) {
		fprintf(fp, "%i %i", inventories[2].items[i].ID, inventories[2].items[i].quantity);
		if (i != 8 && i != 17 && i != 26) fprintf(fp, " ");
		else if (i == 8 || i == 17) fprintf(fp, "\n");
	}
	fprintf(fp, "\n");

	//Lurrean dauden objetuen datuak
	fprintf(fp, "%i ", droppedLength);
	for (i = 0; i < droppedLength; i++) {
		fprintf(fp, "%i %i %i %i %i", droppedItems[i].ID, droppedItems[i].quantity, droppedItems[i].xPos, droppedItems[i].yPos, droppedItems[i].status);
		if (i != droppedLength - 1) fprintf(fp, " ");
	}
	fprintf(fp, "\n");

	int lastWater;
	int time;
	//Landa eremuaren datuak
	for (i = 0; i < 49; i++) {
		lastWater = (int)tiles[plantable_ID[i]].plant.lastWater;
		time = (int)tiles[plantable_ID[i]].plant.time;
		fprintf(fp, "%i %i %i %i %i", tiles[plantable_ID[i]].plant.arado, tiles[plantable_ID[i]].plant.water, lastWater, tiles[plantable_ID[i]].plant.seed, time);
		if (i != 6 && i != 13 && i != 20 && i != 27 && i != 34 && i != 41 && i != 48) fprintf(fp, " ");
		else if (i == 6 || i == 13 || i == 20 || i == 27 || i == 34 || i == 41) fprintf(fp, "\n");
	}
	fprintf(fp, "\n");

	//sturct bat sortzea, zein momentuko betebeharraren datu berdinak izango dituen
	struct Quest quest = getCurrentQuest();
	//Momentuan dagoen betebeharraren datuak
	fprintf(fp, "%i %i %i %i %i %i %i %i %i", quest.ID, quest.action, quest.complete, quest.completion, quest.dialog_str, quest.requiredAmmount, quest.requiredItem, quest.rewardAmmount, quest.rewardItem);

	fclose(fp);
}

//Textu motako dokumentua ireki eta irakurtzen da. Ondoren datu hauek jokuan sartzen dira, utzi den puntutik jarraitzeko
void load(int which) {
	//Ez bada partidarik kargatzen, jokalaria hasieratu
	if(which == 0) 	player = createPlayer();
	int i = 0;
	char buff[512];
	//Aukeratutakoaren arabera, dokumentu ezberdinak irakurtzea
	char* src = "assets/data/GUARDADO_DEFAULT.txt";
	if (which == 1) {
		src = "assets/data/GUARDADO.txt";
	}
	else if (which == 2) {
		src = "assets/data/GUARDADO2.txt";
	}
	fp = fopen(src, "r");

	//Jokalariaren informazioa irakurtzea
	fgets(buff, 256, fp);
	int xPos = 0;
	int yPos = 0;
	int energy = 0;
	int time = 0;
	while (buff[i] != ' ') {
		xPos = xPos * 10 + buff[i] - 48;
		i++;
	}
	i++;
	while (buff[i] != ' ') {
		yPos = yPos * 10 + buff[i] - 48;
		i++;
	}
	i++;
	if (which ==1 || which == 2) {
		player.status = buff[i] - 48;
		if (player.status == SAVE_HOME) {
			player.status = HOME;
			camera.x = 0;
			camera.y = 0;
			inventories[INV_HOTBAR].yPos = 3;
			inventories[INV_PLAYER].yPos = 90;
		}
		else if (player.status == SAVE) {
			player.status = PLAYING;
			inventories[INV_HOTBAR].yPos = SCREEN_HEIGHT - 3 - 64;
			inventories[INV_PLAYER].yPos = SCREEN_HEIGHT - 64 * 3 - 9;
		}
	}
	i++;
	i++;
	while (buff[i] != ' ') {
		energy = energy * 10 + buff[i] - 48;
		i++;
	}
	i++;
	while (buff[i] != '\n') {
		time = time * 10 + buff[i] - 48;
		i++;
	}
	player.x = xPos;
	player.y = yPos;
	player.energy = energy;
	setDayTime(time);

	//Hotbar-aren informazioa irakurtzea
	fgets(buff, 256, fp);
	i = 0;
	int j;
	for (j = 0; j < 9; j++) {
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

	//Inbentarioaren informazioa irakurtzea
	fgets(buff, 256, fp);
	i = 0;
	for (j = 0; j < 18; j++) {
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

	//Kutxaren informazioa kargatzea
	fgets(buff, 256, fp);
	i = 0;
	for (j = 0; j < 27; j++) {
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
	//Partida hasieratik kargatzean ez da irakurtzen, balore guztiak '0' direkalo
	if (which == 1 || which == 2) {
		//Lurrean dauden objetuen array-a garbitzea, berriak kargatu ahal izateko
		for (i = 0; i < 512; i++) {
			droppedItems[i].ID = 0;
			droppedItems[i].quantity = 0;
			droppedItems[i].xPos = 0;
			droppedItems[i].yPos = 0;
			droppedItems[i].status = 0;
			droppedItems[i].name = 0;
			droppedItems[i].seed = 0;
			droppedItems[i].sheetPosX = 0;
			droppedItems[i].sheetPosY = 0;
		}
		fgets(buff, 512, fp);
		i = 0;
		j = 0;
		//Lurreko objetuen kopurua 0 baino gehiago bada lurrean dauden objetuak kargatzen dira
		if (buff[i] > 0) {
			i += 2;
			//Lurrean dauden objetuak kargatzea
			while (buff[i] != '\n' && buff[i] != '\0') {
				while (buff[i] != ' ') {
					droppedItems[j].ID = droppedItems[j].ID * 10 + buff[i] - 48;
					i++;
				}
				droppedItems[j].name = itemPresets[droppedItems[j].ID].name;
				droppedItems[j].seed = itemPresets[droppedItems[j].ID].seed;
				droppedItems[j].sheetPosX = itemPresets[droppedItems[j].ID].sheetPosX;
				droppedItems[j].sheetPosY = itemPresets[droppedItems[j].ID].sheetPosY;
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
				while (buff[i] != ' ') {
					droppedItems[j].yPos = droppedItems[j].yPos * 10 + buff[i] - 48;
					i++;
				}
				i++;
				while (buff[i] != ' ' && buff[i] != '\0' && buff[i] != '\n') {
					droppedItems[j].status = droppedItems[j].status * 10 + buff[i] - 48;
					i++;
				}
				j++;
				droppedLength++;
				if (buff[i] == ' ') i++;
			}
		}
		//Landa eremua kargatzea
		for (j = 0; j < 49; j++) {
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
		//struct bat sortzea ondoren datu horiek momentuko betebeharrera pasatzeko
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
		//Betebeharraren datuak irakurtzea
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

		setCurrentQuest(quest);
	}
	fclose(fp);
}