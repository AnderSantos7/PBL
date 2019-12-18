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

struct Tile {
	int ID;
	int x;
	int y;
	int farmable;
	int atxurra;
	int landatu;
};
struct Tile tiles[256];
extern const int camino_ID[];

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
};
extern struct Player player;
extern SDL_Rect camera;
enum facingDirection { DIR_RIGHT, DIR_LEFT, DIR_UP, DIR_DOWN };
#endif

