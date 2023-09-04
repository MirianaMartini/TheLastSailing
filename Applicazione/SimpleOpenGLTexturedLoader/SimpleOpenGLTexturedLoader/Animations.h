#pragma once
#include "Settings.h"

class Animations {
public:
	Animations(void);
	~Animations(void);

	/* ANIMATION FUNCTIONS */
	void AnimationHit(GLint times);
	void AnimationShoot(void);
	void AnimationShootEnemy(GLint index, GLfloat* enemyPos); //Requires the index of the bullet and position of the enemy ship
	void AnimationEnemy(GLint index); //Requires the index of the enemy ship to show
	void AnimationEnemyMoving(GLint index); //Enemy moving while shooting --> LEVEL 3
	void AnimationSinking(GLint index, GLfloat* enemyPos); 

	/* ANIMATIONS FLAGS */
	bool animationHit = false;
	bool animationSinking = false; //For AnimationSinking
	bool firstAnimationHit = true; //For AnimationHit
	bool firstAnimationEnemy = true; //For AnimationEnemy
	bool firstAnimationEnemyMoving = true; //For AnimationShootEnemyMoving
	bool firstAnimationSinking = true; //For AnimationSinking
	bool firstAnimationShootEnemy = true; //For AnimationShootEnemy

private:

};

