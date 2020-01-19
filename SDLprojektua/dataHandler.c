#define _CRT_SECURE_NO_WARNINGS
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <string.h>
#include <stdio.h>

int getNextNode(char* str, int startPos);
struct JSON_Items parseItem(int line);
struct JSON_Quests parseQuest(int line);

//JSON fitxategitik ateratako objektuen informazioa gordetzeko struct-a
struct JSON_Items {
	int ID;
	int name_str;
	int sheetPosX;
	int sheetPosY;
	int seed;
};

//JSON fitxategitik ateratako misioen informazioa gordetzeko struct-a
struct JSON_Quests {
	int ID;
	int dialog_str;
	int action;
	int requiredItem;
	int requiredAmmount;
	int rewardItem;
	int rewardAmmount;
};

char dic_Items[128][128], dic_Quests[128][128];
struct Item itemPresets[128];

//dic_Items eta dic_Quests 'hiztegiak' hasieratu. Horretarako aukeratutako hizkuntzaren arabera fitxategi desberdinak irakurtzen dira.
void startDic(int language) {
	char* src[] = { "assets/data/EUS_items.txt", "assets/data/ESP_items.txt", "assets/data/ENG_items.txt" };
	FILE* fp;
	fp = fopen(src[language], "r");
	char buff[128];
	int i = 0;
	//While-aren iterazio bakoitzeko txt fitxategiaren hurrengo lerroa irakurri.
	while (feof(fp) == 0) {
		fgets(buff, 128, fp);
		//Buffer-etik lerroa jauzia ezabatu
		char* basura = strtok(buff, "\n");
		//Buffer-eko informazioa hiztegian gorde
		strcpy(dic_Items[i], buff);
		i++;
	}
	fclose(fp);

	//Item-ak bezala misioen informazioa jaso
	char* src2[] = { "assets/data/EUS_quests.txt", "assets/data/ESP_quests.txt", "assets/data/ENG_quests.txt" };
	fp = fopen(src2[language], "r");
	i = 0;
	while (feof(fp) == 0) {
		fgets(buff, 128, fp);
		char* basura = strtok(buff, "\n");
		strcpy(dic_Quests[i], buff);
		i++;
	}
	fclose(fp);
	return;
}

//JSON fitxategitik ateratako informazioa struct-etik itemPresets array-ra gorde. Array hau item 'hiztegi' edo bilduma bezala erabiltzen da.
void startPresests() {
	for (int i = 0; i < 20; i++) {
		struct JSON_Items itemData = parseItem(i);
		itemPresets[i].ID = itemData.ID;
		itemPresets[i].name = dic_Items[itemData.name_str];
		itemPresets[i].quantity = 1;
		itemPresets[i].sheetPosX = itemData.sheetPosX;
		itemPresets[i].sheetPosY = itemData.sheetPosY;
		itemPresets[i].seed = itemData.seed;
	}
	return;
}

//JSON fitxategia irakurri eta bildutako informazioarekin struct-a osatu eta itzuli.
struct JSON_Items parseItem(int line) {
	char* src = "assets/data/items.json";
	FILE* fp = fopen(src, "r");
	char buff[256];
	int i = 1;
	struct JSON_Items itemData = { 0, 0, 0, 0, 0 };
	fgets(buff, 256, fp);
	while (i <= line && feof(fp) == 0) {
		fgets(buff, 256, fp);
		i++;
	}
	i = getNextNode(buff, 0);
	while (buff[i] != ';') {
		itemData.ID = itemData.ID * 10 + buff[i] - 48;
		i++;
	}
	i = getNextNode(buff, i);
	while (buff[i] != ';') {
		itemData.name_str = itemData.name_str * 10 + buff[i] - 48;
		i++;
	}
	i = getNextNode(buff, i);
	while (buff[i] != ';') {
		itemData.sheetPosX = itemData.sheetPosX * 10 + buff[i] - 48;
		i++;
	}
	i = getNextNode(buff, i);
	while (buff[i] != ';') {
		itemData.sheetPosY = itemData.sheetPosY * 10 + buff[i] - 48;
		i++;
	}
	i = getNextNode(buff, i);
	while (buff[i] != ';') {
		itemData.seed = itemData.seed * 10 + buff[i] - 48;
		i++;
	}
	return itemData;
}

//Item-en antzera JSON struct-eko informazioa Quest struct bihurtu eta itzuli. Momentu oro misio bakar baten informazioa gordetzen da, ez dago bilduma array-rik.
struct Quest getQuest(int ID) {
	struct JSON_Quests questData = parseQuest(ID);
	struct Quest quest;

	quest.ID = questData.ID;
	quest.dialog_str = questData.dialog_str;
	quest.action = questData.action;
	quest.requiredItem = questData.requiredItem;
	quest.requiredAmmount = questData.requiredAmmount;
	quest.rewardItem = questData.rewardItem;
	quest.rewardAmmount = questData.rewardAmmount;
	quest.complete = 0;
	quest.completion = 0;
	
	return quest;
}

//Item-en informazioa irakurtzeko erabiltzen den metodo bera.
struct JSON_Quests parseQuest(int line) {
	char* src = "assets/data/quests.json";
	FILE* fp = fopen(src, "r");
	char buff[256];
	int i = 1;
	struct JSON_Quests questData = { 0, 0, 0, 0, 0, 0, 0 };
	fgets(buff, 256, fp);
	while (i <= line && feof(fp) == 0) {
		fgets(buff, 256, fp);
		i++;
	}
	i = getNextNode(buff, 0);
	while (buff[i] != ';') {
		questData.ID = questData.ID * 10 + buff[i] - 48;
		i++;
	}
	i = getNextNode(buff, i);
	while (buff[i] != ';') {
		questData.dialog_str = questData.dialog_str * 10 + buff[i] - 48;
		i++;
	}
	i = getNextNode(buff, i);
	while (buff[i] != ';') {
		questData.action = questData.action * 10 + buff[i] - 48;
		i++;
	}
	i = getNextNode(buff, i);
	while (buff[i] != ';') {
		questData.requiredItem = questData.requiredItem * 10 + buff[i] - 48;
		i++;
	}
	i = getNextNode(buff, i);
	while (buff[i] != ';') {
		questData.requiredAmmount = questData.requiredAmmount * 10 + buff[i] - 48;
		i++;
	}
	i = getNextNode(buff, i);
	while (buff[i] != ';') {
		questData.rewardItem = questData.rewardItem * 10 + buff[i] - 48;
		i++;
	}
	i = getNextNode(buff, i);
	while (buff[i] != ';') {
		questData.rewardAmmount = questData.rewardAmmount * 10 + buff[i] - 48;
		i++;
	}
	return questData;
}

//JSON fitxategian hurrengo 'nodo' edo informazio zatira aurreratu posizioa. Balioa beti ':' eta ';' bitartean egongo da.
int getNextNode(char* str, int startPos) {
	while (str[startPos] != ':' && str[startPos] != '\0') {
		startPos++;
	}
	startPos++;
	return startPos;
}