#ifndef OBJEKTUAK
#define OBJEKTUAK

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int TILE_SIZE;
extern const int GRID_SIZE;
extern const int LEVEL_SIZE;

extern SDL_Window* win;

struct posCoord {
	int x;
	int y;
};

struct Plant {
	int seed;
	int stage;
	double time;
	double lastWater;
	int water;
};

enum Seed { SEED_NONE, SEED_ARADO, SEED_TOMATO, SEED_PUMPKIN };

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