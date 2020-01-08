#ifndef FUNTZIOAK
#define FUNTZIOAK
//Player
struct Player createPlayer();
void movePlayer(double deltaTime);
void drawPlayer();
void animatePlayer(double deltaTime);
void checkPosibleInteraction();

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
void harvest(int tile);
void water(int tile);
void marraztuTiles();
void updateTiles(double deltaTime);

//Inventory
void marraztuInv(int inv);
void updateInv(int inv);
int checkHover();
void marraztuInvTag(int inv);
int insertItem(int inv, struct Item item, int quantity, int pos);
struct Item removeItemFromInv(int inv, int pos);
struct Item pickHovering();
void changeInv(int inv, int InvPos);
void showStackSize(int inv);
void showInv(int inv);
int getHoveringInv();
void marraztuHoveringItem();
void closeInvs();

//Items
void ordenatuDroppedItems();
void marraztuDroppedItems(int ordena);
void showDroppedItem(int index);
void dropHoveringItem();
void dropItem(int tile, int item, int ammount);
int pickUpItems();
void sweepFloor(int i);
#endif