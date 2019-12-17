#ifndef OBJEKTUAK
#define OBJEKTUAK

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int TILE_SIZE;
extern const int GRID_SIZE;

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

struct Player {
	int x;
	int y;
	int w;
	int h;
	int speed;
};
extern struct Player player;
extern SDL_Rect camera;

#endif

