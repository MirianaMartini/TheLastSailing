#pragma once
#include "Settings.h"

class Bullet {
public:
	Bullet(void);
	~Bullet(void);

	void SetPosition(GLfloat x, GLfloat y, GLfloat z);
	void IncreaseZ();
	void DecreaseZ();

	/* VARIABLES */
	GLfloat position[3] = {0.0, 0.0, 0.0};
	GLfloat step = 20;

private:
};

