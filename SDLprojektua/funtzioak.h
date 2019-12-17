#ifndef FUNTZIOAK
#define FUNTZIOAK
//Player
struct Player createPlayer();
struct Player movePlayer(struct Player player);
void drawPlayer(SDL_Renderer* renderer, struct Player player);

//Camera
SDL_Rect createCamera();
void centerCameraInPlayer(SDL_Rect camera, struct Player player);


//Movement
int isMovingRight();
int isMovingLeft();
int isMovingUp();
int isMovingDown();

//Input
int inputHandler(SDL_Event e);
#endif