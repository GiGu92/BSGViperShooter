#include "stdafx.h"
#include "Game.h"
#include "ControlDeviceHandler.h"
#include "SceneUpdater.h"
#include "CylonAI.h"
#include "Fighter.h"
#include "SpaceDust.h"
#include "Thruster.h"
#include "HUD.h"
#include "GameAudio.h"

#include "Ogre.h"
#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>
#include "OGRE/Overlay/OgreOverlayManager.h"
#include "OGRE/Overlay/OgreOverlaySystem.h"

using namespace Ogre;

Root* ogreRoot;
SceneManager* sceneManager;
Camera* camera;
RenderWindow* renderWindow;
Light* light;
SpaceDust* spaceDust;

OIS::InputManager* OISInputManager;
ControlDeviceHandler* controlDeviceHandler;

HUD* hud;
bool isGameOver;
bool isPaused;

GameAudio* gameAudio;

Fighter* viper;
std::vector<Fighter*> fighters;

void setupScene()
{
	// Controller setup
	controlDeviceHandler = new ControlDeviceHandler();

	// Camera setup
	
	camera = sceneManager->createCamera("Main Camera");
	camera->setPosition(5, 5, 5); 
	camera->lookAt(0, 0, 0);
	camera->setFarClipDistance(1000);
	camera->setNearClipDistance(0.1);
	camera->setAutoAspectRatio(true);

	// Light setup
	light = sceneManager->createLight("pointlight1");
	light->setType(Light::LT_DIRECTIONAL);
	light->setAttenuation(1000,1,0,0);
	light->setPowerScale(1000);
	light->setCastShadows(true);
	light->setDirection(-0.35, 0.64, -0.68);
	sceneManager->setAmbientLight(ColourValue(0.1f,0.1f,0.1f,1.0f));

	// SpaceDust setup
	spaceDust = new SpaceDust();

	// Viewport
	Viewport* viewport = renderWindow->addViewport(camera);
	viewport->setBackgroundColour(ColourValue(0.5,0.5,0.5));
	renderWindow->setActive(true); 

	// Shadow
	sceneManager->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);
	sceneManager->setShadowColour(ColourValue(0.5,0.5,0.5));
	
	// Skybox
	sceneManager->setSkyBox(true, "Space", 10, true);

	// Scene
	SceneNode* rootNode = sceneManager->getRootSceneNode();

	// Fighters
	
	SceneNode* viperNode = rootNode->createChildSceneNode();
	Entity* viperEntity = sceneManager->createEntity("viper", "model/Viper/Viper-Mk-II.mesh");
	viperNode->attachObject(viperEntity);
	viper = new Fighter(viperNode, viperEntity, FighterType::VIPER, 0, Vector3(0,0,0), 0.0f, 5);
	viper->initThrusters();
	viperNode->setVisible(true);
	fighters.insert(fighters.begin(), viper);
	
	SceneNode* raider1Node = rootNode->createChildSceneNode();
	Entity* raider1Entity = sceneManager->createEntity("model/Raider/Cylon-Raider.mesh");
	raider1Node->attachObject(raider1Entity);
	//raider1Node->showBoundingBox(true);
	Fighter* raider1 = new Fighter(raider1Node, raider1Entity, FighterType::RAIDER, 1, Vector3(-20,30,200));
	raider1->initThrusters();
	fighters.insert(fighters.end(), raider1);

	SceneNode* raider2Node = rootNode->createChildSceneNode();
	Entity* raider2Entity = sceneManager->createEntity("model/Raider/Cylon-Raider.mesh");
	raider2Node->attachObject(raider2Entity);
	//raider2Node->showBoundingBox(true);
	Fighter* raider2 = new Fighter(raider2Node, raider2Entity, FighterType::RAIDER, 2, Vector3(0,20,-150));
	raider2->initThrusters();
	fighters.insert(fighters.end(), raider2);

	SceneNode* raider3Node = rootNode->createChildSceneNode();
	Entity* raider3Entity = sceneManager->createEntity("model/Raider/Cylon-Raider.mesh");
	raider3Node->attachObject(raider3Entity);
	//raider3Node->showBoundingBox(true);
	Fighter* raider3 = new Fighter(raider3Node, raider3Entity, FighterType::RAIDER, 3, Vector3(+120,0,50));
	raider3->initThrusters();
	fighters.insert(fighters.end(), raider3);

	// HUD
	hud = new HUD();
	isGameOver = false;

	// Audio
	gameAudio->playMusic();

}

void resetScene()
{
	sceneManager->getRootSceneNode()->removeAndDestroyAllChildren();
	sceneManager->destroyAllBillboardSets();
	sceneManager->destroyAllCameras();
	sceneManager->destroyAllEntities();
	sceneManager->destroyAllLights();
	sceneManager->destroyAllParticleSystems();

	renderWindow->removeAllViewports();

	fighters.clear();

	setupScene();
}

void setupListeners()
{
	ogreRoot->addFrameListener(controlDeviceHandler);
	ogreRoot->addFrameListener(new SceneUpdater());
	ogreRoot->addFrameListener(new CylonAI());
}

int _tmain(int argc, _TCHAR* argv[])
{
	ogreRoot = new Root("","");
#ifdef _DEBUG
	ogreRoot->loadPlugin("RenderSystem_Direct3D9_d");
	ogreRoot->loadPlugin("Plugin_OctreeSceneManager_d");
	ogreRoot->loadPlugin("Plugin_ParticleFX_d");
#else
	ogreRoot->loadPlugin("RenderSystem_Direct3D9");
	ogreRoot->loadPlugin("Plugin_OctreeSceneManager");
	ogreRoot->loadPlugin("Plugin_ParticleFX");
#endif

	RenderSystemList list = ogreRoot->getAvailableRenderers(); 
	ogreRoot->setRenderSystem(list.at(0));
	ogreRoot->initialise(false);

	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	renderWindow = ogreRoot->createRenderWindow("Battlestar Galactica - Viper Shooter", desktop.right, desktop.bottom, true);

	sceneManager = ogreRoot->createSceneManager(0, "OctreeSceneManager"); 
	OverlaySystem* overlaySystem = OGRE_NEW OverlaySystem();
	sceneManager->addRenderQueueListener(overlaySystem);

	ResourceGroupManager::getSingleton().addResourceLocation("media", "FileSystem", "General", false);
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups(); 

	gameAudio = new GameAudio(argc, (char**)argv);
	gameAudio->loadLevelResources(4);

	setupScene();
	setupListeners();

	//ogreRoot->startRendering(); 
	while (true)
	{
		if (renderWindow->isClosed())
		{
			ogreRoot->shutdown();
			break;
		}
		WindowEventUtilities::messagePump();
		
		if (!ogreRoot->renderOneFrame())
		{
			ogreRoot->shutdown();
			break;
		}
	}

	return 0;
}
