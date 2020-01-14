#define _CRT_SECURE_NO_WARNINGS
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <string.h>
#include <stdlib.h>

struct Quest currentQuest = {-1, 0, 0, 0, 0, 0};

void giveReward();
void getNextQuest();
void completeQuest();
struct Quest generateRandomQuest();

int completeAnimation = 0;
void giveReward() {
	if (!insertItem(0, itemPresets[currentQuest.rewardItem], currentQuest.rewardAmmount, -1)) {
		if (!insertItem(1, itemPresets[currentQuest.rewardItem], currentQuest.rewardAmmount, -1)) {
			dropItem(player.facingTile, currentQuest.rewardItem, currentQuest.rewardAmmount);
		}
	}
	SDL_SetRenderDrawColor(renderer, 128, 255, 0, 255);
	SDL_RenderFillRect(renderer, NULL);
	return;
}

void getNextQuest() {
	int lastQuest = 1;
	if (currentQuest.ID >= lastQuest) {
		currentQuest = generateRandomQuest();
	}else{
		currentQuest = getQuest(currentQuest.ID + 1);
	}
	return;
}

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

void completeQuest() {
	currentQuest.complete = 1;
	completeAnimation = 1;
	return;
}

void acceptReward() {
	if (currentQuest.complete) {
		giveReward();
		getNextQuest();
	}
	return;
}

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

void showCurrentQuest() {
	int xOffset = 16, yOffset = 16;
	char str[128];
	char tmp[128];
	TTF_Font* font;
	font = TTF_OpenFont("assets/fonts/y.n.w.u.a.y.ttf", 16);
	SDL_Color color = { 255, 255, 255 };
	strcpy(str, "Current quest:");
	SDL_Surface* s = TTF_RenderText_Solid(font, str, color);
	SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
	int w, h;
	SDL_QueryTexture(t, NULL, NULL, &w, &h);
	aplikatuSurface(xOffset, yOffset, w, h, t, NULL);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_Rect r = { xOffset, h + yOffset, w, 2 };
	SDL_RenderFillRect(renderer, &r);
	float percentil = currentQuest.completion / (float)currentQuest.requiredAmmount;
	SDL_Rect percent = { xOffset, 2.5 * h + yOffset, percentil * w / 2, 8 };
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
	if(font != NULL) s = TTF_RenderText_Solid(font, tmp, color);
	t = SDL_CreateTextureFromSurface(renderer, s);
	int wBar = w / 2;
	SDL_QueryTexture(t, NULL, NULL, &w, &h);
	aplikatuSurface(xOffset + wBar + 8, 2.5 * h + yOffset + 8, w, h, t, NULL);

	SDL_FreeSurface(s);
	SDL_DestroyTexture(t);

	switch (currentQuest.action) {
	case HARVEST: strcpy(str, "Harvest: "); break;
	case PLANT: strcpy(str, "Plant: "); break;
	case WATER: strcpy(str, "Water: "); break;
	case ENTREGA: strcpy(str, "Deliver: "); break;
	case ARAR:strcpy(str, "Plow: "); break;
	case FERTILIZAR: strcpy(str, "Fertilize: "); break;
	default: strcpy(str, "Error");
	}

	if (currentQuest.requiredItem != 0) {
		if (currentQuest.action == PLANT || currentQuest.action == WATER || currentQuest.action == FERTILIZAR) {
			strcat(str, itemPresets[seedToItem(currentQuest.requiredItem)].name);
		}else if (currentQuest.requiredItem == 0) {
			strcat(str, "Edozein");
		}else{
			strcat(str, itemPresets[currentQuest.requiredItem].name);
		}
		strcat(str, " ");
	}
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
	return;
}

double animTime;
void questCompleteAnim(double deltaTime) {
	if (completeAnimation) {
		char* str = "QUEST COMPLETE!";
		TTF_Font* font;
		font = TTF_OpenFont("assets/fonts/y.n.w.u.a.y.ttf", 36);
		SDL_Color color = { 255, 255, 255 };
		SDL_Surface* s = TTF_RenderText_Solid(font, str, color);
		SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
		int w, h;
		SDL_QueryTexture(t, NULL, NULL, &w, &h);
		if (animTime < 1) {
			int clipW = animTime * w / 1;
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
void setQuestMenuState(int state) {
	questMenuOpen = state;
	return;
}

int getQuestMenuState() {
	return questMenuOpen;
}

void showQuestMenu() {

}

struct Quest generateRandomQuest() {
	struct Quest quest;
	int seedKopurua = 5;
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

	quest.rewardItem = 16;
	quest.rewardAmmount = rand() % maxRequired + 1;
	quest.complete = 0;
	quest.completion = 0;
	return quest;
}

struct Quest getCurrentQuest() {
	return currentQuest;
}