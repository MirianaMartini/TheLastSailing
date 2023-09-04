#pragma once
#include "Settings.h"

class Score {
public:
	void* font = GLUT_BITMAP_TIMES_ROMAN_24;
	GLint score = 0;
	GLint oldScore = 0;

	/* Mothods for Points */
	void SaveScore(GLint level);
	void InitScore();
	void IdleScore(GLfloat interval);
	void RockHit();
	void EnemyHit();
	void ShipShip();
	void EnemyDefeated();
	void PowerUp();

	/* Renders */
	void RenderScore();
	void RenderLeaderboard();
	void RenderObtainedScore(int level);

private:
	void Output(int x, int y, std::string str, GLfloat RGB[3]);
	void DrawSquare(int x, int y);
};

