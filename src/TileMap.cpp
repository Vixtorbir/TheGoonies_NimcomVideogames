#include "TileMap.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <cstring>

TileMap::TileMap()
{
	map = nullptr;
	width = 0;
	height = 0;
	laser = nullptr;
	img_tiles = nullptr;

	InitTileDictionary();
}
TileMap::~TileMap()
{
	if (map != nullptr)
	{
		delete[] map;
		map = nullptr;
	}
	if (laser != nullptr)
	{
		laser->Release();
		delete laser;
		laser = nullptr;
	}
}
void TileMap::InitTileDictionary()
{
	const int n = TILE_SIZE;

	dict_rect[(int)Tile::BLOCK_WITHOUT_L] = { 0,  0, n, n };
	dict_rect[(int)Tile::BLOCK_WITHOUT_R] = { 1 * n ,0, n, n };
	dict_rect[(int)Tile::BLOCK_WITH_L_UP] = { 4 * n, 4 * n, n, n };
	dict_rect[(int)Tile::BLOCK_WITH_R_UP] = { 3 * n, 4 * n, n, n };
	dict_rect[(int)Tile::BLOCK_WITH_L_DOWN] = { 5 * n, 5 * n, n, n };
	dict_rect[(int)Tile::BLOCK_WITH_R_DOWN] = { 4 * n, 5 * n, n, n };
	dict_rect[(int)Tile::STONE_WITHOUT_1] = { 2 * n,  1 * n, n, n };
	dict_rect[(int)Tile::STONE_WITHOUT_2] = { 0,  2 * n, n, n };
	dict_rect[(int)Tile::STONE_WITHOUT_3] = { 1 * n, 2 * n, n, n };
	dict_rect[(int)Tile::STONE_WITHOUT_4] = { 1 * n, 2 * n, -n, n };
	dict_rect[(int)Tile::STONE_WITH_1_UP] = { 2 * n, 4 * n, n, n };
	dict_rect[(int)Tile::STONE_WITH_1_DOWN] = { 2 * n, 5 * n, n, n };
	dict_rect[(int)Tile::STONE_WITH_3_UP] = { 1 * n ,  4 * n, n, n };
	dict_rect[(int)Tile::STONE_WITH_3_DOWN] = { 1 * n, 5 * n, n, n };
	dict_rect[(int)Tile::STONE_WITH_4_UP] = { 1 * n, 4 * n, -n, n };
	dict_rect[(int)Tile::STONE_WITH_4_DOWN] = { 1 * n, 5 * n, -n, n };
	dict_rect[(int)Tile::STONE_WITH_2_UP] = { 0, 4 * n, n, n };
	dict_rect[(int)Tile::STONE_WITH_2_DOWN] = { 0, 5 * n, n, n };
	dict_rect[(int)Tile::STONE_WITH_5_UP] = { 9 * n, 0, n, n };
	dict_rect[(int)Tile::STONE_WITH_5_DOWN] = { 9 * n, 1 * n, n, n };
	dict_rect[(int)Tile::STONE_WITH_6_UP] = { 10 * n, 0, n, n };
	dict_rect[(int)Tile::STONE_WITH_6_DOWN] = { 10 * n, 1 * n, n, n };
	dict_rect[(int)Tile::TROPICAL_BLOCK_L] = { 2 * n, 2 * n, n, n };
	dict_rect[(int)Tile::TROPICAL_BLOCK_R] = { 3 * n, 2 * n, n, n };


	dict_rect[(int)Tile::LADDER_BOTTOM] = { 4 * n, 2 * n, n, n };
	dict_rect[(int)Tile::LADDER] = { 4 * n, 1 * n, n, n };
	dict_rect[(int)Tile::LADDER_TOP] = { 4 * n, 0, n, n };
	dict_rect[(int)Tile::COLUMN_TOP_L] = { 7 * n, 0, n, n };
	dict_rect[(int)Tile::COLUMN_1_L] = { 7 * n, 1 * n, n, n };
	dict_rect[(int)Tile::COLUMN_2_L] = { 7 * n, 2 * n, n, n };
	dict_rect[(int)Tile::COLUMN_3_L] = { 7 * n, 3 * n, n, n };
	dict_rect[(int)Tile::COLUMN_TOP_R] = { 8 * n, 0, n, n };
	dict_rect[(int)Tile::COLUMN_1_R] = { 8 * n, 1 * n, n, n };
	dict_rect[(int)Tile::COLUMN_2_R] = { 8 * n, 2 * n, n, n };
	dict_rect[(int)Tile::COLUMN_3_R] = { 8 * n, 3 * n, n, n };
	dict_rect[(int)Tile::HALF_COLUMN_TOP_WITHOUT] = { 5 * n, 0, n, n };
	dict_rect[(int)Tile::HALF_COLUMN_TOP] = { 6 * n, 0, n, n };
	dict_rect[(int)Tile::HALF_COLUMN_TOP_L] = { 6 * n, 1 * n, n, n };
	dict_rect[(int)Tile::HALF_COLUMN_TOP_BOT] = { 6 * n, 1 * n, -n, n };
	dict_rect[(int)Tile::HALF_COLUMN_1] = { 5 * n, 1 * n, n, n };
	dict_rect[(int)Tile::HALF_COLUMN_1_L] = { 5 * n, 1 * n, -n, n };
	dict_rect[(int)Tile::HALF_COLUMN_2] = { 5 * n, 2 * n, n, n };
	dict_rect[(int)Tile::HALF_COLUMN_2_L] = { 5 * n, 2 * n, -n, n };
	dict_rect[(int)Tile::HALF_COLUMN_BOTTOM] = { 6 * n, 3 * n, n, n };
	dict_rect[(int)Tile::HALF_COLUMN_BOTTOM_L] = { 6 * n, 3 * n, -n, n };
	dict_rect[(int)Tile::HALF_COLUMN_BOTTOM_WITHOUT] = { 5 * n, 3 * n, n, n };

	dict_rect[(int)Tile::WATER_L] = { 0, 3 * n, n, n };
	dict_rect[(int)Tile::WATER_R] = { 1 * n, 3 * n, n, n };



}
AppStatus TileMap::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();

	if (data.LoadTexture(Resource::IMG_TILES, "images/tileset.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_tiles = data.GetTexture(Resource::IMG_TILES);

	laser = new Sprite(img_tiles);
	if (laser == nullptr)
	{
		LOG("Failed to allocate memory for laser sprite");
		return AppStatus::ERROR;
	}
	laser->SetNumberAnimations(1);
	laser->SetAnimationDelay(0, ANIM_DELAY);
	laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME0]);
	laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME1]);
	laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME2]);
	laser->SetAnimation(0);

	return AppStatus::OK;
}
AppStatus TileMap::Load(int data[], int w, int h)
{
	size = w * h;
	width = w;
	height = h;

	if (map != nullptr)	delete[] map;

	map = new Tile[size];
	if (map == nullptr)
	{
		LOG("Failed to allocate memory for tile map");
		return AppStatus::ERROR;
	}
	memcpy(map, data, size * sizeof(int));

	return AppStatus::OK;
}
void TileMap::Update()
{
	laser->Update();
}
Tile TileMap::GetTileIndex(int x, int y) const
{
	int idx = x + y * width;
	if (idx < 0 || idx >= size)
	{
		LOG("Error: Index out of bounds. Tile map dimensions: %dx%d. Given index: (%d, %d)", width, height, x, y)
			return Tile::AIR;
	}
	return map[x + y * width];
}
bool TileMap::IsTileSolid(Tile tile) const
{
	return (Tile::SOLID_FIRST <= tile && tile <= Tile::SOLID_LAST);
}
bool TileMap::IsTileLadderTop(Tile tile) const
{
	return tile == Tile::LADDER_TOP;
}
bool TileMap::IsTileLadder(Tile tile) const
{
	return tile == Tile::LADDER;
}
bool TileMap::TestCollisionWallLeft(const AABB& box) const
{
	return CollisionX(box.pos, box.height);
}
bool TileMap::TestCollisionWallRight(const AABB& box) const
{
	return CollisionX(box.pos + Point(box.width - 1, 0), box.height);
}
bool TileMap::TestCollisionGround(const AABB& box, int* py) const
{
	Point p(box.pos.x, *py);	//control point
	int tile_y;

	if (CollisionY(p, box.width))
	{
		tile_y = p.y / TILE_SIZE;

		*py = tile_y * TILE_SIZE - 1;
		return true;
	}
	return false;
}
bool TileMap::TestFalling(const AABB& box) const
{
	return !CollisionY(box.pos + Point(0, box.height), box.width);
}
bool TileMap::CollisionX(const Point& p, int distance) const
{
	int x, y, y0, y1;

	//Calculate the tile coordinates and the range of tiles to check for collision
	x = p.x / TILE_SIZE;
	y0 = p.y / TILE_SIZE;
	y1 = (p.y + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the vertical range
	for (y = y0; y <= y1; ++y)
	{
		//One solid tile is sufficient
		if (IsTileSolid(GetTileIndex(x, y)))
			return true;
	}
	return false;
}
bool TileMap::CollisionY(const Point& p, int distance) const
{
	int x, y, x0, x1;
	Tile tile;

	//Calculate the tile coordinates and the range of tiles to check for collision
	y = p.y / TILE_SIZE;
	x0 = p.x / TILE_SIZE;
	x1 = (p.x + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the horizontal range
	for (x = x0; x <= x1; ++x)
	{
		tile = GetTileIndex(x, y);

		//One solid or laddertop tile is sufficient
		if (IsTileSolid(tile) || IsTileLadderTop(tile))
			return true;
	}
	return false;
}
bool TileMap::TestOnLadder(const AABB& box, int* px) const
{
	int left, right, bottom;
	int tx1, tx2, ty;
	Tile tile1;

	//Control points
	left = box.pos.x;
	right = box.pos.x + box.width;
	bottom = box.pos.y + box.height - 1;

	//Calculate the tile coordinates
	tx1 = left / TILE_SIZE;
	tx2 = right / TILE_SIZE;
	ty = bottom / TILE_SIZE;

	//To be able to climb up or down, both control points must be on ladder
	tile1 = GetTileIndex(tx1, ty);

	if (IsTileLadder(tile1))
	{
		*px = GetLadderCenterPos(left, bottom) - box.width / 2;
		return true;
	}
	return false;
}
bool TileMap::TestOnLadderTop(const AABB& box, int* px) const
{
	int left, right, bottom;
	int tx1, tx2, ty;
	Tile tile1;

	//Control points
	left = box.pos.x;
	right = box.pos.x + box.width;
	bottom = box.pos.y + box.height - 1;

	//Calculate the tile coordinates
	tx1 = left / TILE_SIZE;
	tx2 = right / TILE_SIZE;
	ty = bottom / TILE_SIZE;

	//To be able to climb up or down, both control points must be on ladder
	tile1 = GetTileIndex(tx1, ty);

	if (IsTileLadderTop(tile1))
	{
		*px = GetLadderCenterPos(left, bottom) - box.width / 2;
		return true;
	}
	return false;
}
int TileMap::GetLadderCenterPos(int pixel_x, int pixel_y) const
{
	int tx, ty;

	tx = pixel_x / TILE_SIZE;
	ty = pixel_y / TILE_SIZE;
	Tile tile = GetTileIndex(tx, ty);

	if (tile == Tile::LADDER || tile == Tile::LADDER_TOP)		return tx * TILE_SIZE + TILE_SIZE;

	else
	{
		LOG("Internal error, tile should be a LADDER, coord: (%d,%d), tile type: %d", pixel_x, pixel_y, (int)tile);
		return 0;
	}
}
void TileMap::Render()
{
	Tile tile;
	Rectangle rc;
	Vector2 pos;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			tile = map[i * width + j];
			if (tile != Tile::AIR)
			{
				pos.x = (float)j * TILE_SIZE;
				pos.y = (float)i * TILE_SIZE;

				if (tile != Tile::LASER)
				{
					rc = dict_rect[(int)tile];
					DrawTextureRec(*img_tiles, rc, pos, WHITE);
				}
				else
				{
					laser->Draw((int)pos.x, (int)pos.y);
				}
			}
		}
	}
}
void TileMap::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_TILES);

	laser->Release();

	dict_rect.clear();
}