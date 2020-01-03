#define _CRT_SECURE_NO_WARNINGS
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <libxml/parser.h>
#include <stdio.h>

char dic_Items[128][128], dic_Quests[128];
struct Item itemPresets[128];

void startDic(int language) {
	char* src = "assets/data/ESP_items.txt";
	FILE* fp;
	fp = fopen(src, "r");
	char buff[128];
	int i = 0;
	while (feof(fp) == 0) {
		fgets(buff, 128, fp);
		strtok(buff, "\n");
		strcpy(dic_Items[i], buff);
		i++;
	}

	fclose(fp);
	return;
}

void startPresests() {
	char* src = "assets/data/items.xml";
	xmlDocPtr *doc = NULL;
	xmlNode* cur = NULL;
	xmlChar *key;
	doc = xmlReadFile(src, NULL, 0);

	if (doc != NULL) {
		int i = 0;
		cur = xmlDocGetRootElement(doc);
		while (cur != NULL) {
			if ((!xmlStrcmp(cur->name, (const xmlChar*)"ID"))) {
				key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
				itemPresets[i].ID = key;
				xmlFree(key);
			}
			cur = cur->next;
			i++;
		}
	}

	for (int i = 0; i < 20; i++) {
		itemPresets[i].name = dic_Items[i];
		itemPresets[i].quantity = 1;
		itemPresets[i].seed = 1;
		itemPresets[i].sheetPosX = 0;
		itemPresets[i].sheetPosY = 64;
	}
	return;
}