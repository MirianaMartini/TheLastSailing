#include "Score.h"

extern Score score;
extern Ship ship;
extern StateMachine stateMachine;
extern File file;

void Score::InitScore() {
	score = 0;
}

void Score::SaveScore(GLint level) {
	file.SavePointsInFile(level, score);
	score = 0;
}

void Score::IdleScore(GLfloat interval) {
	if (ship.old_position[2] - ship.position[2] >= interval) {
		ship.old_position[2] = ship.position[2];
		score += 10; //Every interval Ship gets 10 points
		cout << "SCORE: " << score << endl;
	}
}

void Score::RockHit() {
	score -= 20; 
}

void Score::EnemyHit() {
	score -= 50;
}

void Score::ShipShip() {
	score -= 100;
}

void Score::EnemyDefeated() {
	score += 100;
}

void Score::PowerUp() {
	score += 200; //Score
	if (ship.life < 3.f) ship.life += 0.5f; //Life
}

void Score::RenderScore() {
	stringstream stream;
	stream << score;

	string str;
	stream >> str;

	str = "SCORE: " + str;

	GLfloat RGB[3] = { 1.0f, 0.0f, 0.5f };
	Output(SW - 250, 50, str, RGB);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void Score::RenderLeaderboard() {
	DrawSquare(10, 420);

	GLfloat RGB[3] = { 1.0f, 0.3f, 0.0f };
	Output(20, 450, "Best Scores", RGB);
		
	RGB[0] = 0.2f;
	RGB[1] = 0.2f;

	std::string scr = file.GetBestScore(LEVEL_1);
	Output(20, 500, "Level 1: " + scr + "/750", RGB);

	scr = file.GetBestScore(LEVEL_2);
	Output(20, 540, "Level 2: " + scr + "/1430", RGB);

	scr = file.GetBestScore(LEVEL_3);
	Output(20, 580, "Level 3: " + scr + "/2100", RGB);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void Score::RenderObtainedScore(int level) {
	GLfloat RGB[3] = { 1.0f, 0.0f, 0.0f };
	std::string tmp;

	oldScore = std::stof(file.GetBestScore(level));

	if(score > oldScore) tmp = "NEW RECORD: " + std::to_string(score);
	else tmp = "YOUR SCORE: " + std::to_string(score);

	Output(SW - 350, 50, tmp, RGB);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void Score::Output(int x, int y, std::string str, GLfloat RGB[3])
{
	int len, i;

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); 
	glLoadIdentity();

	gluOrtho2D(0, SW, SH, 0);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_LIGHTING);

	glColor3f(RGB[0], RGB[1], RGB[2]);
	glRasterPos2f(x, y);
	len = str.length();
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(font, str[i]);
	}

	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Score::DrawSquare(int x, int y)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(0, SW, SH, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_LIGHTING);

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
		glVertex3f(x, y, 0.0);
		glVertex3f(x + 260, y, 0.0);
		glVertex3f(x + 260, y + 180, 0.0);
		glVertex3f(x, y + 180, 0.0);
	glEnd();

	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}