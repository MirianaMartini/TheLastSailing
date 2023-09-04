#pragma once
#include "Settings.h"

class PowerUp {
public:
	PowerUp();
	PowerUp(int min, int max);
	~PowerUp(void);

	GLfloat angle = 1;
	GLfloat position[3] = { 0.0, 0.0, 0.0 }; //Randomly Generated
	bool isPickedUp = false;

	void RenderPowerUp();
	bool GenerateRandomPosition(int min, int max); //sets position[] array
	bool IsPickedUp();
	
private:
	int ManageObstacleColliding(GLfloat* pos);
};

