#pragma once
#include "spaceobject.h"
#include "Ogre.h"

using namespace Ogre;

class Projectile :
	public SpaceObject
{
private:
	BillboardSet* bbs;
	Billboard* bb;
public:
	Projectile(
		SceneNode* node,
		Entity* entity,
		Vector3 position = Vector3(0,0,0), 
		float speed = 0.0f,
		Quaternion orientation = Quaternion::IDENTITY);
	void update(float dt);
	~Projectile(void);
};

