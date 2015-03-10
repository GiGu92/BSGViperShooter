#include "StdAfx.h"
#include "GamePadListener.h"
#include "Game.h"


GamePadListener::GamePadListener(void)
{
	LogManager::getSingletonPtr()->logMessage("*** Initializing JoyStick OIS ***");
		
	OIS::ParamList pl;

	size_t windowHnd = 0;
	renderWindow->getCustomAttribute("WINDOW", &windowHnd);
	std::ostringstream windowHndStr;
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	OISInputManager = OIS::InputManager::createInputSystem( pl );

	mJoy =  static_cast<OIS::JoyStick*>(OISInputManager->createInputObject( OIS::OISJoyStick, true ));
}

bool GamePadListener::frameStarted(const FrameEvent& evt)
{
	const float rs = 2.0f;
	const float dt = evt.timeSinceLastFrame;

	mJoy->capture();
	OIS::JoyStickState joyStickState = mJoy->getJoyStickState();

	std::vector<OIS::Axis> axes = joyStickState.mAxes;
	std::vector<bool> buttons = joyStickState.mButtons;

	bool isSteering = false;

	return true;
}

GamePadListener::~GamePadListener(void)
{
}
