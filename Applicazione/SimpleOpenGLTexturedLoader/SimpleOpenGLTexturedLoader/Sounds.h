#pragma once
#include "Settings.h"

class Sounds {
public:
	Sounds(void);
	~Sounds(void);

	/* ANIMATION FUNCTIONS */
	void PlayMenuSoundtrack(bool play);
	void PlayWaves(bool play);
	void PlayHit(void);
	void PlayBullet(void);
	void PlayEnemySinking(void);
	void PlayEnemyAppearing(bool play);
	void PlayClick(void);
	void PlayVictory(bool play);
	void PlayGameOver(bool play);
	void PlayPowerUp(void);

private:

};

