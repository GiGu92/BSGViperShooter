#pragma once
#include "Ogre.h"
#include "OGRE/Overlay/OgreOverlayManager.h"

using namespace Ogre;

class HUD
{
private:
	OverlayElement* healthText;
	OverlayElement* killsText;
	OverlayElement* speedText;
	OverlayElement* gameOverText;
public:
	HUD(void);
	void update(float dt);
	~HUD(void);
};

