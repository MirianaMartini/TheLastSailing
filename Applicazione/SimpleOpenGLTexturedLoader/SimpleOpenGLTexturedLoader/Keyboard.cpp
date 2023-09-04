#include "Keyboard.h"

extern Keyboard keyboard;
extern Ship ship;
extern StateMachine stateMachine;

Keyboard::Keyboard(void) {}
Keyboard::~Keyboard(void) {}

void Keyboard::KeyboardBehaviour(unsigned char key, int x, int y) {
	switch (key) {
	/*
	case 27:
		exit(1);
		break;
	*/
	case 'A': // Move Left
	case 'a':
		if(stateMachine.GetCurrentState() == LEVEL_1 || stateMachine.GetCurrentState() == LEVEL_2 || stateMachine.GetCurrentState() == LEVEL_3)
			ship.MoveLeft();
		break;
	case 'D': //Move Right
	case 'd':
		if (stateMachine.GetCurrentState() == LEVEL_1 || stateMachine.GetCurrentState() == LEVEL_2 || stateMachine.GetCurrentState() == LEVEL_3)
			ship.MoveRight();
		break;
	default:
		break;
	}
}
