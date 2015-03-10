#pragma once

#include "Ogre.h"
#include "stdafx.h"

using namespace Ogre;

class SpaceDust
{
private:
	float maxDist; 
	float mirrorDist; 
	float dimFactor;
	ParticleSystem* ps;
	SceneNode* node;
public:
	SpaceDust(void);
	void update(float dt);
	~SpaceDust(void);
};

