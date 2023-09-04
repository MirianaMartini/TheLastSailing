#pragma once
#include "Settings.h"

class Enemy {
public:
	Enemy(void);
	~Enemy(void);

	void SetPosition(GLfloat x, GLfloat y, GLfloat z);
	void DecreaseX();
	void IncreaseX();
	void DecreaseIdleX();
	void IncreaseIdleX();
	void DecreaseY();

	/* ACTIONS */
	void InitEnemy();
	bool IsHitShip_Ship(GLint enemyIndex, GLfloat* enemyPos); //if our ship hits enemy ship
	bool IsHitShip_Enemy(GLint enemyIndex, GLfloat* bulletPos, GLfloat* enemyPos); //if our ship bullet hits enemy ship
	bool IsHitEnemy_Ship(GLint enemyIndex, GLfloat* bulletPos); //if enemy shooting hits our ship
	void Shoot();

	/* RENDERS */
	void ShowEnemies(GLint level, GLfloat shipPos_z); //depending on the level it shows the enemies

	/* VARIABLES */
	GLfloat position[3] = { 0.0, 0.0, 0.0 };
	GLfloat step = 1.5;
	GLfloat stepIdle = 0.8;
	GLfloat stepSinking = 1.f;
	GLfloat goal = 0.f;
	bool isHit = false;
	bool isShooting = false;
	bool isSinking = false;
	bool isAppearing = true;

private:

};

