#include "StdAfx.h"
#include "Fighter.h"
#include "Thruster.h"
#include "Projectile.h"
#include "Ogre.h"
#include "Game.h"

using namespace Ogre;

Fighter::Fighter(
	SceneNode* node, Entity* entity, FighterType type, int id, Vector3 position, float speed, int health, Quaternion orientation) :
	SpaceObject(node, entity, position, speed, orientation)
{	
	this->type = type;
	this->id = id;
	this->health = health;
	this->kills = 0;
	timeBetweenShots = 0.1f;
	timeSinceLastShot = timeBetweenShots;
	maxSpeed = 50.0f;
	maxRotationSpeed = 0.002f;	
}

void Fighter::initThrusters()
{
	// Thrusters
	if (this->type == FighterType::VIPER)
	{
		Thruster* t1 = new Thruster(this, Vector3(0.7,1.3,-4));
		this->addThruster(t1);
		Thruster* t2 = new Thruster(this, Vector3(-0.7,1.3,-4));
		this->addThruster(t2);
		Thruster* t3 = new Thruster(this, Vector3(0,1.9,-4));
		this->addThruster(t3);
	}
	else if (this->type == FighterType::RAIDER)
	{
		Thruster* t1 = new Thruster(this, Vector3(-0.5,-0.7,-4));
		this->addThruster(t1);
		Thruster* t2 = new Thruster(this, Vector3(0.5,-0.7,-4));
		this->addThruster(t2);
	}
}

void Fighter::accelerate(float dt, float power)
{
	float accelerationSpeed = 30.0f;

	if (power < 0) power = 0.0f;
	if (power > 1) power = 1.0f;

	if (this->speed < this->maxSpeed)
	{
		this->speed += accelerationSpeed * power * dt;
	}
	else
	{
		this->speed = this->maxSpeed;
	}
}

void Fighter::decelerate(float dt, float power)
{
	float decelerationSpeed = 30.0f;

	if (power < 0) power = 0.0f;
	if (power > 1) power = 1.0f;

	if (this->speed > 0)
	{
		this->speed -= decelerationSpeed * power * dt;
	}
	else
	{
		this->speed = 0;
	}
}

void Fighter::getToSpeed(float targetSpeed, float dt)
{
	if (targetSpeed > this->speed)
	{
		accelerate(dt);
	}
	else if (targetSpeed < this->speed)
	{
		decelerate(dt);
	}

	if (Math::Abs(targetSpeed - this->speed) < 0.5f)
	{
		this->speed = targetSpeed;
	}
}

void Fighter::turn(Vector3 targetDirection, float dt, float power)
{
	if (this->getDirection().angleBetween(targetDirection) > Radian(0.25f))
	{
		Quaternion inverseOrientation = this->getOrientation().Inverse();
		Vector3 targetDirectionLocal = (inverseOrientation * targetDirection).normalisedCopy();
		Vector3 currentDirectionLocal = (inverseOrientation * this->getDirection()).normalisedCopy();
		Vector3 axis = currentDirectionLocal.crossProduct(targetDirectionLocal).normalisedCopy();

		Quaternion rot;
		if (power < 0) power = 0.0f;
		if (power > 1) power = 1.0f;
		rot.FromAngleAxis(Radian(0.02f * power * dt), axis);
		//this->setOrientation(this->getOrientation() * rot);
		Quaternion rotproduct = this->rotationSpeed * rot;
		if (Math::Abs(rotproduct.getPitch().valueRadians()) < this->maxRotationSpeed && 
			Math::Abs(rotproduct.getYaw().valueRadians()) < this->maxRotationSpeed)
		{
			this->rotationSpeed = rotproduct;
		}		
	}
}

void Fighter::roll(bool left, float dt, float power)
{
	Vector3 axis;
	if (left)
	{
		axis = Vector3(0,0,-1);
	}
	else
	{
		axis = Vector3(0,0,1);
	}

	Quaternion rot;
	if (power < 0) power = 0.0f;
	if (power > 1) power = 1.0f;
	rot.FromAngleAxis(Radian(0.02f * power * dt), axis);
	Quaternion rotproduct = this->rotationSpeed * rot;
	if (Math::Abs(rotproduct.getRoll().valueRadians()) < this->maxRotationSpeed * 2.0f)
	{
		this->rotationSpeed = rotproduct;
	}
}

void Fighter::slide(float dt)
{
	Vector3 axis;
	Radian angle;
	this->getRotationSpeed().ToAngleAxis(angle,axis);
	axis = -axis;

	Quaternion rot;
	rot.FromAngleAxis(Radian(0.01f * dt), axis);
	this->rotationSpeed = this->rotationSpeed * rot;
}

void Fighter::shoot(float dt)
{	
	if (timeSinceLastShot > timeBetweenShots)
	{		
		SceneNode* projectileNode = sceneManager->getRootSceneNode()->createChildSceneNode();
		//Entity* projectileEntity = sceneManager->createEntity("model/cube.mesh");
		Entity* projectileEntity = NULL;
		//projectileNode->attachObject(projectileEntity);
		Projectile* projectile = new Projectile(projectileNode, projectileEntity, this->position + this->getDirection() * 4.0f, 500, this->orientation);

		this->projectiles.insert(projectiles.begin(), projectile);
		timeSinceLastShot = 0.0f;

		if (this->type == FighterType::VIPER)
		{
			gameAudio->playGunShotSound(this);
		}
	}
}

void Fighter::removeTooFarProjectiles()
{
	int projectileTooFarIndex = projectiles.size();
	for (int i = 0; i < projectiles.size(); i++)
	{
		if ((projectiles[i]->getPosition() - this->getPosition()).length() > 500)
		{
			projectileTooFarIndex = i;
			break;
		}
	}
	for (int i = 0; i < projectiles.size(); i++)
	{
		if (i >= projectileTooFarIndex)
		{
			sceneManager->getRootSceneNode()->removeAndDestroyChild(projectiles[i]->getNode()->getName());
		}
	}
	projectiles.erase(projectiles.begin() + projectileTooFarIndex, projectiles.end());
}

void Fighter::update(float dt)
{
	SpaceObject::update(dt);
	this->slide(dt);

	removeTooFarProjectiles();
	for (int i = 0; i < this->projectiles.size(); i++)
	{
		this->projectiles[i]->update(dt);
	}
	timeSinceLastShot += dt;

	for (int i = 0; i < this->thrusters.size(); i++)
	{
		this->thrusters[i]->update(dt);
	}
}