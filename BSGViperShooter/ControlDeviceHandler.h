#pragma once
#include "ogreframelistener.h"
#include "Ogre.h"
#include <OIS/OIS.h>
#include "XInputManager.h"

using namespace Ogre;

class ControlDeviceHandler : public FrameListener
{
protected:
	OIS::Keyboard* mKeyboard;
	XInputManager* xinput;
	bool isResetting;
public:
	ControlDeviceHandler(void);
	inline XInputManager* getXInputManager() { return xinput; }
	bool frameStarted(const FrameEvent&);
	~ControlDeviceHandler(void);
};

