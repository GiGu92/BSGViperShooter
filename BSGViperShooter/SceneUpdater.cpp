#include "StdAfx.h"
#include "SceneUpdater.h"
#include "Game.h"
#include "OgreMath.h"

SceneUpdater::SceneUpdater(void)
{
	timeSinceHit = 99;
}

void SceneUpdater::detectCollisions()
{
	std::vector<int> destroyedFighterIndices;
	for (int i = 0; i < fighters.size(); i++)
	{
		std::vector<int> destroyedProjectileIndices;
		for (int j = 0; j < fighters[i]->getProjectiles()->size(); j++)
		{
			for (int k = 0; k < fighters.size(); k++)
			{
				// if the fighters[k] is hit by the the j-th projectile of fighters[i]
				if ((fighters[k]->getPosition() - fighters[i]->getProjectiles()->at(j)->getPosition()).length() < 3.5f)
				{
					fighters[k]->decreaseHealth();

					controlDeviceHandler->getXInputManager()->SetVibration(0,1,1);
					timeSinceHit = 0.0f;

					destroyedProjectileIndices.insert(destroyedProjectileIndices.begin(), j);

					// Destroying collided projectile node
					sceneManager->getRootSceneNode()->removeAndDestroyChild(fighters[i]->getProjectiles()->at(j)->getNode()->getName());

					// if the fighter is destroyed by the hit
					if (fighters[k]->getHealth() <= 0)
					{
						fighters[i]->increaseKills();

						// Explosion
						String explosionID = fighters[i]->getNode()->getName() + fighters[k]->getNode()->getName() + StringConverter::toString(fighters[i]->getKills());
						ParticleSystem* particleSystem = sceneManager->createParticleSystem(explosionID + "PS", "Explosion");
						SceneNode* explisonNode = sceneManager->getRootSceneNode()->createChildSceneNode(explosionID + "Node", fighters[k]->getNode()->getPosition());
						explisonNode->attachObject(particleSystem);
						gameAudio->playExplosionSound(fighters[k]);

						// if the destroyed ship is the viper
						if (fighters[k]->getType() == FighterType::VIPER)
						{
							// Game Over
							isGameOver = true;
							controlDeviceHandler->getXInputManager()->SetVibration(0,0,0);
						}
						else
						{
							destroyedFighterIndices.insert(destroyedFighterIndices.begin(), k);

							// Destroying fighter node and projectiles
							for (int l = 0; l < fighters[k]->getProjectiles()->size(); l++)
							{
								sceneManager->getRootSceneNode()->removeAndDestroyChild(fighters[k]->getProjectiles()->at(l)->getNode()->getName());
							}
							fighters[k]->getProjectiles()->clear();
							sceneManager->getRootSceneNode()->removeAndDestroyChild(fighters[k]->getNode()->getName());
						}
					}
				}
			}
		}
		// Removing collided projectiles from model
		for (int j = 0; j < destroyedProjectileIndices.size(); j++)
		{
			fighters[i]->getProjectiles()->erase(fighters[i]->getProjectiles()->begin() + destroyedProjectileIndices[j] - j);
		}
	}
	// Removing destroyed fighters from model
	std::vector<int> destroyedFighterIDs;
	for (int i = 0; i < destroyedFighterIndices.size(); i++)
	{
		destroyedFighterIDs.insert(destroyedFighterIDs.begin(), fighters[destroyedFighterIndices[i] - i]->getID());
		fighters.erase(fighters.begin() + destroyedFighterIndices[i] - i);
	}

	// Adding new raiders to replace the destroyed ones
	for (int i = 0; i < destroyedFighterIndices.size(); i++)
	{
		SceneNode* raiderNode = sceneManager->getRootSceneNode()->createChildSceneNode();
		Entity* raiderEntity = sceneManager->createEntity("model/Raider/Cylon-Raider.mesh");
		raiderNode->attachObject(raiderEntity);
		//raiderNode->showBoundingBox(true);
		Vector3 position = viper->getPosition() + Vector3(
			Math::RangeRandom(-1,1) > 0 ? Math::RangeRandom(100,200) : Math::RangeRandom(-200,-100), 
			Math::RangeRandom(-1,1) > 0 ? Math::RangeRandom(100,200) : Math::RangeRandom(-200,-100), 
			Math::RangeRandom(-1,1) > 0 ? Math::RangeRandom(100,200) : Math::RangeRandom(-200,-100));
		int id = destroyedFighterIDs[0];
		destroyedFighterIDs.erase(destroyedFighterIDs.begin());
		Fighter* raider = new Fighter(raiderNode, raiderEntity, FighterType::RAIDER, id, position);
		raider->initThrusters();
		fighters.insert(fighters.end(), raider);
	}

	if (timeSinceHit > 0.3f)
	{
		controlDeviceHandler->getXInputManager()->SetVibration(0,0,0);
	}
}

bool SceneUpdater::frameStarted(const FrameEvent& evt)
{
	float dt = evt.timeSinceLastFrame;
	timeSinceHit += dt;

	if (!isGameOver)
	{
		viper->getNode()->setVisible(true);

		// Updating fighters
		for (int i = 0; i < fighters.size(); i++)
		{
			fighters[i]->update(dt);
		}

		// Detecting and handling collisions
		detectCollisions();

		// Updating camera
		camera->setPosition(viper->getPosition() - 20 * viper->getDirection() + 5 * camera->getUp());
		camera->setOrientation(viper->getOrientation() * Quaternion(0,0,1,0));
		//camera->setPosition(fighters[1]->getPosition() - 20 * fighters[1]->getDirection() + 5 * camera->getUp());
		//camera->setOrientation(fighters[1]->getOrientation() * Quaternion(0,0,1,0));

		// Updating stardust
		spaceDust->update(dt);

		// Updating HUD
		hud->update(dt);

		// Updating sound
		gameAudio->setListenerPosition(viper->getPosition(), viper->getDirection());
	}
	else
	{
		viper->getNode()->setVisible(false);
	}

	return true;
}

SceneUpdater::~SceneUpdater(void)
{
}
