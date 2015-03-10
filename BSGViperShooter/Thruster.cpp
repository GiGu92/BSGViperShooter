#include "StdAfx.h"
#include "Thruster.h"
#include "Game.h"
#include "Ogre.h"

using namespace Ogre;

Thruster::Thruster(Fighter* fighter, Vector3 relativePos) : fighter(fighter), relativePos(relativePos)
{
	ps = sceneManager->createParticleSystem(
		fighter->getNode()->getName() + StringConverter::toString(fighter->getThrusters()->size()) + "Thrusters", "Thrusters");
	ps->getEmitter(0)->setPosition(relativePos);

	fighter->getNode()->attachObject(ps);
}

void Thruster::update(float dt)
{
	float power = fighter->getSpeed() / fighter->getMaxSpeed();
	ps->getEmitter(0)->setParameter("time_to_live", StringConverter::toString(0.08f * power * power + 0.02f));
}


Thruster::~Thruster(void)
{
}
