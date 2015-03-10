#include "StdAfx.h"
#include "CylonAI.h"
#include "Game.h"

using namespace Ogre;

CylonAI::CylonAI(void)
{
}

bool CylonAI::frameStarted(const FrameEvent& evt)
{
	float dt = evt.timeSinceLastFrame;

	for (int i = 1; i < fighters.size(); i++)
	{
		float distanceToViper = (viper->getPosition() - fighters[i]->getPosition()).length();
		Vector3 directionToViper = viper->getPosition() - fighters[i]->getPosition();
		directionToViper.normalise();

		if (fighters[i]->getDirection().angleBetween(directionToViper) < Radian(Math::PI / 6.0f))
			{
				fighters[i]->shoot(dt);
			}

		if (distanceToViper > 50)
		{
			if (fighters[i]->getDirection().angleBetween(directionToViper) > Radian(0.001f))
			{
				fighters[i]->turn(directionToViper, dt);
			}

			if (distanceToViper < 100)
			{
				fighters[i]->getToSpeed(fighters[i]->getMaxSpeed() / 2.0f, dt);
			}
			else
			{
				fighters[i]->getToSpeed(fighters[i]->getMaxSpeed(), dt);
			}
		}
		else if (distanceToViper < 50)
		{
			Vector3 reflection = fighters[i]->getDirection().reflect(viper->getOrientation().yAxis());
			fighters[i]->turn(reflection, dt);
			fighters[i]->getToSpeed(fighters[i]->getMaxSpeed(), dt);
		}
	}

	return true;
}

CylonAI::~CylonAI(void)
{
}
