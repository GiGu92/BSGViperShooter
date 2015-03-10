#pragma once
#include "Ogre.h"
//#include "Fighter.h"

using namespace Ogre;

class Fighter;

class Thruster
{
private:
	Fighter* fighter;
	Vector3 relativePos;
	float length;
	ParticleSystem* ps;
public:
	Thruster(
		Fighter* fighter,
		Vector3 relativePos);
	inline float getLength() { return length; }
	inline void setLength(float l) { this->length = l; }
	void update(float dt);
	~Thruster(void);
};

