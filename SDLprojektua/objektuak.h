#ifndef OBJEKTUAK
#define OBJEKTUAK

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int TILE_SIZE;
extern const int GRID_SIZE;
extern const int LEVEL_SIZE;

extern SDL_Window* win;
extern SDL_Surface* itemsSurface;
extern SDL_Surface* screenSurface;

struct Item {
	int ID;
	char* name;
	int sheetPosX;
	int sheetPosY;
	int quantity;
	int xPos;
	int yPos;
};

struct Item droppedItems[128];
extern struct Item hoveringItem;
extern int showingItem;

struct Inventory {
	int open;
	int xPos;
	int yPos;
	int rows;
	int cols;
	int slotSize;
	struct Item items[128];
};

extern struct Inventory inventories[];
enum INVENTORIES {INV_HOTBAR, INV_PLAYER, INV_CHEST};
struct posCoord {
	int x;
	int y;
};

extern struct posCoord mousePos;

enum Seed {NONE, CALABAZA, TOMATE};

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
	int farmable;
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
};
extern struct Player player;

extern SDL_Rect camera;
enum facingDirection { DIR_RIGHT, DIR_LEFT, DIR_UP, DIR_DOWN };
enum status { PLAYING, HOME, PAUSE, PAUSE_HOME, SLEEPING, COLLOCATING};
extern SDL_Surface* homeSurface;
extern SDL_Surface* playerSurface;
#endif