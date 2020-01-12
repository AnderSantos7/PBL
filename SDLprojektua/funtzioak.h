#ifndef FUNTZIOAK
#define FUNTZIOAK

//Player
struct Player createPlayer();
void movePlayer(double deltaTime);
void drawPlayer();
void animatePlayer(double deltaTime);
int checkInRange(int x, int y, int range);
void checkPosibleInteraction();

void initGame();
void reset();

//Camera
SDL_Rect createCamera();
SDL_Rect centerCameraInPlayer(SDL_Rect camera);
void aplikatuSurface(int x, int y, int w, int h, SDL_Texture* texture, SDL_Rect* clip);

//Input
int inputHandler(SDL_Event e);
int inputMainMenu(SDL_Event e);

//Media
void loadFiles();
void playPickUpSFX();
void playWellWaterSFX();

//Data
void startDic(int language);
void startPresests();
void playPickUpSFX();

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
void moveItemToHotbar(int slot);
struct Item pickHovering();
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

//Guardar partida
void load();
void save();

//Menu
void menu(double deltaTime);

//Energy
void marraztuEnergy();
void restaEnergy();

//Day Cycle
void updateDay(double deltaTime);
void drawDayFilter();
void drawClock();

//Quests
void checkQuestCompletion(int action, int item, int ammount);
void acceptReward();
void resetDay();
#endif