#include "Sounds.h"

extern Sounds sounds;

Sounds::Sounds(void){}
Sounds::~Sounds(void){}


void Sounds::PlayMenuSoundtrack(bool play) {
	static ISoundEngine* SoundEngine = createIrrKlangDevice();

	if (play) SoundEngine->play2D("music/treasureHunterSoundtrack.mp3", true);
	else SoundEngine->stopAllSounds();
}

void Sounds::PlayWaves(bool play) {
	static ISoundEngine* SoundEngine = createIrrKlangDevice();

	if (play) SoundEngine->play2D("music/oceanWaves.mp3", true);
	else SoundEngine->stopAllSounds();
}

void Sounds::PlayHit(void) {
	ISoundEngine* SoundEngine = createIrrKlangDevice();
	SoundEngine->play2D("music/rockHit.mp3");
}

void Sounds::PlayBullet(void) {
	ISoundEngine* SoundEngine = createIrrKlangDevice();
	SoundEngine->play2D("music/cannonFire.mp3");
}

void Sounds::PlayEnemySinking(void) {
	ISoundEngine* SoundEngine = createIrrKlangDevice();
	SoundEngine->play2D("music/sinkingShip.mp3");
}

void Sounds::PlayEnemyAppearing(bool play) {
	static ISoundEngine* SoundEngine = createIrrKlangDevice();

	if (play) SoundEngine->play2D("music/enemyAppearing.mp3");
	else SoundEngine->stopAllSounds();
}

void Sounds::PlayClick(void) {
	ISoundEngine* SoundEngineApp = createIrrKlangDevice();
	SoundEngineApp->play2D("music/click.mp3");
}

void Sounds::PlayVictory(bool play) {
	static ISoundEngine* SoundEngineVic = createIrrKlangDevice();

	if (play) SoundEngineVic->play2D("music/victory.mp3");
	else SoundEngineVic->stopAllSounds();
}

void Sounds::PlayGameOver(bool play) {
	static ISoundEngine* SoundEngineOv = createIrrKlangDevice();

	if (play) SoundEngineOv->play2D("music/gameover.mp3");
	else SoundEngineOv->stopAllSounds();
}

void Sounds::PlayPowerUp(void) {
	ISoundEngine* SoundEngineApp = createIrrKlangDevice();
	SoundEngineApp->play2D("music/powerUp.mp3");
}
