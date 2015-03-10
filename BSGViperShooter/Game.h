#pragma once

#include "stdafx.h"
#include "Fighter.h"
#include "SpaceDust.h"
#include "HUD.h"
#include "ControlDeviceHandler.h"
#include "GameAudio.h"
#include "Ogre.h"
#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>
#include "OGRE/Overlay/OgreOverlayManager.h"

using namespace Ogre;

extern Root* ogreRoot;
extern SceneManager* sceneManager;
extern Camera* camera;
extern RenderWindow* renderWindow;
extern Light* light;
extern SpaceDust* spaceDust;

extern OIS::InputManager* OISInputManager;
extern ControlDeviceHandler* controlDeviceHandler;
extern HUD* hud;
extern bool isGameOver;
extern bool isPaused;

extern GameAudio* gameAudio;

extern Fighter* viper;
extern std::vector<Fighter*> fighters;

enum FighterType { VIPER, RAIDER };

void resetScene();