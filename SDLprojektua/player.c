#include "SDL.h"
#include "SDL_image.h"
#include "funtzioak.h"
#include "objektuak.h"
#include <stdio.h>

//Jokalari struct-a hasieratu
struct Player createPlayer() {
	struct Player player;
	player.x = 0;
	player.y = 0;
	player.w = TILE_SIZE;
	player.h = 2 * TILE_SIZE;
	player.speed = 300;
	player.movingRight = 0;
	player.movingLeft = 0;
	player.movingUp = 0;
	player.movingDown = 0;
	player.facingDirection = DIR_DOWN;
	player.status = PLAYING;
	player.timer = 0;
	player.hotbarSlot = 0;
	player.frame = 0;
	player.clip.w = 64;
	player.clip.h = 128;
	player.clip.x = 64;
	player.clip.y = 0;
	player.canInteract = 0;
	player.energy = 200;
	player.sleeping = 0;
	return player;
}

//Jokalaria mugitzeko funtzioa. Mugimenduaren ondoren hitboxak aztertzen dira, eta beharrezkoa izanez gero, posizoa aldatzen da.
void movePlayer(double deltaTime) {
	float multiplier = 1;
	int bidean = 0, i = 0;
	if (player.sleeping == 0)
	{
		player.tile = getTileFromPos(player.x + player.w / 2, player.y + 2 * TILE_SIZE);
		//Etxetik kanpoko mugimendua
		if (player.status == PLAYING) {
			//Bide motako tile batean dagoen begiratu. Hala bada, jokalariaren abiadura * 1.2 izango da.
			while (!bidean && i < 33) {
				if (tiles[camino_ID[i]].x * TILE_SIZE < player.x + player.w - 20 &&
					tiles[camino_ID[i]].x * TILE_SIZE + TILE_SIZE > player.x + 20 &&
					tiles[camino_ID[i]].y * TILE_SIZE < player.y + 2 * TILE_SIZE &&
					tiles[camino_ID[i]].y * TILE_SIZE + TILE_SIZE > player.y + 2 * TILE_SIZE)
				{
					bidean = 1;
					multiplier = 1.2;
				}
				i++;
			}

			if (player.movingRight) {
				player.x += (int)(player.speed * multiplier * deltaTime);
				if (player.x > LEVEL_SIZE - player.w) {
					player.x = LEVEL_SIZE - player.w;
				}
				if (player.y < 70 && player.x > 440 && player.x < 580)
				{
					player.x = 440;
				}
			}
			if (player.movingLeft) {
				player.x -= (int)(player.speed * multiplier * deltaTime);
				if (player.x < 0) {
					player.x = 0;
				}

				if (player.y > 6 * 64 - 8 && player.x < 3 * 64 + 32) {
					player.x = 3 * 64 + 32;
				}
				else if (player.y < 64 && player.x < 4 * 64) {
					player.x = 4 * 64;
				}
				if (player.y < 70 && player.x > 440 && player.x < 580)
				{
					player.x =580;
				}
			}
			if (player.movingUp) {
				player.y -= (int)(player.speed * multiplier * deltaTime);
				if (player.y < 0) {
					player.y = 0;
				}
				if (player.y < 64 && player.y  > 64 - 13 && player.x > 12 * 64 && player.x < 14 * 64) {
					player.y = 64;
				}
				else if (player.y < 64 && player.x < 4 * 64) {
					player.y = 64;
					if (player.x + 32 > 64 * 2 && player.x + 32 < 64 * 3) {
						player.status = HOME;
						inventories[INV_HOTBAR].yPos = 3;
						inventories[INV_PLAYER].yPos = 90;
						closeInvs();
						player.y = 480 - 64 * 2;
						player.x = 375;
					}
				}
				if (player.y < 70 && player.x > 440 && player.x < 580)
				{
					player.y = 70;
				}
			}
			if (player.movingDown) {
				player.y += (int)(player.speed * multiplier * deltaTime);
				if (player.y > LEVEL_SIZE - player.h) {
					player.y = LEVEL_SIZE - player.h;
				}
				if (player.y > 6 * 64 - 8 && player.x < 3 * 64 + 32 && player.x >= 0) {
					player.y = 6 * 64 - 8;
				}
				else if (player.y < 64 && player.y > 64 - 13 && player.x > 12 * 64 && player.x < 14 * 64) {
					player.y = 64 - 13;
				}
			}
		}
		//Etxe barruko mugimendua
		else if (player.status == HOME) {
			if (player.movingRight) {
				player.x += (int)(player.speed * deltaTime * multiplier);
				if (player.y < 70 && player.x > 340 && player.x < 350)
				{
					player.x = 340;
				}
				if (player.x > 640 - 64) {
					player.x = 640 - 64;
				}
				else if (player.x > 449 - 64 && player.y > 445 - 2 * 64) {
					player.x = 449 - 64;
				}

			}

			if (player.movingLeft) {
				player.x -= (int)(player.speed * deltaTime * multiplier);
				if (player.y < 70 && player.x < 440 && player.x >430)
				{
					player.x = 440;
				}
				else if (player.y < 150 && player.x < 100) {
					player.x = 100;
				}
				else if (player.x < 0) {
					player.x = 0;
				}
				else if (player.x < 352 && player.y > 445 - 2 * 64) {
					player.x = 352;
				}
			}

			if (player.movingUp) {
				player.y -= (int)(player.speed * deltaTime * multiplier);
				if (player.y < 70 && player.x > 340 && player.x < 440)
				{
					player.y = 70;
				}
				else if (player.y < 150 && player.x < 100) {
					player.y = 150;
				}
				else if (player.x >= 100 && player.x <= 640 - 64 && player.y < 0) {
					player.y = 0;
				}
			}

			if (player.movingDown) {
				player.y += (int)(player.speed * deltaTime * multiplier);

				if (player.y > 445 - 64 * 2 && player.x > 352 - 20 && player.x < 449 - 44) {
					if (player.y > 480 - 64 * 2) {
						player.timer += deltaTime;
						closeInvs();
					}
				}
				else if (player.y > 445 - 64 * 2) {
					player.y = 445 - 64 * 2;
				}
			}
		}
	}
	return;
}

double animationTime = 0;
//Baserritarraren ibiltzeko animaziorako funtzioa. Denboraren arabera aldatzen da frame-a.
//Jokalariaren spritesheetean lerro bakoitzeko noraznko bat. Goitik behera: Beherantz, gorantz, eskuin eta ezker.
//Noranzko bakoitzak 3 frame ditu eta erdikoa birritan errepikatzen da.
void animatePlayer(double deltaTime) {
	if (player.sleeping == 0)
	{
		if (player.movingUp || player.movingDown || player.movingRight || player.movingLeft) {
			animationTime += deltaTime;
			if (animationTime >= 0.1)
			{
				player.frame++;
				if (player.frame > 3)
					player.frame = 0;
				animationTime = 0;
			}
			if (player.frame == 3)
				player.clip.x = 64;
			else
			{
				player.clip.x = 64 * player.frame;
			}
			player.clip.y = player.facingDirection * 128;
		}
		else if (player.clip.x != 64) {
			player.clip.x = 64;
			animationTime = 0;
		}
	}
	return;
}

//Jokalaria bere posizoan marrazteko funtzioa. Gainera, interakzioa posible bada, 'Q' markagailua ere marrazten da.
void drawPlayer() {
	aplikatuSurface(player.x - camera.x, player.y - camera.y, player.w, player.h, textures[playerSurface], &player.clip);
	if (player.canInteract != -1) {
		if (player.status == PLAYING && obstaclesOutside[player.canInteract].interactuable) {
			SDL_Rect clip;
			clip.x = 582;
			clip.y = 208;
			clip.w = 30;
			clip.h = 30;
			aplikatuSurface(player.x - camera.x + player.w, player.y - camera.y, 30, 30, textures[HUDSurface], &clip);
		}
		if (player.status == HOME && obstaclesInside[player.canInteract].interactuable) {
			SDL_Rect clip;
			clip.x = 582;
			clip.y = 208;
			clip.w = 30;
			clip.h = 30;
			aplikatuSurface(player.x - camera.x + player.w, player.y - camera.y, 30, 30, textures[HUDSurface], &clip);
		}
	}
	return;
}

//Puntu bat zirkunferentzia baten barruan dagoen kalkulatzeko funtzioa. Interakzio-eremuak eta lurreko item-ak hartzeko eremua kalkulatzeko.
int checkInRange(int x, int y, int range) {
	double p = (pow(((double)x), 2) / pow(range, 2)) + (pow((double)y, 2) / pow(range, 2));
	return (p <= 1);
}

//Jokalaria interakzio eremu batean dagoen ala ez begiratzeko.
void checkPosibleInteraction() {
	int i = 0, range = 150;
	player.canInteract = -1;
	switch(player.status) {
	case PLAYING:
		while (player.canInteract == -1 && i < 5) {
			if (checkInRange(obstaclesOutside[i].x + obstaclesOutside[i].w / 2 - player.x - player.w / 2, obstaclesOutside[i].y + obstaclesOutside[i].h / 2 - player.y - player.h, range)) {
				player.canInteract = i;
			}
			i++;
		}
		if (player.canInteract != 3 && inventories[INV_SHOP].open) {
			inventories[INV_SHOP].open = 0;
			inventories[INV_PLAYER].open = 0;
		}
		if (player.canInteract != 2 && getQuestMenuState()) setQuestMenuState(0);
	case HOME:
		while (player.canInteract  == -1 && i < 2) {
			if (checkInRange(obstaclesInside[i].x + obstaclesInside[i].w / 2 - player.x, obstaclesInside[i].y + obstaclesInside[i].h / 2 - player.y, range)) {
				player.canInteract = i;
			}
			i++;
		}
		if (player.canInteract == -1 && inventories[INV_CHEST].open) inventories[INV_CHEST].open = 0;
	}

	return;
}