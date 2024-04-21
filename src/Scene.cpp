#include "Scene.h"
#include "Entity.h"
#include "Player.h"
#include "TileMap.h"
#include "Enemy.h"
#include <stdio.h>
#include "Globals.h"
#include "Game.h"

Scene::Scene()
{	player = nullptr;
    level = nullptr;
	enemy = nullptr;
	sfxItem = { 0 };
	
	camera.target = { 0, 0 };				//Center of the screen
	camera.offset = { 0, MARGIN_GUI_Y };	//Offset from the target (center of the screen)
	camera.rotation = 0.0f;					//No rotation
	camera.zoom = 1.0f;						//Default zoom

	debug = DebugMode::OFF;
}
Scene::~Scene()
{
	if (player != nullptr)
	{
		player->Release();
		delete player;
		player = nullptr;
	}
	if (enemy != nullptr)
	{
		enemy->Release();
		delete enemy;
		enemy = nullptr;
	}
    if (level != nullptr)
    {
		level->Release();
        delete level;
        level = nullptr;
    }
	for (Entity* obj : objects)
	{
		delete obj;
	}
	objects.clear();
	UnloadSound(sfxItem);
}
AppStatus Scene::Init()
{
	//Create player
	player = new Player({ 0,0 }, State::IDLE, Look::RIGHT);
	if (player == nullptr)
	{
		LOG("Failed to allocate memory for Player");
		return AppStatus::ERROR;
	}
	//Initialise player
	if (player->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Player");
		return AppStatus::ERROR;
	}
	//Create enemy 
	enemy = new Enemy({ 0,0 }, EnemyState::WALKING_LEFT);
	if (enemy == nullptr)
	{
		LOG("Failed to allocate memory for Enemy");
		return AppStatus::ERROR;
	}
	//Initialise enemy
	if (enemy->Initialize() != AppStatus::OK)
	{
		LOG("Failed to initialise Enemy");
		return AppStatus::ERROR;
	}
	//Create level 
    level = new TileMap();
    if (level == nullptr)
    {
        LOG("Failed to allocate memory for Level");
        return AppStatus::ERROR;
    }
	//Initialise level
	if (level->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}
	//Initialise player score
	player->InitScore();
	//Initialise player lives
	player->InitLives();
	//Load level
	if (LoadLevel(1) != AppStatus::OK)
	{
		LOG("Failed to load Level 1");
		return AppStatus::ERROR;
	}
	//Assign the tile map reference to the player to check collisions while navigating
	player->SetTileMap(level);
	enemy->SetTileMap(level);
	//Load sound effects
	sfxItem = LoadSound("sounds/key.wav");
	
    return AppStatus::OK;
}
AppStatus Scene::LoadLevel(int stage)
{
	int size;
	int x, y, i;
	Tile tile;
	Point pos;
	int *map = nullptr;
	Object *obj;
	
	ClearLevel();

	size = LEVEL_WIDTH * LEVEL_HEIGHT;
	
	if (stage == 1)
	{
		SetStage(1);
		map = new int[size] { 
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, //34
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			 0,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,	  2,   1,   2,   1,   2,   1,   2,   1,	  2,   1,   2,   1,   0,
			 0,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,	  1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   0,
			 0,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,	  2,   1,   2,   1,   2,   1,   2,   1,	  2,   1,   2,   1,   0,
			 0,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,	  1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   0,
			 0,  30,   7,   8,   9,  10,   7,   8,   9,  10,   7,   8,   9,  10,   7,   8,   9,  10,   7,   8,   9,  10,   7,   8,  20,  24,   7,   8,   9,  10,   7,   8,   9,   0,
			 0,  32,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  21,  25,   0,   0,   0,   0,   0,   0,   0,   0,
			 0,  33,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  22,  26,   0,   0,   0,	  0,   0,   0,   0,   0,
			 0,  32,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  23,  27,   0,   0,   0,   0,   0,   0,   0,   0,
			 0,  33,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,
			 0,  32,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			 0,  33,   0,   0,   0,   0,   0,  70,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,
			 0,  32,   0,   0,   0,   0,   0,   0,   0,   0,   0, 100,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			 0,  33,   0,   0,  14,  11,  40,  14,  15,  16,  14,  11,  12,  13,   14, 11,  12,  13,  40,  14,  11,	  0,   0,   0,   0,   0,   0,   0,  11,	 14,  40,  14,  31,   0,
			 0,  32,   0,   0,  46,  43,  41,  46,  47,  48,  46,  43,  44,  45,   46, 43,  44,  45,  41,  46,  43,	  0,   0,   0,   0,   0,   0,   0,  43,  46,  41,  46,  39,   0,
			 0,  33,   0,   0,   0,   0,  41,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  41,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,	  0,  41,   0,  17,   0,
			 0,  32,   0,   0,   0,   0,  41,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  41,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,  41,   0,  18,   0,
			 0,  33,   0,   0,   0,   0,  41,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  41,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,	  0,  41,   0,  18,   0,
			 0,  34,   0,   0,   0,   0,  41,   0,   0,  71,   0,   101,   0,   0,   0,   0,   0,   0,  41,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,  41,   0,  19,   0,
			 0,   3,   3,   3,   3,   3,  42,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,  42,   3,   3,	  3,   3,   3,   3,   3,   3,   3,   3,   3,  42,   3,   3,   0, 
			 0,   6,   2,   1,   5,   1,   2,   5,   5,   6,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,	  1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   0, 
			 0,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,	  2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   0,  
			 0,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,	  1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   0, 
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
             //28
		};
		
	}
	else if (stage == 2)
	{
		SetStage(2);
		
		map = new int[size] {
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, //34
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			 0,  30,   7,   8,   9,  10,   7,   8,   9,  10,   7,   8,   9,  10,   7,   8,   9,  10,   7,   8,   9,  10,   7,   8,  20,  24,   7,   8,   9,  10,   7,   8,   9,   0,
			 0,  32,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   
			 0,  32,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   
			 0,  33,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,  14,  11,  40,  12,  13,  11,  14,  12,  13,  11,  14,  12,  13,  11,  14,  12,  13,  11,  14,  12,  13,  13,  11,  14,  12,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,  45,  43,  41,  44,  45,  43,  46,  44,  45,  43,  46,  44,  45,  43,  46,  44,  45,  43,  46,  44,  45,	 45,  43,  46,  44,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  41,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,  70,   0,   0,  41,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  41,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			 0,  31,  11,  14,  12,  13,  11,  14,  12,  13,  42,  14,  36,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,  36,  14,  11,  12,   0,
			 0,  38,  43,  46,  44,  45,  43,  46,  44,  45,  43,  46,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  46,  43,  44,   0,
			 0,  32,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,
			 0,  32,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			 0,  33,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,
			 0,  33,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			 0,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,   0, 
			 0,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,   0, 
			 0,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,   0,  
			 0,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,  60,  61,   0, 
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
		};
		
	}
	else if (stage == 3)
	{
		SetStage(3);
		map = new int[size] {
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, //34
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			 0,  10,   7,   8,   9,  10,   7,   8,   9,  10,   7,   8,   9,  10,   7,   8,   9,  10,   7,   8,   9,  10,   7,   8,  20,  24,   7,   8,   9,  10,   7,   8,   9,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  13,  11,  14,  12,  13,  40,  14,  12,  13,  11,  14,  12,  13,  40,  14,  12,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  45,  43,  46,  44,  45,  41,  46,  44,  45,  43,  46,  44,  45,  41,  46,  44,   0,
			 0,  13,  11,  14,  12,  13,  11,  14,  12,  13,  14,  40,  14,  11,   0,   0,   0,   0,   0,   0,   0,   0,  41,   0,   0,   0,   0,   0,   0,   0,  41,   0,   0,   0,
			 0,  45,  43,  46,  44,  45,  43,  46,  44,  45,  46,  41,  46,  43,   0,   0,   0,   0,   0,   0,   0,   0,  41,   0,   0,   0,   0,   0,   0,   0,  41,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  41,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  41,   0,   0,   0,   0,   0,   0,   0,  41,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,  70,   0,   0,   0,  41,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,  41,   0,   0,   0,   0,   0,   0,	  0,  41,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  41,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,  41,   0,   0,   0,   0,   0,   0,   0,  41,   0,   0,   0,
			 0,  31,  11,  14,  12,  13,  11,  14,  12,  13,  14,  42,  12,  13,  40,  11,  13,   0,   0,   0,  11,  14,  42,  14,  12,  13,  14,  40,  12,  13,  42,  14,  11,   0,
			 0,  38,  43,  46,  44,  45,  43,  46,  44,  45,  46,  43,  44,  45,  41,  43,  45,   0,   0,   0,  43,  46,  43,  46,  44,  45,  46,  41,  44,  45,  46,  46,  43,   0,
			 0,  32,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  41,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,  41,   0,	  0,   0,   0,   0,   0,
			 0,  32,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  41,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,  41,   0,   0,   0,   0,   0,   0,
			 0,  33,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  41,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,  41,   0,	  0,   0,   0,   0,   0,
			 0,  34,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  41,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,  41,   0,   0,   0,   0,   0,   0,
			 0,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,  42,   3,   3,   3,   3,   3,   3,	  3,   3,   3,   3,   3,   3,  42,   3,   3,   3,   3,   3,   0, 
			 0,   6,   2,   1,   5,   1,   2,   5,   5,   6,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,	  1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   0, 
			 0,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,	  2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   0,  
			 0,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,	  1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   1,   2,   0, 
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
		};
		
	}
	else
	{
		//Error level doesn't exist or incorrect level number
		LOG("Failed to load level, stage %d doesn't exist", stage);
		return AppStatus::ERROR;	
	}

	//Entities and objects
	i = 0;
	for (y = 0; y < LEVEL_HEIGHT; ++y)
	{
		for (x = 0; x < LEVEL_WIDTH; ++x)
		{
			tile = (Tile)map[i];
			if (tile == Tile::EMPTY)
			{
				map[i] = 0;
			}
			else if (tile == Tile::PLAYER)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				player->SetPos(pos);
				map[i] = 0;
			}
			else if (tile == Tile::ENEMY)
{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				enemy->SetPos(pos);
				map[i] = 0;
			}
			else if (tile == Tile::KEY)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::KEY);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::OBJECT)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::OBJECT);
				objects.push_back(obj);
				map[i] = 0;
			}
			++i;
		}
	}
	//Tile map
	level->Load(map, LEVEL_WIDTH, LEVEL_HEIGHT);

	return AppStatus::OK;
}
void Scene::Update()
{
	Point pos;
	Point p1, p2;
	AABB box;

	//Switch between the different debug modes: off, on (sprites & hitboxes), on (hitboxes)  make appear position of the player too
	if (IsKeyPressed(KEY_F1))
	{
		debug = (DebugMode)(((int)debug + 1) % (int)DebugMode::SIZE);

	}
	//Debug levels instantly
	if (IsKeyPressed(KEY_ONE))
	{
		LoadLevel(1);
	}
	else if (IsKeyPressed(KEY_TWO))
	{
		LoadLevel(2);
	}
	else if (IsKeyPressed(KEY_THREE))
	{
		LoadLevel(3);
	}
	else if (IsKeyPressed(KEY_F2))
	{
		youWin=true;
	}
	else if (IsKeyPressed(KEY_F3))
	{
		youLose=true;
	}
	else if (player->NextLevel() && currentStage == 1)
	{
		LoadLevel(2);
		
		player->SetPos({ 10, player->GetY()});
	}
	else if (player->NextLevel() && currentStage == 2)
	{
		LoadLevel(3);
		player->SetPos({ 10, player->GetY() });
	}
	else if (player->NextLevel() && currentStage == 3)
	{
		youWin = true;
	}
	else if (player->PrevLevel() && currentStage == 2)
	{
		LoadLevel(1);
		player->SetPos({ 250, player->GetY() });
	}
	else if (player->PrevLevel() && currentStage == 3)
	{
		LoadLevel(2);
		player->SetPos({ 250, player->GetY() });
	}
	level->Update();
	player->Update();
	enemy->Update();
	CheckCollisions();
}
void Scene::SetStage(int stage)
{
	this->currentStage = stage;
}
void Scene::Render()
{
	BeginMode2D(camera);

    level->Render();
	enemy->Draw();
	if (debug == DebugMode::OFF || debug == DebugMode::SPRITES_AND_HITBOXES)
	{
		RenderObjects(); 
		player->Draw();
	}
	if (debug == DebugMode::SPRITES_AND_HITBOXES || debug == DebugMode::ONLY_HITBOXES)
	{
		RenderObjectsDebug(YELLOW);
		player->DrawDebug(GREEN);
		enemy->DrawDebug(RED);
	}

	EndMode2D();

	RenderGUI();
}
void Scene::Release()
{
    level->Release();
	player->Release();
	enemy->Release();
	ClearLevel();
}
			
void Scene::CheckCollisions()
{
	AABB player_box, obj_box;
	
	player_box = player->GetHitbox();
	auto it = objects.begin();
	while (it != objects.end())
	{
		obj_box = (*it)->GetHitbox();
		if(player_box.TestAABB(obj_box))
		{
			PlaySound(sfxItem);
			player->IncrScore((*it)->Points());
			
			//Delete the object
			delete* it; 
			//Erase the object from the vector and get the iterator to the next valid element
			it = objects.erase(it); 
		}
		else
		{
			//Move to the next object
			++it; 
		}
	}
	AABB enemy_box;

	enemy_box = enemy->GetHitbox();
	//Check collision between player and enemy
	if (player_box.TestAABB(enemy_box))
	{
		
		youLose = true;
	}
}

void Scene::ClearLevel()
{
	for (Object* obj : objects)
	{
		delete obj;
	}
	objects.clear();
}
void Scene::RenderObjects() const
{
	for (Object* obj : objects)
	{
		obj->Draw();
	}
}
void Scene::RenderEnemies() const
{
	enemy->Draw();
}
void Scene::RenderObjectsDebug(const Color& col) const
{
	for (Object* obj : objects)
	{
		obj->DrawDebug(col);
	}
}
void Scene::RenderGUI() const
{
	//Temporal approach
	DrawText(TextFormat("SCORE : %d", player->GetScore()), 8, 5, 8, WHITE);
	DrawText(TextFormat("LIVES : %d", player->GetLives()), 8, 20, 8, WHITE);
	DrawText(TextFormat("STAGE : %d", currentStage), 8, 35, 8, WHITE);
}