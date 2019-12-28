#ifndef FUNTZIOAK
#define FUNTZIOAK
//Player
struct Player createPlayer();
void movePlayer(double deltaTime);
void drawPlayer(SDL_Rect camera, SDL_Surface* surface, SDL_Surface* screenSurface);

void reset();

//Camera
SDL_Rect createCamera();
SDL_Rect centerCameraInPlayer(SDL_Rect camera);
void aplikatuSurface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip);

//Input
int inputHandler(SDL_Event e);

//Media
SDL_Surface* loadMedia(char* src);

//Tiles
int getTileFromPos(int x, int y);
struct posCoord getTilePosFromId(int ID);
int getFacingTileId();
void landatu();
void marraztuTiles(SDL_Surface* aradoSurface, SDL_Surface* screenSurface);
void updateTiles(double deltaTime);

//Inventory
void marraztuInv(SDL_Surface* spriteSheetSurface, SDL_Surface* screenSurface);
struct Inventory updateInv();
int checkHover();
void marraztuInvTag(SDL_Surface* textua, SDL_Surface* screenSurface);
void insertItem(struct Item item, int quantity, int pos);
struct Item removeItemFromInv(int pos);
struct Item pickHovering();
void changeInv(int InvPos);
void showStackSize(SDL_Surface* textua, SDL_Surface* screenSurface);

//Items
void marraztuHoveringItem(SDL_Surface* spriteSheetSurface, SDL_Surface* textua, SDL_Surface* screenSurface);
#endif