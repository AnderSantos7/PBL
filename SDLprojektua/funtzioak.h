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
int inputMainMenu(SDL_Event e);

//Media
SDL_Surface* loadMedia(char* src);

//Data
void startDic(int language);
void startPresests();

//Tiles
int getTileFromPos(int x, int y);
struct posCoord getTilePosFromId(int ID);
int getFacingTileId();
void landatu(int seed);
void water(int tile);
void marraztuTiles(SDL_Surface* aradoSurface, SDL_Surface* screenSurface);
void updateTiles(double deltaTime);

//Inventory
void marraztuInv(int inv, SDL_Surface* spriteSheetSurface, SDL_Surface* screenSurface);
void updateInv(int inv);
int checkHover();
void marraztuInvTag(int inv, SDL_Surface* textua, SDL_Surface* screenSurface);
int insertItem(int inv, struct Item item, int quantity, int pos);
struct Item removeItemFromInv(int inv, int pos);
struct Item pickHovering();
void changeInv(int inv, int InvPos);
void showStackSize(int inv, SDL_Surface* textua, SDL_Surface* screenSurface);
void showInv(int inv, SDL_Surface* itemsSurface, SDL_Surface* screenSurface, SDL_Surface* textua, SDL_Surface* HUDSurface);
int getHoveringInv();
void marraztuHoveringItem(SDL_Surface* spriteSheetSurface, SDL_Surface* textua, SDL_Surface* screenSurface);
void closeInvs();

//Items
void ordenatuDroppedItems();
void marraztuDroppedItems(int ordena);
void showDroppedItem(int index);
void dropItem();
void pickUpItems();
int checkInRange(int i);
void sweepFloor(int i);
#endif