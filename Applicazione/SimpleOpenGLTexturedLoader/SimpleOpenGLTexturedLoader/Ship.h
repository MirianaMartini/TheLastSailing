#pragma once
#include "Settings.h"

class Ship {
public:
	Ship(void);
	~Ship(void);

	/* ACTIONS */
	void InitShip();
	void IdleForwardMovement(); //Ship moves forwards by default
	void MoveLeft();
	void MoveRight();
	void PowerDown(); //it takes -0.5 from life
	void GameOver(); //ship is broken and game ends
	void Shoot();
	void ManageLevelSwitch(int state);
	int ManageObstacleHits();

	/* RENDERS */
	void ShowShip();
	void ShowPath(); //Shows a line under the ship that indicates where the ship is gonna go
	void ShowLife();

	/* VARIABLES */
	float speed = 5; //moving step
	float position[3] = { 0.0, 0.0, 0.0 };
	float old_position[3] = { 0.0, 0.0, 0.0 };
	bool isHit = false;
	bool isHitFromEnemy = false;
	bool isShooting = false;
	bool firstHit = true;
	bool gameOver = false;
	float life = 3.0; //three lives, each hit with rock it takes 0.5, each shoot from enemy boat it takes 0.5, each cruch with enemy
	//boat it takes the whole life leading to death

private:

};

