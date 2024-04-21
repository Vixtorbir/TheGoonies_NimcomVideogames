#pragma once
#include <raylib.h>
#include "Sprite.h"
#include "Point.h"
#include "AABB.h"
#include "Globals.h"
#include <unordered_map>

enum class Tile {

	// -1: area covered by an entity
	EMPTY = -1,
	//  0: air tile
	AIR = 0,

	// 0 < id < 50: static tiles
	BLOCK_WITHOUT_L = 1, BLOCK_WITHOUT_R, BLOCK_WITH_L_UP, BLOCK_WITH_R_UP, BLOCK_WITH_L_DOWN, BLOCK_WITH_R_DOWN,
	STONE_WITHOUT_1, STONE_WITHOUT_2, STONE_WITHOUT_3, STONE_WITHOUT_4, STONE_WITH_1_UP, STONE_WITH_3_UP,  STONE_WITH_4_UP, 
	STONE_WITH_2_UP,  STONE_WITH_5_UP,  STONE_WITH_6_UP, HALF_COLUMN_1_L,HALF_COLUMN_2_L, HALF_COLUMN_BOTTOM_L,
	
	COLUMN_TOP_L = 20, COLUMN_1_L, COLUMN_2_L, COLUMN_3_L, COLUMN_TOP_R, COLUMN_1_R, COLUMN_2_R, COLUMN_3_R,
	HALF_COLUMN_TOP_WITHOUT = 30, HALF_COLUMN_TOP, HALF_COLUMN_1, HALF_COLUMN_2, HALF_COLUMN_BOTTOM, HALF_COLUMN_BOTTOM_WITHOUT, TROPICAL_BLOCK_L, TROPICAL_BLOCK_R, 
	HALF_COLUMN_TOP_L, HALF_COLUMN_TOP_BOT,

	LADDER_TOP = 40, LADDER, LADDER_BOTTOM, STONE_WITH_1_DOWN, STONE_WITH_3_DOWN, STONE_WITH_4_DOWN, STONE_WITH_2_DOWN, STONE_WITH_5_DOWN, STONE_WITH_6_DOWN,
	
	WATER_L = 60, WATER_R,
	// 50 <= id < 100: special tiles

	KEY = 70, OBJECT,
	LASER = 80, LASER_FRAME0, LASER_FRAME1, LASER_FRAME2,

	// id >= 100: entities' initial locations
	PLAYER = 100, ENEMY,

	//Intervals
	STATIC_FIRST = BLOCK_WITHOUT_L,
	STATIC_LAST = STONE_WITH_6_DOWN,
	SOLID_FIRST = BLOCK_WITHOUT_L,
	SOLID_LAST = HALF_COLUMN_TOP_BOT,
	SPECIAL_FIRST = KEY,
	SPECIAL_LAST = LASER,
	ENTITY_FIRST = PLAYER,
	ENTITY_LAST = ENEMY
};

class TileMap
{
public:
	TileMap();
	~TileMap();

	AppStatus Initialise();
	AppStatus Load(int data[], int w, int h);
	void Update();
	void Render();
	void Release();

	//Test for collisions with walls
	bool TestCollisionWallLeft(const AABB& box) const;
	bool TestCollisionWallRight(const AABB& box) const;
	
	//Test collision with the ground and update 'py' with the maximum y-position to prevent
	//penetration of the grounded tile, that is, the pixel y-position above the grounded tile.
	//Grounded tile = solid tile (blocks) or ladder tops.
	bool TestCollisionGround(const AABB& box, int *py) const;
	
	//Test if there is a ground tile one pixel below the given box
	bool TestFalling(const AABB& box) const;
	
	//Test if box is on ladder and update 'px' with the x-center position of the ladder
	bool TestOnLadder(const AABB& box, int* px) const;
	
	//Test if box is on ladder top and update 'px' with the x-center position of the ladder
	bool TestOnLadderTop(const AABB& box, int* px) const;

private:
	void InitTileDictionary();

	Tile GetTileIndex(int x, int y) const;
	bool IsTileSolid(Tile tile) const;
	bool IsTileLadderTop(Tile tile) const;
	bool IsTileLadder(Tile tile) const;
	bool CollisionX(const Point& p, int distance) const;
	bool CollisionY(const Point& p, int distance) const;
	int GetLadderCenterPos(int pixel_x, int pixel_y) const;

	//Tile map
	Tile *map;

	//Size of the tile map
	int size, width, height;
	
	//Dictionary of tile frames
	std::unordered_map<int, Rectangle> dict_rect;

	Sprite *laser;
	
	//Tile sheet
	const Texture2D *img_tiles;
};

