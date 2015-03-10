#pragma once
#include "spaceobject.h"
#include "Projectile.h"
//#include "Thruster.h"

class Thruster;
enum FighterType;

class Fighter :	public SpaceObject
{
protected:
	FighterType type;
	int health;
	int kills;
	int id;
	float timeSinceLastShot;
	float timeBetweenShots;
	float maxSpeed;
	float maxRotationSpeed;
	std::vector<Thruster*> thrusters;
	std::vector<Projectile*> projectiles;
	void removeTooFarProjectiles();
public:
	Fighter(
		SceneNode* node,
		Entity* entity,
		FighterType type,
		int id,
		Vector3 position = Vector3(0,0,0), 
		float speed = 0.0f,
		int health = 1,
		Quaternion orientation = Quaternion::IDENTITY);
	void initThrusters();
	inline std::vector<Projectile*>* getProjectiles() { return &projectiles; }
	inline float getMaxSpeed() { return maxSpeed; }
	void accelerate(float dt, float power = 1.0f);
	void decelerate(float dt, float power = 1.0f);
	void getToSpeed(float targetSpeed, float dt);
	inline FighterType getType() { return type; }
	inline int getID() { return id; }
	inline int getHealth() { return health; }
	inline void decreaseHealth() { health--; }
	inline int getKills() { return kills; }
	inline void increaseKills() { kills++; }
	void turn(Vector3 targetDirection, float dt, float power = 1.0f);
	void roll(bool left, float dt, float power = 1.0f);
	void slide(float dt);
	void shoot(float dt);
	void update(float dt);
	inline std::vector<Thruster*>* getThrusters() { return &thrusters; }
	inline void addThruster(Thruster* p) { thrusters.insert(thrusters.begin(), p); }
};

