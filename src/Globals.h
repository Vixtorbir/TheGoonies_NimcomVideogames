#pragma once

//Displaying scale factor
#define GAME_SCALE_FACTOR	4.0f

//Function return values
enum class AppStatus { OK = 0, ERROR, QUIT };

//Game states
enum class GameState { STAR_MENU, MAIN_MENU, PLAYING, SETTINGS, CREDITS, YOU_WIN, YOU_LOSE };

//Main return values
#define EXIT_SUCCESS	0
#define EXIT_FAILURE	1

//Log messages
#define LOG(text, ...) log(__FILE__, __LINE__, text, __VA_ARGS__);
void log(const char file[], int line, const char* text, ...);

//Window size
#define WINDOW_WIDTH    272
#define WINDOW_HEIGHT   224

//Game constants
#define MARGIN_GUI_Y	0
#define TILE_SIZE		8
#define LEVEL_WIDTH		34
#define LEVEL_HEIGHT	28

//Entities animation delay
#define ANIM_DELAY		8