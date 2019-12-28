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
void marraztuInv(int inv, SDL_Surface* spriteSheetSurface, SDL_Surface* screenSurface);
void updateInv(int inv);
int checkHover(int inv);
void marraztuInvTag(int inv, SDL_Surface* textua, SDL_Surface* screenSurface);
void insertItem(int inv, struct Item item, int quantity, int pos);
struct Item removeItemFromInv(int inv, int pos);
struct Item pickHovering();
void changeInv(int inv, int InvPos);
void showStackSize(int inv, SDL_Surface* textua, SDL_Surface* screenSurface);
void showInv(int inv, SDL_Surface* itemsSurface, SDL_Surface* screenSurface, SDL_Surface* textua);
int getHoveringInv();
void closeInvs();

//Items
void marraztuHoveringItem(int inv, SDL_Surface* spriteSheetSurface, SDL_Surface* textua, SDL_Surface* screenSurface);
#endif