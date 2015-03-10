#include "StdAfx.h"
#include "SpaceDust.h"
#include "Game.h"
#include "Ogre.h"

SpaceDust::SpaceDust(void)
{
	maxDist = 250.0;
	mirrorDist = maxDist * 0.99;
	dimFactor = 0.8*0.005*0.005;
	ps = sceneManager->createParticleSystem("SpaceDust", "SpaceDust");
	node = sceneManager->getRootSceneNode()->createChildSceneNode("SpaceDustNode");
	node->attachObject(ps);
}

void SpaceDust::update(float dt)
{
	ParticleIterator pit = ps->_getIterator();

	while (!pit.end())
	{
		Particle* particle = pit.getNext(); 
		Vector3& pos = particle->position; 
		particle->timeToLive = 999999.0f; 
		Vector3 pDir = pos - camera->getPosition(); 
		float dist = pDir.squaredLength(); 
		float dim = dist * dimFactor; 
		particle->setDimensions(dim, dim); 
		
		if (dist > maxDist * maxDist) { 
			pDir.normalise(); 
			Vector3 p = camera->getPosition() - pDir*mirrorDist;       
			particle->position = p; 
		} 
	}
}

SpaceDust::~SpaceDust(void)
{
}
