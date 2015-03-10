#include "StdAfx.h"
#include "Projectile.h"
#include "Ogre.h"
#include "Game.h"

using namespace Ogre;

Projectile::Projectile(SceneNode* node, Entity* entity, Vector3 position, float speed, Quaternion orientation) : 
	SpaceObject(node, entity, position, speed, orientation)
{
	bbs = sceneManager->createBillboardSet();
	this->node->attachObject(bbs);
	bbs->setDefaultDimensions(5,5);
	bbs->setBillboardType(BBT_ORIENTED_COMMON);
	bbs->setBillboardOrigin(BBO_CENTER);
	bbs->setCommonDirection((4 * Vector3::UNIT_Z + Vector3::UNIT_Y).normalisedCopy());
	bbs->setMaterialName("Projectile");
	bb = bbs->createBillboard(Vector3(0,0,0));
}

void Projectile::update(float dt)
{
	SpaceObject::update(dt);
}


Projectile::~Projectile(void)
{
}
