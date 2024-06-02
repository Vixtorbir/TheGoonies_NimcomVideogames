#include "Object.h"
#include "StaticImage.h"

Object::Object(const Point& p, ObjectType t) : Entity(p, OBJECT_PHYSICAL_SIZE, OBJECT_PHYSICAL_SIZE, OBJECT_FRAME_SIZE, OBJECT_FRAME_SIZE)
{
	type = t; 
	
	Rectangle rc;
	const int n = TILE_SIZE;
	switch (type)
	{
		case ObjectType::KEY: rc = {0, 7*n, n, n}; break;
		case ObjectType::HELMET: rc = {1*n, 7*n, n, n}; break;
		case ObjectType::EXP: rc = {2*n, 7*n, n, n}; break;
		case ObjectType::PONER_LLAVE: rc = {3*n, 7*n, n, n}; break;
		case ObjectType::SHOES: rc = { 1 * n, 7 * n, n, n }; break;

		default: LOG("Internal error: object creation of invalid type");
	}

	ResourceManager& data = ResourceManager::Instance();
	render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
}
Object::~Object()
{
}
void Object::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}
int Object::Points() const
{
	if (type == ObjectType::KEY)		return POINTS_KEY;
	else if (type == ObjectType::HELMET)	return POINTS_OBJECT;
	else if (type == ObjectType::EXP)	return POINTS_OBJECT;
	else if (type == ObjectType::SHOES)	return POINTS_OBJECT;
	else
	{
		LOG("Internal error: object type invalid when giving points");
		return 0;
	}
}
int Object::EXPpoints() const
{
	if (type == ObjectType::EXP)	return 40;
	else
	{
		LOG("Internal error: object type invalid when giving points");
		return 0;
	}
}
ObjectType Object::GetID() const
{
	if (type == ObjectType::KEY)		return ObjectType::KEY;
	else if (type == ObjectType::HELMET)	return ObjectType::HELMET;
	else if (type == ObjectType::EXP)	return ObjectType::EXP;
	else if (type == ObjectType::PONER_LLAVE)	return ObjectType::PONER_LLAVE;
	else if (type == ObjectType::SHOES)	return ObjectType::SHOES;
		
}