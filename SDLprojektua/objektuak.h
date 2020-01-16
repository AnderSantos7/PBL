#ifndef OBJEKTUAK
#define OBJEKTUAK

extern const int SCREEN_WIDTH, SCREEN_HEIGHT, TILE_SIZE, GRID_SIZE, LEVEL_SIZE;

extern SDL_Window* win;
extern SDL_Renderer* renderer;
SDL_Texture* textures[16];
enum textures { bgSurface, playerSurface, itemsSurface, plantsSurface, pauseSurface, HUDSurface, homeSurface, obstacleSurface, firmaSurface, menuSurface };

extern int main_menu, language;

struct Item {
	int ID;
	char* name;
	int sheetPosX;
	int sheetPosY;
	int quantity;
	int seed;
	int xPos;
	int yPos;
	int status;
};

extern struct Item itemPresets[];
extern struct Item droppedItems[];
extern int droppedLength;
extern struct Item hoveringItem;
extern int showingItem;

struct Inventory {
	int open;
	int xPos;
	int yPos;
	int rows;
	int cols;
	int slotSize;
	int sheetPosX;
	int sheetPosY;
	int headerSize;
	struct Item items[128];
};

extern struct Inventory inventories[];
enum INVENTORIES { INV_HOTBAR, INV_PLAYER, INV_CHEST, INV_SHOP };
enum LANGUAGES { EUS, ESP, ENG };
struct posCoord {
	int x;
	int y;
};

extern struct posCoord mousePos;

enum Seed { NONE, CALABAZA, TOMATE, PIMIENTO, BERENJENA, TRIGO };

struct Plant {
	int seed;
	int stage;
	double time;
	double lastWater;
	int water;
	int arado;
};

struct Tile {
	int ID;
	int x;
	int y;
	struct Plant plant;
};
struct Tile tiles[256];
extern const int camino_ID[];
extern const int plantable_ID[];

struct Player {
	int x;
	int y;
	int w;
	int h;
	int speed;
	int tile;
	int movingRight;
	int movingLeft;
	int movingUp;
	int movingDown;
	int facingDirection;
	int facingTile;
	int status;
	double timer;
	int hotbarSlot;
	int frame;
	int canInteract;
	int load;
	SDL_Rect clip;
	int energy;
	int sleeping;
};
extern struct Player player;

struct Obstacle {
	int interactuable;
	int x;
	int y;
	int w;
	int h;
	int sheetPosX;
	int sheetPosY;
};

struct Quest {
	int ID;
	int dialog_str;
	int action;
	int requiredItem;
	int requiredAmmount;
	int rewardItem;
	int rewardAmmount;
	int completion;
	int complete;
};

enum Actions { HARVEST, PLANT, WATER, ENTREGA, ARAR, FERTILIZAR };
extern char dic_Quests[128][128];

extern struct Obstacle obstaclesOutside[];
extern struct Obstacle obstaclesInside[];

extern SDL_Rect camera;
enum facingDirection { DIR_DOWN, DIR_UP, DIR_LEFT, DIR_RIGHT };
enum status { PLAYING, HOME, PAUSE, PAUSE_HOME, SLEEPING, COLLOCATING };
#endif