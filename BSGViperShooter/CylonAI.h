#pragma once
#include "ogreframelistener.h"
#include "Game.h"

class CylonAI :	public FrameListener
{
public:
	CylonAI(void);
	bool frameStarted(const FrameEvent&);
	~CylonAI(void);
};

