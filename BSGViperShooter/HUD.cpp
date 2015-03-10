#include "StdAfx.h"
#include "HUD.h"
#include "Ogre.h"
#include "Game.h"
#include "OGRE/Overlay/OgreOverlayManager.h"
#include "OGRE/Overlay/OgreOverlayContainer.h"
#include "OGRE/Overlay/OgreOverlaySystem.h"

using namespace Ogre;

HUD::HUD(void)
{
	OverlayManager* om = OverlayManager::getSingletonPtr();
	Overlay* mainOverlay = om->getByName("MainOverlay");
	
	mainOverlay->show();

	// Create overlay
	healthText = om->getOverlayElement("Health");
	killsText = om->getOverlayElement("Kills");
	speedText = om->getOverlayElement("Speed");

	gameOverText = om->getOverlayElement("GameOver");
	gameOverText->setCaption("Game Over.");

	healthText->show();
	killsText->show();
	speedText->show();
	gameOverText->show();
}

void HUD::update(float dt)
{
	healthText->setCaption("Health: " + StringConverter::toString(viper->getHealth()));
	killsText->setCaption("Kills: " + StringConverter::toString(viper->getKills()));
	speedText->setCaption("Speed: " + StringConverter::toString((Math::Floor(viper->getSpeed() + 0.5f))));

	if (isGameOver)
	{
		gameOverText->setCaption("Game Over.");
	}
	else
	{
		gameOverText->setCaption("");
	}
}

HUD::~HUD(void)
{
}
