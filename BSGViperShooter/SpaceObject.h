#pragma once

#include "stdafx.h"
#include "Ogre.h"

using namespace Ogre;

class SpaceObject
{
protected:
	Vector3 position;
	float speed;
	Quaternion orientation;
	Quaternion rotationSpeed;
	SceneNode* node;
	Entity* entity;

public:
	SpaceObject(
		SceneNode* node,
		Entity* entity,
		Vector3 position = Vector3(0,0,0), 
		float speed = 0.0f,
		Quaternion orientation = Quaternion::IDENTITY);
	inline Vector3 getPosition() { return this->position; }
	inline void setPosition(Vector3 p) { this->position = p; }
	inline float getSpeed() { return this->speed; }
	inline void setSpeed(float s) { this->speed = s; }
	inline Vector3 getDirection() { return (this->orientation * Vector3(0,0,1)).normalisedCopy(); }
	inline Quaternion getOrientation() { return this->orientation; }
	inline void setOrientation(Quaternion o) { this->orientation = o; }
	inline Quaternion getRotationSpeed() { return this->rotationSpeed; }
	inline void setRotationSpeed(Quaternion rs) { this->rotationSpeed = rs; }
	inline SceneNode* getNode() { return this->node; }
	inline void setNode(SceneNode* n) { this->node = n; }
	inline Entity* getEntity() { return this->entity; }
	inline void setEntity(Entity* e) {this->entity = e; }
	virtual void update(float dt);
	~SpaceObject(void);
};

