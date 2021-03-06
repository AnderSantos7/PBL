#define _CRT_SECURE_NO_WARNINGS
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <string.h>
#include <stdlib.h>

//Momentu oroko misioari buruzko informazioa
struct Quest currentQuest = {-1, 0, 0, 0, 0, 0};

void giveReward();
void getNextQuest();
void completeQuest();
struct Quest generateRandomQuest();
void showQuestDialog();

int completeAnimation = 0;
//Misioa bukatzean saria jokalariaren inbentarion sartu edo lurrera botatzeko
void giveReward() {
	if (!insertItem(0, itemPresets[currentQuest.rewardItem], currentQuest.rewardAmmount, -1)) {
		if (!insertItem(1, itemPresets[currentQuest.rewardItem], currentQuest.rewardAmmount, -1)) {
			dropItem(player.facingTile, currentQuest.rewardItem, currentQuest.rewardAmmount);
		}
	}
	return;
}

//Hurrengo misioa lortzeko funtzioa. Hasierako misioak dokumentutik irakurtzen dira, hauek bukatutakoan ausazkoak sortzen dira.
void getNextQuest() {
	int lastQuest = 14;
	if (currentQuest.ID >= lastQuest) {
		currentQuest = generateRandomQuest();
	}else{
		currentQuest = getQuest(currentQuest.ID + 1);
	}
	return;
}

//Akzio bat egitean misioaren akzioarekin bat datorren konprobatu. Horrela bada misioaren kontagailura gehitzen da.
//Misioak eskatutako akzio kopurua burutzean, misioa bukatzen da.
void checkQuestCompletion(int action, int item, int ammount) {
	 if (!currentQuest.complete && action == currentQuest.action && (item == currentQuest.requiredItem || currentQuest.requiredItem == 0))
	{
		currentQuest.completion += ammount;
		if (currentQuest.completion >= currentQuest.requiredAmmount) {
			completeQuest();
		}
	}
	return;
}

//Misioa bukatzean animazioa deitzen da.
void completeQuest() {
	currentQuest.complete = 1;
	completeAnimation = 1;
	return;
}

//Misioa bukatu eta menuan 'bukatu' botoia sakatzean saria jokalaria ematen zaio, hurrengo misioa hartzen da eta dendariak ea hazirik desblokeatzen duen konprobatu
void acceptReward() {
	if (currentQuest.complete) {
		unlockShopItem(currentQuest.ID);
		giveReward();
		getNextQuest();
	}
	return;
}

//'Entrega' motako misioak egiteko funtzioa. Lehenik inbentarioan eskatzen den motako zenbat item daukan kontatzen du. Misioa bukatzeko adina badu, beharrezkoa dena kenduko zaio.
//Ez badu nahikoa, duena hartuko da eta misioaren kontagailuari gehituko zaio. Hurrengoan aurretik entregatutako kopurua kontuan izango da.
int deliverQuest() {
	int success = 0;
	if (currentQuest.action == ENTREGA && !currentQuest.complete) {
		success = 1;
		int toBeDelivered = checkHowManyOfItem(currentQuest.requiredItem);
		int missingForQuest = currentQuest.requiredAmmount - currentQuest.completion;
		if (toBeDelivered > 0) {
			if (toBeDelivered >= missingForQuest) {
				removeCertainItem(currentQuest.requiredItem, missingForQuest);
				currentQuest.completion += currentQuest.requiredAmmount;
			}
			else {
				removeCertainItem(currentQuest.requiredItem, toBeDelivered);
				currentQuest.completion += toBeDelivered;
			}
		}
		if (currentQuest.completion >= currentQuest.requiredAmmount) {
			currentQuest.completion = currentQuest.requiredAmmount;
			completeQuest();
		}
	}
	return success;
}

//Momentuan abian den betebeharraren informazioa pantailan ezkerrean goiko ertzean pantailaratzen da.
void showCurrentQuest() {
	if (currentQuest.ID >= 0) {
		int xOffset = 16, yOffset = 16;
		char str[128];
		char tmp[128];
		TTF_Font* font;
		font = TTF_OpenFont("assets/fonts/y.n.w.u.a.y.ttf", 16);
		SDL_Color color = { 255, 255, 255 };
		strcpy(str, dic_Quests[8]);
		strcat(str, ":");
		SDL_Surface* s = TTF_RenderText_Solid(font, str, color);
		SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
		int w, h;
		SDL_QueryTexture(t, NULL, NULL, &w, &h);
		aplikatuSurface(xOffset, yOffset, w, h, t, NULL);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_Rect r = { xOffset, h + yOffset, w, 2 };
		SDL_RenderFillRect(renderer, &r);
		float percentil = currentQuest.completion / (float)currentQuest.requiredAmmount;
		SDL_Rect percent = { xOffset + 8, 2.5 * h + yOffset, percentil * w / 2, 8 };
		SDL_RenderFillRect(renderer, &percent);
		percent.w = w / 2;
		SDL_RenderDrawRect(renderer, &percent);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
		SDL_RenderFillRect(renderer, &percent);

		TTF_CloseFont(font);
		SDL_FreeSurface(s);
		SDL_DestroyTexture(t);

		font = TTF_OpenFont("assets/fonts/y.n.w.u.a.y.ttf", 12);
		SDL_itoa(percentil * 100, tmp, 10);
		strcat(tmp, "%");
		if (font != NULL) s = TTF_RenderText_Solid(font, tmp, color);
		t = SDL_CreateTextureFromSurface(renderer, s);
		int wBar = w / 2;
		SDL_QueryTexture(t, NULL, NULL, &w, &h);
		aplikatuSurface(xOffset + wBar + 16, 2.5 * h + yOffset + 8, w, h, t, NULL);

		SDL_FreeSurface(s);
		SDL_DestroyTexture(t);

		strcpy(str, dic_Quests[currentQuest.action]);
		strcat(str, ": ");

		if (currentQuest.requiredItem != 0) {
			if (currentQuest.action == PLANT || currentQuest.action == WATER || currentQuest.action == FERTILIZAR) {
				strcat(str, itemPresets[seedToItem(currentQuest.requiredItem)].name);
			}
			else {
				strcat(str, itemPresets[currentQuest.requiredItem].name);
			}
		}
		else {
			strcat(str, dic_Quests[9]);
		}
		strcat(str, " ");
		SDL_itoa(currentQuest.completion, tmp, 10);
		strcat(str, tmp);
		strcat(str, "/");
		SDL_itoa(currentQuest.requiredAmmount, tmp, 10);
		strcat(str, tmp);
		s = TTF_RenderText_Solid(font, str, color);
		t = SDL_CreateTextureFromSurface(renderer, s);
		SDL_QueryTexture(t, NULL, NULL, &w, &h);
		aplikatuSurface(xOffset + 6, h + 14 + yOffset, w, h, t, NULL);

		SDL_FreeSurface(s);
		SDL_DestroyTexture(t);
		TTF_CloseFont(font);
	}
	return;
}

double animTime;
//Misioa bukatzean 'Mision Complete!' animazioa. Hasieran maskara erabiltzen da textua zatika bistaratzeko. Ondoren ez da irudikatzen eta ondoren berriz marrazten da
//'Sweep' eta 'flash' erako efektuak lortzeko.
void questCompleteAnim(double deltaTime) {
	if (completeAnimation) {
		char* str = dic_Quests[10];
		TTF_Font* font;
		int fontSize = 36;
		if (language == EUS) fontSize = 28;
		font = TTF_OpenFont("assets/fonts/y.n.w.u.a.y.ttf", fontSize);
		SDL_Color color = { 255, 255, 255 };
		SDL_Surface* s = TTF_RenderText_Solid(font, str, color);
		SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
		int w, h;
		SDL_QueryTexture(t, NULL, NULL, &w, &h);
		if (animTime < 1) {
			int clipW = animTime * w;
			SDL_Rect clip = {0, 0, clipW, h};
			aplikatuSurface(SCREEN_WIDTH / 2 - w / 2, 128, clipW, h, t, &clip);
		}else if ((animTime <= 2 && animTime >= 1.5) || (animTime <= 3 && animTime >= 2.5)) {
			aplikatuSurface(SCREEN_WIDTH / 2 - w / 2, 128, w, h, t, NULL);
		}else if((animTime > 2 && animTime < 2.5) || animTime > 3){
			completeAnimation = 0;
			animTime = 0;
		}
		TTF_CloseFont(font);
		SDL_FreeSurface(s);
		SDL_DestroyTexture(t);
		animTime += deltaTime;
	}
	return;
}

int questMenuOpen = 0;
//Misioen menua zabaldu eta ixteko 'setter'-a
void setQuestMenuState(int state) {
	questMenuOpen = state;
	return;
}

//Misioen menua zabalik dagoen ikusteko 'getter'-a
int getQuestMenuState() {
	return questMenuOpen;
}

//Misioen menua marrazteko funtzio laburra. Hasieran 'hasi' botoia soilik marrazten da. Misioekin hastean berriz: dialogoa, saria eta behar eta egin diren akzio kopurua.
//'Bukatu' edo 'Entregatu' botoia ere marrazten da.
void showQuestMenu() {
	if (questMenuOpen) {
		SDL_Rect clip = { 0, 533, 320, 180 };
		int xPos = (SCREEN_WIDTH - clip.w * 1.5) / 2;
		int yPos = (SCREEN_HEIGHT - clip.h * 1.5) / 2;
		aplikatuSurface(xPos, yPos, clip.w * 1.5, clip.h * 1.5, textures[HUDSurface], &clip);
		if (currentQuest.ID >= 0) {
			int xOffset = 250;
			int yOffset = 190;
			clip.x = 201;
			clip.y = 477;
			clip.w = 200;
			clip.h = 50;
			aplikatuSurface(xPos + xOffset, yPos + yOffset, clip.w, clip.h, textures[HUDSurface], &clip);
			SDL_Rect button = { 334, 299, 190, 40 };
			if (checkIfClicking(&button)) {
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 16);
				SDL_RenderFillRect(renderer, &button);
			}

			TTF_Font* font;
			SDL_Surface* s;
			font = TTF_OpenFont("assets/fonts/y.n.w.u.a.y.ttf", 18);
			SDL_Color color = { 255, 255, 255 };
			if (!currentQuest.complete && currentQuest.action == ENTREGA) {
				if (checkHowManyOfItem(currentQuest.requiredItem) == 0) {
					color.r = 160;
					color.g = 60;
					color.b = 60;
				}
				s = TTF_RenderText_Solid(font, dic_Quests[ENTREGA], color);
			}
			else {
				if (!currentQuest.complete) {
					color.r = 160;
					color.g = 60;
					color.b = 60;
				}
				s = TTF_RenderText_Solid(font, dic_Quests[6], color);
			}
			SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
			int w, h;
			SDL_QueryTexture(t, NULL, NULL, &w, &h);
			int x = xPos + xOffset + (clip.w / 2) - w / 2;
			int y = yPos + yOffset + (clip.h / 2) - h / 2;
			aplikatuSurface(x, y, w, h, t, NULL);
			SDL_FreeSurface(s);
			SDL_DestroyTexture(t);
			TTF_CloseFont(font);

			char str[128], tmp[128];
			font = TTF_OpenFont("assets/fonts/y.n.w.u.a.y.ttf", 16);
			strcpy(str, dic_Quests[currentQuest.action]);
			strcat(str, ": ");

			color.r = 255;
			color.g = 255;
			color.b = 255;
			s = TTF_RenderText_Solid(font, str, color);
			t = SDL_CreateTextureFromSurface(renderer, s);
			SDL_QueryTexture(t, NULL, NULL, &w, &h);
			x = 334;
			y = 299;
			aplikatuSurface(x, y - 160, w, h, t, NULL);

			SDL_FreeSurface(s);
			SDL_DestroyTexture(t);
			str[0] = '\0';
			if (currentQuest.requiredItem != 0) {
				if (currentQuest.action == PLANT || currentQuest.action == WATER || currentQuest.action == FERTILIZAR) {
					strcpy(str, itemPresets[seedToItem(currentQuest.requiredItem)].name);
				}
				else {
					strcpy(str, itemPresets[currentQuest.requiredItem].name);
				}

			}
			else {
				strcpy(str, dic_Quests[9]);
			}
			strcat(str, " ");
			SDL_itoa(currentQuest.completion, tmp, 10);
			strcat(str, tmp);
			strcat(str, "/");
			SDL_itoa(currentQuest.requiredAmmount, tmp, 10);
			strcat(str, tmp);

			s = TTF_RenderText_Solid(font, str, color);
			t = SDL_CreateTextureFromSurface(renderer, s);
			SDL_QueryTexture(t, NULL, NULL, &w, &h);
			aplikatuSurface(x, y - 160 + h * 1.5, w, h, t, NULL);

			SDL_FreeSurface(s);
			SDL_DestroyTexture(t);
			TTF_CloseFont(font);

			SDL_Rect iClip = { itemPresets[currentQuest.requiredItem].sheetPosX, itemPresets[currentQuest.requiredItem].sheetPosY, 64, 64 };
			if (currentQuest.action == ARAR) {
				iClip.x = itemPresets[1].sheetPosX;
				iClip.y = itemPresets[1].sheetPosY;
			}
			else if (currentQuest.action == PLANT || currentQuest.action == WATER || currentQuest.action == FERTILIZAR) {
				int item = seedToItem(currentQuest.requiredItem) + 1;
				iClip.x = itemPresets[item].sheetPosX;
				iClip.y = itemPresets[item].sheetPosY;
			}
			if (currentQuest.action == WATER && currentQuest.requiredItem == 0) {
				iClip.x = itemPresets[2].sheetPosX;
				iClip.y = itemPresets[2].sheetPosY;
			}
			aplikatuSurface(x - 20, y - 84, 64, 64, textures[itemsSurface], &iClip);

			SDL_Rect r = { x + 60, y - 64, 120, 32 };
			int boxSize = r.w / currentQuest.requiredAmmount;
			r.w = boxSize * currentQuest.completion;
			SDL_SetRenderDrawColor(renderer, 128, 255, 128, 128);
			SDL_RenderFillRect(renderer, &r);
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			r.w = 120;
			for (int i = 0; i < currentQuest.requiredAmmount; i++) {
				SDL_Rect box = { r.x + boxSize * i, y - 64, boxSize, 32 };
				SDL_RenderDrawRect(renderer, &box);
			}
			font = TTF_OpenFont("assets/fonts/y.n.w.u.a.y.ttf", 14);

			strcpy(str, dic_Quests[7]);
			strcat(str, ":");

			s = TTF_RenderText_Solid(font, str, color);
			t = SDL_CreateTextureFromSurface(renderer, s);
			SDL_QueryTexture(t, NULL, NULL, &w, &h);
			int xOffsetReward = 100;
			aplikatuSurface(xOffsetReward, y + 16, w, h, t, NULL);
			SDL_FreeSurface(s);
			SDL_DestroyTexture(t);

			SDL_Rect rClip = { itemPresets[currentQuest.rewardItem].sheetPosX, itemPresets[currentQuest.rewardItem].sheetPosY, 64, 64 };
			if (currentQuest.rewardAmmount == 0) {
				rClip.x = 448; rClip.y = 0;
			}
			xOffsetReward += w + 4;
			aplikatuSurface(xOffsetReward, y - 8, 64, 64, textures[itemsSurface], &rClip);
			xOffsetReward += 48;

			if (currentQuest.rewardAmmount > 0) {
				SDL_itoa(currentQuest.rewardAmmount, tmp, 10);
				s = TTF_RenderText_Solid(font, tmp, color);
				t = SDL_CreateTextureFromSurface(renderer, s);
				SDL_QueryTexture(t, NULL, NULL, &w, &h);
				aplikatuSurface(xOffsetReward, y + 32, w, h, t, NULL);

				SDL_FreeSurface(s);
				SDL_DestroyTexture(t);
			}
			TTF_CloseFont(font);
			showQuestDialog();
		}else {
		TTF_Font* font = TTF_OpenFont("assets/fonts/y.n.w.u.a.y.ttf", 18);
		SDL_Color color = { 255, 255, 255 };
		SDL_Surface* s = TTF_RenderText_Solid(font, dic_Quests[11], color);
		SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
		int w, h;
		SDL_QueryTexture(t, NULL, NULL, &w, &h);
		SDL_Rect button = { (SCREEN_WIDTH - 190) / 2, (SCREEN_HEIGHT - 60) / 2, 190, 60};
		SDL_Rect clip = { 0, 533, 320, 180 };

		aplikatuSurface(button.x, button.y, button.w, button.h, textures[HUDSurface], &clip);

		if (checkIfClicking(&button)) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 16);
			SDL_RenderFillRect(renderer, &button);
		}
		
		aplikatuSurface((SCREEN_WIDTH - w) / 2, (SCREEN_HEIGHT - h) / 2, w, h, t, NULL);

		TTF_CloseFont(font);
		SDL_DestroyTexture(t);
		SDL_FreeSurface(s);
		}
	}
	return;
}

//Misioen dialogoa irudikatzeko funtzioa. TXT fitxategitik dialogoa hartu eta asteriskoak '*' lerroak bereizten dituzte.
void showQuestDialog() {
	TTF_Font* font = TTF_OpenFont("assets/fonts/y.n.w.u.a.y.ttf", 9);
	SDL_Color color = { 255, 255, 255 };
	char str[256];
	if (currentQuest.ID < 15) strcpy(str, dic_Quests[currentQuest.dialog_str + 12]);
	else strcpy(str, dic_Quests[27]);
	int i = 0;
	char* rest = str;
	char* token;
	while (token = strtok_s(rest, "*", &rest)) {
		SDL_Surface* s = TTF_RenderText_Solid(font, token, color);
		SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
		int w, h;
		SDL_QueryTexture(t, NULL, NULL, &w, &h);
		int xPos = 80 + 20;
		int yPos = 115 + 10 + h * i * 2;
		aplikatuSurface(xPos, yPos, w, h, t, NULL);
		token = strtok(str, "*");
		SDL_FreeSurface(s);
		SDL_DestroyTexture(t);
		i++;
	}
	TTF_CloseFont(font);
	return;
}

//Misioen menuko botoiekin interaktuatzeko.
void interactQuestMenu() {
	if (currentQuest.ID < 0) {
		SDL_Rect button = { (SCREEN_WIDTH - 190) / 2, (SCREEN_HEIGHT - 60) / 2, 190, 60 };
		if (checkIfClicking(&button)) getNextQuest();
	}else {
		SDL_Rect button = { 334, 299, 190, 40 };
		if (checkIfClicking(&button)) {
			if (!deliverQuest()) acceptReward();
		}
	}

	return;
}

int upgraded = 0;
//Misio infinitu desberdin sortzeko funtzioa. Ausazko akzio eta kopuru bat aukeratzen da (max 20). Akzioaren arabera item batzuetatik aukeratzen da bat
//(landatu, ureztatu eta ongarritzeko --> haziak || uzta bildu eta entregatzeko --> barazkiak). Saria beti txanpona izango da.
//Sari kopuruaren minimoa beharrezko akzio kopuruaren 1/4 eta 1/2 artekoa da. Minimoa beti 1 izanda.
struct Quest generateRandomQuest() {
	struct Quest quest;
	int seedKopurua = 4;
	if (upgraded) seedKopurua = 5;
	int maxRequired = 20;

	quest.ID = currentQuest.ID + 1;
	quest.action = rand() % 6;

	if (quest.action == PLANT || quest.action == WATER || quest.action == FERTILIZAR) {
		quest.requiredItem = rand() % seedKopurua + 1;
	}
	else if(quest.action == ARAR){
		quest.requiredItem = 0;
	}
	else {
		int harvest[] = { 4, 6, 8, 10, 12, 14};	
		quest.requiredItem = harvest[rand() % (seedKopurua + 1)];
	}

	quest.requiredAmmount = rand() % maxRequired + 1;

	quest.rewardItem = 17;
	int maxReward = quest.requiredAmmount / 2 + 1;
	quest.rewardAmmount = 0;
	while(quest.rewardAmmount < quest.requiredAmmount / 4) quest.rewardAmmount = rand() % maxReward;
	if (quest.rewardAmmount == 0) quest.rewardAmmount = 1;
	quest.complete = 0;
	quest.completion = 0;
	return quest;
}

//Azkeneko landarearen hazia erostean misio gehiago desblokeatzen dira.
void upgradeQuests() {
	upgraded = 1;
	return;
}

//Momentuko misioa lortzeko 'getter'-a
struct Quest getCurrentQuest() {
	return currentQuest;
}

//Misioa ezartzeko 'setter'-a
void setCurrentQuest(struct Quest quest) {
	currentQuest = quest;
	return;
}