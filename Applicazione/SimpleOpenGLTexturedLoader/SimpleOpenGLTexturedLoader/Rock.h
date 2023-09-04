#pragma once
#include "Settings.h"

class Rock {
public:
	Rock(void);
	~Rock(void);

	/* ACTIONS */
	GLint IsHit(GLint minI, GLint maxI); //returns the index of the obj that has been hit or -1 otherwise
	GLint IsColliding(GLint minI, GLint maxI, GLfloat* posPowerUp); //returns the index of the obj that has been hit or -1 otherwise

	
private:
};

