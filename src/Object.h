#pragma once
#include "Entity.h"

#define OBJECT_PHYSICAL_SIZE	8
#define OBJECT_FRAME_SIZE		8

#define POINTS_KEY	10
#define POINTS_OBJECT	20

enum class ObjectType { KEY, HELMET, EXP, PONER_LLAVE };

class Object : public Entity
{
public:
	Object(const Point& p, ObjectType t);
	~Object();

	void DrawDebug(const Color& col) const;
	int Points() const;
	int EXPpoints() const;

	ObjectType GetID() const;




private:
	ObjectType type;

};

