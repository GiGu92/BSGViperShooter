#include "StdAfx.h"
#include "SpaceObject.h"
#include "Ogre.h"

SpaceObject::SpaceObject(
	SceneNode* node, Entity* entity, Vector3 position, float speed, Quaternion orientation) : 
	node(node), entity(entity), position(position), speed(speed), orientation(orientation)
{ 
	rotationSpeed = Quaternion::IDENTITY;
}

void SpaceObject::update(float dt)
{
	if (dt > 0)
	{
		this->orientation = this->orientation * this->rotationSpeed * dt;
		this->orientation.normalise();
		this->rotationSpeed.normalise();
		this->position += this->getDirection() * this->speed * dt;
		
		this->node->setPosition(this->position);
		this->node->setOrientation(this->orientation);
	}
}

SpaceObject::~SpaceObject(void)
{
}