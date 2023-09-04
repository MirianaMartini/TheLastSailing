#pragma once
#include "Settings.h"

class Mouse {
public:
	Mouse();
	~Mouse();
	float pixel_x, pixel_y;
	bool left_down, right_down;

	void SetMousePixelPosition(int x, int y);
	void GetMouseState(int button, int state, int x, int y);

	/* ACTIONS */
	bool StartPressed();
	bool StartHover();

	bool RulesPressed();
	bool RulesHover();

	bool CommandPressed();
	bool CommandHover();

	bool BackToMenuRulesPressed();
	bool BackToMenuRulesHover();

	bool BackToMenuCommandsPressed();
	bool BackToMenuCommandsHover();

	bool NextLevelPressed();
	bool NextLevelHover();

	bool BackToMenuVictoryPressed();
	bool BackToMenuVictoryHover();

	bool TryAgainPressed();
	bool TryAgainHover();

	bool BackToMenuGameoverPressed();
	bool BackToMenuGameoverHover();

private:

};