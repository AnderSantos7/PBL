#ifndef OBSTACLES
#define OBSTACLES
struct Obstacle {
	int x;
	int y;
	int w;
	int h;
	int sheetPosX;
	int sheetPosY;
};

struct Obstacle obstacles[] = {
	{896, 864, 100, 40}, 
};
#endif