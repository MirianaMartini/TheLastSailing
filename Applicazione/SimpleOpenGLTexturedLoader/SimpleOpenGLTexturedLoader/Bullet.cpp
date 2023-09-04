#include "Bullet.h"

extern Bullet bullet;
extern StateMachine stateMachine;

Bullet::Bullet(void) {}
Bullet::~Bullet(void) {}


void Bullet::SetPosition(GLfloat x, GLfloat y, GLfloat z) {
	position[0] = x;
	position[1] = y;
	position[2] = z;
}

void Bullet::IncreaseZ() {
	position[2] -= step * stateMachine.dt;
}

void Bullet::DecreaseZ() {
	position[2] += step * stateMachine.dt;
}


