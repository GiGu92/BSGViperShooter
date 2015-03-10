#pragma once

//#include "Inputs.h"
#include <AL/alut.h>

#define ALUT_SAFE_CALL(call, message) {call; \
	ALenum error = alutGetError(); \
	if(error != ALUT_ERROR_NO_ERROR) \
	Ogre::LogManager::getSingleton().logMessage(message + Ogre::String(" ") + Ogre::String(alutGetErrorString(error))); \
    }

#define AL_SAFE_CALL(call, message) {call; \
	ALenum error = alGetError(); \
	if(error != AL_NO_ERROR) \
	Ogre::LogManager::getSingleton().logMessage(message + Ogre::String(" ") + Ogre::String(alutGetErrorString(error))); \
    }

class GameAudio
{
	ALuint musicBuffer;
	ALuint gunShotBuffer;
	ALuint explosionBuffer;

	ALuint musicSource;
	ALuint* fighterSources;
		
	ALuint loadFile(const char* filename)
	{
		ALenum error;
		ALuint newBuffer;
		ALUT_SAFE_CALL(newBuffer = alutCreateBufferFromFile(filename), 
		     "Cannot load audio file: " + Ogre::String(filename));        
		return newBuffer;
	}

	void createSource(ALuint* source)
	{
		AL_SAFE_CALL(alGenSources (1, source), "Unable to generate source");
	}

public:

    GameAudio(int argc, char** argv)
    {
        Ogre::LogManager::getSingleton().logMessage("*** Initializing OpenAL ***\n");
        
        alutInit (&argc, argv);
		ALenum error; 
		error = alutGetError();
        if(error != AL_NO_ERROR)
        {
            Ogre::LogManager::getSingleton().logMessage("Error initializing OpenAL: " + Ogre::String(alutGetErrorString(error)));
        }
    }
    
    ~GameAudio()
    {
        Ogre::LogManager::getSingleton().logMessage("*** Shutting down OpenAL ***\n");
        
        alutExit();
    }
    

	void loadLevelResources(int fighterCount)
	{		
        musicBuffer = loadFile("media/cylonmusic.wav");
		gunShotBuffer =  loadFile("media/gunshot.wav");
		explosionBuffer =  loadFile("media/explosion.wav");

		fighterSources = new ALuint[fighterCount];
		for(int i = 0; i < fighterCount; ++i)
		{
			alGenSources (1, &fighterSources[i]);
			alSourcef(fighterSources[i], AL_GAIN, 1);
			//alSourcei(fighterSources[i], AL_BUFFER, gunShotBuffer);
		}

		createSource(&musicSource);
		AL_SAFE_CALL(alSourcei (musicSource, AL_BUFFER, musicBuffer), "unable to bind title buffer");
        AL_SAFE_CALL(alSourcei (musicSource, AL_LOOPING, AL_TRUE), "unable to set looping");
        alSourcef (musicSource, AL_GAIN, 0.5f);
	}

	void playMusic()
	{
		//AL_SAFE_CALL(alSourcePause(fighterSources[0]), "unable to stop viper source");
		//AL_SAFE_CALL(alSourceStop(musicSource), "unable to stop music source");
		ALenum state;
		alGetSourcei(musicSource, AL_SOURCE_STATE, &state);
		if (state != AL_PLAYING)
		{
			AL_SAFE_CALL(alSourcei (musicSource, AL_BUFFER, musicBuffer), "unable to bind buffer to music source");
			AL_SAFE_CALL(alSourcePlay (musicSource), "unable to play music source");
		}
	}

	void playGunShotSound(Fighter* fighter)
	{
		AL_SAFE_CALL(alSourceStop(fighterSources[fighter->getID()]), "unable to stop fighter source" + Ogre::StringConverter::toString(fighter->getID()));
		AL_SAFE_CALL(alSourcei (fighterSources[fighter->getID()], AL_BUFFER, gunShotBuffer), "unable to bind buffer to fighter source " + Ogre::StringConverter::toString(fighter->getID()));
		AL_SAFE_CALL(alSource3f(fighterSources[fighter->getID()], AL_POSITION, 
			fighter->getPosition().x, fighter->getPosition().y, fighter->getPosition().z), 
			"unable to set position to fighter source " + Ogre::StringConverter::toString(fighter->getID()));
		AL_SAFE_CALL(alSourcePlay (fighterSources[fighter->getID()]), "unable to play fighter source" + Ogre::StringConverter::toString(fighter->getID()));
		alSourcef(fighterSources[fighter->getID()], AL_GAIN, 0.1f);
	}

	void playExplosionSound(Fighter* fighter)
	{
		AL_SAFE_CALL(alSourceStop(fighterSources[fighter->getID()]), "unable to stop fighter source" + Ogre::StringConverter::toString(fighter->getID()));
		AL_SAFE_CALL(alSourcei (fighterSources[fighter->getID()], AL_BUFFER, explosionBuffer), "unable to bind buffer to fighter source " + Ogre::StringConverter::toString(fighter->getID()));
		AL_SAFE_CALL(alSource3f(fighterSources[fighter->getID()], AL_POSITION, 
			fighter->getPosition().x, fighter->getPosition().y, fighter->getPosition().z), 
			"unable to set position to fighter source " + Ogre::StringConverter::toString(fighter->getID()));
		AL_SAFE_CALL(alSourcePlay (fighterSources[fighter->getID()]), "unable to play fighter source" + Ogre::StringConverter::toString(fighter->getID()));
		alSourcef(fighterSources[fighter->getID()], AL_GAIN, 1.0f);
	}
    
	void setListenerPosition(Ogre::Vector3 pos, Ogre::Vector3 dir)
	{
		alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
		float orient[] = {dir.x, dir.y, dir.z, 0, 1, 0};
		alListenerfv(AL_ORIENTATION, orient);
	}
};


