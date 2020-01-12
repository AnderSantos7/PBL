#define _CRT_SECURE_NO_WARNINGS
#include "SDL.h"
#include "SDL_image.h"
#include "funtzioak.h"
#include "objektuak.h"

struct Quest currentQuest = { 0, 0, 0, 4, 1, 7, 1, 0};

void giveReward();

void giveReward() {
	if (!insertItem(0, itemPresets[currentQuest.rewardItem], currentQuest.rewardAmmount, -1)) {
		if (!insertItem(1, itemPresets[currentQuest.rewardItem], currentQuest.rewardAmmount, -1)) {
			dropItem(player.facingTile, currentQuest.rewardItem, currentQuest.rewardAmmount);
		}
	}
	return;
}

void getNextQuest() {
	return;
}

void checkQuestCompletion(int action, int item, int ammount) {
	if (action == currentQuest.action && item == currentQuest.requiredItem)
	{
		currentQuest.completion += ammount;
		if (currentQuest.completion >= currentQuest.requiredAmmount) {
			currentQuest.complete = 1;
		}
	}
	return;
}

void acceptReward() {
	giveReward();
	return;
}