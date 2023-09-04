#include "Ship.h"

extern Ship ship;
extern StateMachine stateMachine;
extern Rock rock;
extern Render render;
extern Mouse mouse;
extern Enemy enemy;
extern Sounds sounds;
extern Score score;
extern File file;

Animations animationShip;

Ship::Ship(void) {}
Ship::~Ship(void) {}

/* ACTIONS */
void Ship::InitShip() {
	ship.position[0] = ship.position[1] = ship.position[2]= 0.0;
	ship.old_position[0] = ship.old_position[1] = ship.old_position[2] = 0.0;
	isHit = false;
	isHitFromEnemy = false;
	isShooting = false;
	firstHit = true;
	gameOver = false;
	animationShip.animationHit = false;
	animationShip.firstAnimationHit = true;
	life = 3.0;

	if (stateMachine.GetCurrentState() == LEVEL_1 || stateMachine.GetCurrentState() == MENU) ship.speed = 5.f;
	if (stateMachine.GetCurrentState() == LEVEL_2) ship.speed = 6.f;
	if (stateMachine.GetCurrentState() == LEVEL_3) ship.speed = 6.5f;
}

void Ship::IdleForwardMovement() {
	int index = -1;

	/* FORWARD MOVEMENT */
	ship.position[2] -= ship.speed * stateMachine.dt;

	/* SCORE */
	score.IdleScore(10.f);

	/* VICTORY */
	ManageLevelSwitch(stateMachine.GetCurrentState());

	/* SHOOTING */ //Shooting is not possible in LEVEL_1
	if (position[2] < -1.f && (stateMachine.GetCurrentState() == LEVEL_2 ||
							   stateMachine.GetCurrentState() == LEVEL_3)) {
		ship.Shoot();
	}

	/* MANAGE OBSTACLES HITS */
	index = ManageObstacleHits();
	
	if (index >= 0) {
		ship.isHit = true;
	}
	else
		ship.isHit = false;
}

int Ship::ManageObstacleHits() {
	int index = -1;
	if (stateMachine.GetCurrentState() == LEVEL_1) {
		switch (stateMachine.randomMap) {
		case 1:
			//ROCKS v1
			index = rock.IsHit(16, 81); //Indicate indexes of the obstacles to be considered in the level
			break;
		case 2:
			//ROCKS v2
			index = rock.IsHit(761, 826); //Indicate indexes of the obstacles to be considered in the level
			break;
		case 3:
			//ROCKS v3
			index = rock.IsHit(827, 893); //Indicate indexes of the obstacles to be considered in the level
			break;
		}
	}
	if (stateMachine.GetCurrentState() == LEVEL_2) {
		switch (stateMachine.randomMap) {
		case 1:
			//ROCKS v1
			index = rock.IsHit(318, 489); //Indicate indexes of the obstacles to be considered in the level
			break;
		case 2:
			//ROCKS v2
			index = rock.IsHit(893, 1064); //Indicate indexes of the obstacles to be considered in the level
			break;
		case 3:
			//ROCKS v3
			index = rock.IsHit(1065, 1236); //Indicate indexes of the obstacles to be considered in the level
			break;
		}
	}
	if (stateMachine.GetCurrentState() == LEVEL_3) {
		switch (stateMachine.randomMap) {
		case 1:
			//ROCKS v1
			index = rock.IsHit(490, 750); //Indicate indexes of the obstacles to be considered in the level
			break;
		case 2:
			//ROCKS v2
			index = rock.IsHit(1237, 1497); //Indicate indexes of the obstacles to be considered in the level
			break;
		case 3:
			//ROCKS v3
			index = rock.IsHit(1498, 1758); //Indicate indexes of the obstacles to be considered in the level
			break;
		}
	}
	return index;
}

void Ship::MoveLeft() {
	position[0] -= ship.speed * stateMachine.dt;
	if (position[0] < -1.895)
		position[0] = -1.895;
}

void Ship::MoveRight() {
	position[0] += ship.speed * stateMachine.dt;
	if (position[0] > 1.9)
		position[0] = 1.9;
}

void Ship::PowerDown() {
	if (ship.life > 0.f) {
		ship.life = ship.life - 0.5;
		//cout << "[LIFE--> " << ship.life << "]\n";
		if (ship.life == 0.0)
			ship.GameOver();
	}
}

void Ship::GameOver() {
	//The ship breaks
	//It shows the GameOver Window
	ship.life = 0.0;
	ship.gameOver = true;
	stateMachine.SetOldState(stateMachine.GetCurrentState());
	stateMachine.SetState(GAMEOVER);
	sounds.PlayGameOver(true);
	cout << "GAME OVER";
}

void Ship::Shoot() {
	if (mouse.left_down) {
		cout << "__SHOOT__";
		if(!ship.isShooting) ship.isShooting = true;
	}
}

void Ship::ManageLevelSwitch(int state) {
	switch (state) {
	case LEVEL_1:
		if (position[2] < -353.0 && !ship.gameOver) {
			//file.SavePointsInFile(state, score.score);
			stateMachine.SetOldState(stateMachine.GetCurrentState());
			stateMachine.SetState(stateMachine.GetCurrentState() + 1); //Goes to VICTORY_1 state
			stateMachine.changed = true;
			render.first = true;
			sounds.PlayVictory(true);
		}
		break;
	case LEVEL_2:
		if (position[2] < -353.0 - LEVEL_2_ISLAND_OFFSET && !ship.gameOver) {
			//file.SavePointsInFile(state, score.score);
			stateMachine.SetOldState(stateMachine.GetCurrentState());
			stateMachine.SetState(stateMachine.GetCurrentState() + 1); //Goes to VICTORY_2 state
			stateMachine.changed = true;
			render.first = true;
			sounds.PlayVictory(true);
		}
		break;
	case LEVEL_3:
		if (position[2] < -353.0 - LEVEL_3_ISLAND_OFFSET && !ship.gameOver) {
			//file.SavePointsInFile(state, score.score);
			stateMachine.SetOldState(stateMachine.GetCurrentState());
			stateMachine.SetState(stateMachine.GetCurrentState() + 1); //Goes to VICTORY_3 state
			stateMachine.changed = true;
			render.first = true;
			sounds.PlayVictory(true);
		}
		break;
	}
}



/* RENDERS */

void Ship::ShowShip() {
	if (ship.gameOver) {
		render.recursive_render(render.scene, render.scene->mRootNode->mChildren[14], 1.0); //Broken ship
	}
	else {
		if ((ship.isHit || isHitFromEnemy) && ship.firstHit) { //triggers just the first hit
			ship.firstHit = false;
			if (ship.isHit) animationShip.AnimationHit(1); //Ship loses half heart
			if (ship.isHitFromEnemy) animationShip.AnimationHit(2); //Ship loses 1 heart
		}
		else if (animationShip.animationHit) //if the animations is still playing
			animationShip.AnimationHit(0); //It triggers to continue the animation, it's not first call
		else { //if animation ended and !isHit
			ship.firstHit = true;
			render.recursive_render(render.scene, render.scene->mRootNode->mChildren[12], 1.0);
		}
	}
}

void Ship::ShowPath() {
	//TODO: Sostituire con un piano importato da blender con annessa texture

	/*
	glBegin(GL_POLYGON);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-0.1f, 0.01f, -150.0f);
		glVertex3f(-0.1f, 0.01f, 1.0f);
		glVertex3f(0.1f, 0.01f, 1.0f);
		glVertex3f(0.1f, 0.01f, -150.0f);
	glEnd();
	*/

	glBegin(GL_LINES);
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.f, 0.01f, -1000.0f);
	glVertex3f(0.f, 0.01f, 1000.0f);
	glEnd();
}

void Ship::ShowLife() {
	//Renders the three hearts depending on the current state of life
	if (ship.life == 0) {}
	else if (ship.life > 0)
		render.recursive_render(render.scene, render.scene->mRootNode->mChildren[4], 1.0);
	if (ship.life > 0.5)
		render.recursive_render(render.scene, render.scene->mRootNode->mChildren[5], 1.0);
	if (ship.life > 1.0)
		render.recursive_render(render.scene, render.scene->mRootNode->mChildren[6], 1.0);
	if (ship.life > 1.5)
		render.recursive_render(render.scene, render.scene->mRootNode->mChildren[7], 1.0);
	if (ship.life > 2.0)
		render.recursive_render(render.scene, render.scene->mRootNode->mChildren[8], 1.0);
	if (ship.life > 2.5)
		render.recursive_render(render.scene, render.scene->mRootNode->mChildren[9], 1.0);
}