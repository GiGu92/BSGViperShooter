#include "GameAudio.h"
#include "AL\alut.h"
#include "Game.h"

GameAudio::GameAudio(int argc, char** argv)
{
    alutInit (&argc, argv);
	
	musicBuffer = alutCreateBufferFromFile("media/cylonmusic.wav");	
	alGenSources(1, &musicSource);
	alSourcei(musicSource, AL_BUFFER, musicBuffer);
	alSourcei(musicSource, AL_LOOPING, AL_TRUE);
	alSourcei(musicSource, AL_GAIN, 10);

	gunShotBuffer = alutCreateBufferFromFile("media/gunburst.wav");
	alGenSources(1, &gunShotSource);
	alSourcei(gunShotSource, AL_BUFFER, gunShotBuffer);
	alSourcei(gunShotSource, AL_LOOPING, AL_TRUE);
	alSourcei(gunShotSource, AL_GAIN, 10);

	explosionBuffer = alutCreateBufferFromFile("media/explosion.wav");
	alGenSources(1, &explosionSource);
	alSourcei(explosionSource, AL_BUFFER, explosionBuffer);
	alSourcei(explosionSource, AL_LOOPING, AL_TRUE);
	alSourcei(explosionSource, AL_GAIN, 10);

	/*mgr = cAudio::createAudioManager(true);
	musicSrc = mgr->create("music", "media/cylonmusic.wav", true);
	gunShotSrc = mgr->create("music", "media/gunburst.wav", true);*/
}

void GameAudio::playMusic()
{
	alSourcePlay(musicSource);
	/*if (musicSrc)
	{
		musicSrc->play2d(true);
		while(musicSrc->isPlaying())
		{
			cAudio::cAudioSleep(10); 
		}
	}*/
}

void GameAudio::playGunShotSound(Fighter* fighter)
{
	//alSourcePlay(gunShotSource);
}

void GameAudio::playExplosionSound(Fighter* fighter)
{

}

GameAudio::~GameAudio(void)
{
	alutExit();
	//mgr->shutDown();
	//cAudio::destroyAudioManager(mgr);
}
