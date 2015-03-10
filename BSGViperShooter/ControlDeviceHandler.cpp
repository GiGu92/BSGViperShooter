#include "StdAfx.h"
#include "Game.h"
#include "ControlDeviceHandler.h"
#include "OgreMath.h"

using namespace Ogre;

ControlDeviceHandler::ControlDeviceHandler(void)
{
	LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
		
	OIS::ParamList pl;

	size_t windowHnd = 0;
	renderWindow->getCustomAttribute("WINDOW", &windowHnd);
	std::ostringstream windowHndStr;
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	OISInputManager = OIS::InputManager::createInputSystem( pl );

	mKeyboard  =  static_cast<OIS::Keyboard*>(OISInputManager->createInputObject( OIS::OISKeyboard, false ));
	xinput = new XInputManager();	

	isResetting = false;
}

bool ControlDeviceHandler::frameStarted(const FrameEvent& evt)
{
	const float rs = 2.0f;
	const float dt = evt.timeSinceLastFrame;

	mKeyboard->capture();

	xinput->Update();
	GamepadState gamepadState = xinput->GetState(0);

	float gamepadDeadSpace = 20.0f;

	// GamePad
	if (!gamepadState.error)
	{
		// Pitch
		if (gamepadState.RightStick.Y < -gamepadDeadSpace)
		{
			Vector3 up = viper->getOrientation() * Vector3(0,1,0);
			viper->turn(up, dt, Math::Abs(gamepadState.RightStick.Y / 128.0f));
		} 
		else if (gamepadState.RightStick.Y > gamepadDeadSpace)
		{
			Vector3 down = viper->getOrientation() * Vector3(0,-1,0);
			viper->turn(down, dt, Math::Abs(gamepadState.RightStick.Y / 128.0f));
		}

		// Yaw
		if (gamepadState.RightStick.X < -gamepadDeadSpace)
		{
			Vector3 left = viper->getOrientation() * Vector3(1,0,0);
			viper->turn(left, dt, Math::Abs(gamepadState.RightStick.X / 128.0f));
		} 
		else if (gamepadState.RightStick.X > gamepadDeadSpace)
		{
			Vector3 right = viper->getOrientation() * Vector3(-1,0,0);
			viper->turn(right, dt, Math::Abs(gamepadState.RightStick.X / 128.0f));
		}

		// Roll
		if (gamepadState.LeftStick.X < -gamepadDeadSpace)
		{
			viper->roll(true, dt, Math::Abs(gamepadState.LeftStick.X / 128.0f));
		}
		else if (gamepadState.LeftStick.X > gamepadDeadSpace)
		{
			viper->roll(false, dt, Math::Abs(gamepadState.LeftStick.X / 128.0f));
		}

		// Throttle
		if (gamepadState.RightTrigger > 0)
		{
			viper->accelerate(dt * gamepadState.RightTrigger);
		}
		if (gamepadState.LeftTrigger > 0)
		{
			viper->decelerate(dt * gamepadState.LeftTrigger);
		}

		// Shoot
		if (gamepadState.Buttons.RightShoulder)
		{
			viper->shoot(dt);
		}

		// Reset
		if (gamepadState.Buttons.Back)
		{
			isResetting = true;
		}
		else if (isResetting)
		{
			isResetting = false;
			resetScene();
		}

		// Exit
		if (gamepadState.Buttons.Start)
		{
			return false;
		}
	}

	

	// Keyboard
	if (mKeyboard->isKeyDown(OIS::KC_ESCAPE))
	{
		return false;
	}

	// Reset
	if (mKeyboard->isKeyDown(OIS::KC_BACK))
	{
		resetScene();
	}

	if (mKeyboard->isKeyDown(OIS::KC_W))
	{
		Vector3 down = viper->getOrientation() * Vector3(0,-1,0);
		viper->turn(down, dt);
	}
	if (mKeyboard->isKeyDown(OIS::KC_S))
	{
		Vector3 up = viper->getOrientation() * Vector3(0,1,0);
		viper->turn(up, dt);
	}
	if (mKeyboard->isKeyDown(OIS::KC_A))
	{
		viper->roll(true, dt);
	}
	if (mKeyboard->isKeyDown(OIS::KC_D))
	{
		viper->roll(false, dt);
	}
	if (mKeyboard->isKeyDown(OIS::KC_Q))
	{
		Vector3 left = viper->getOrientation() * Vector3(1,0,0);
		viper->turn(left, dt);
	}
	if (mKeyboard->isKeyDown(OIS::KC_E))
	{
		Vector3 right = viper->getOrientation() * Vector3(-1,0,0);
		viper->turn(right, dt);
	}

	if (mKeyboard->isKeyDown(OIS::KC_LSHIFT))
	{
		viper->accelerate(dt);
	}
	if (mKeyboard->isKeyDown(OIS::KC_LCONTROL))
	{
		viper->decelerate(dt);
	}
	
	if (mKeyboard->isKeyDown(OIS::KC_SPACE))
	{
		viper->shoot(dt);
	}
		
	return true;
}

ControlDeviceHandler::~ControlDeviceHandler(void)
{
}
