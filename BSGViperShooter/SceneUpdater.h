#pragma once
#include "ogreframelistener.h"
#include "stdafx.h"
#include "Ogre.h"
#include <OIS/OIS.h>

using namespace Ogre;

class SceneUpdater : public FrameListener
{
protected:
	float timeSinceHit;
	void detectCollisions();
public:
	SceneUpdater(void);
	bool frameStarted(const FrameEvent&);
	~SceneUpdater(void);
};

