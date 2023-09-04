#pragma once
#include "Settings.h"

class StateMachine {
public:
	StateMachine(void);
	~StateMachine(void);

	void SetState(int state);
	int GetCurrentState(void);
	void SetOldState(int state);
	int GetOldState(void);
	GLint GenerateRandomMapIndex();
	void ChangeState();

	/* Random Value for Map Spawning */
	GLint randomMap = 1;

	/* State */
	bool changed = true; //flag to say if scene changed or not
	bool changedWindow = true; //flag to say if window got reshaped

	/* Music */
	bool soundtrackStart = true;

	/* Delta Time */
	int t = 0;
	float old_t = 0.0;
	float dt = 0.0; //delta time

private:
	int current_state;
	int old_state;
	int hover_flag = false;

};

