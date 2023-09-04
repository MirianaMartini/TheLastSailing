#include "PowerUp.h"

extern PowerUp powerUp;
extern Render render;
extern Ship ship;
extern StateMachine stateMachine;
extern Rock rock;
extern Sounds sounds;
extern Score score;

PowerUp::PowerUp() {}

PowerUp::PowerUp(int min, int max) {
	GenerateRandomPosition(min, max);
	isPickedUp = false;
}

PowerUp::~PowerUp(void) {}

void PowerUp::RenderPowerUp() {
	angle += 100 * stateMachine.dt;

	if (!IsPickedUp() && !isPickedUp) { //If is not picked Up it is rendered otherwise it is not
		glPushMatrix();
			glTranslatef(position[0], position[1], position[2]);
			glRotatef(angle, 0.f, 1.f, 0.f);
			render.recursive_render(render.scene, render.scene->mRootNode->mChildren[760], 1.0);
		glPopMatrix();
	}
	else {
		if (!isPickedUp) {
			sounds.PlayPowerUp();
			score.PowerUp();
			isPickedUp = true;
		}
	}
}

bool PowerUp::GenerateRandomPosition(int min, int max) {
	int dirty = rand(); //Every time GenerateRandomPosition() calls itself dirty changes
	long int t = static_cast<long int>(time(0)) + static_cast<long int>(dirty*(max-min));

	/* X POSITION */
	GLint _x = (t % 37); //it gets a value between [0, 36]}

	if(_x == 0) position[0] = 0.f;
	if (_x > 18) { //Positive [19, 36]
		_x -= 18;
		position[0] = (float)_x/10.f; // [0.1, 1.8]
	}
	else { //Negative [1, 18]
		position[0] = -(float)_x/10.f; // [-1.8, -0.1]
	}

	/* Y POSITION */
	position[1] = 0.f; // y = 0.f Always

	/* Z POSITION */
	position[2] = -((t % (max-min)) + min);

	/* ROCKS COLLIDING CHECK */
	int index = -1;
	index = ManageObstacleColliding(position);

	if (index >= 0) {
		cout << "RECALCULATE\n";
		return GenerateRandomPosition(min, max); //generate a new dirty
	}
	else return true;

}

int PowerUp::ManageObstacleColliding(GLfloat* pos) {
	int index = -1;
	if (stateMachine.GetCurrentState() == LEVEL_1) {
		switch (stateMachine.randomMap) {
		case 1:
			//ROCKS v1
			index = rock.IsColliding(16, 81, pos); //Indicate indexes of the obstacles to be considered in the level
			break;
		case 2:
			//ROCKS v2
			index = rock.IsColliding(761, 826, pos); //Indicate indexes of the obstacles to be considered in the level
			break;
		case 3:
			//ROCKS v3
			index = rock.IsColliding(827, 893, pos); //Indicate indexes of the obstacles to be considered in the level
			break;
		}
	}
	if (stateMachine.GetCurrentState() == LEVEL_2) {
		switch (stateMachine.randomMap) {
		case 1:
			//ROCKS v1
			index = rock.IsColliding(318, 489, pos); //Indicate indexes of the obstacles to be considered in the level
			break;
		case 2:
			//ROCKS v2
			index = rock.IsColliding(893, 1064, pos); //Indicate indexes of the obstacles to be considered in the level
			break;
		case 3:
			//ROCKS v3
			index = rock.IsColliding(1065, 1236, pos); //Indicate indexes of the obstacles to be considered in the level
			break;
		}
	}
	if (stateMachine.GetCurrentState() == LEVEL_3) {
		switch (stateMachine.randomMap) {
		case 1:
			//ROCKS v1
			index = rock.IsColliding(490, 750, pos); //Indicate indexes of the obstacles to be considered in the level
			break;
		case 2:
			//ROCKS v2
			index = rock.IsColliding(1237, 1497, pos); //Indicate indexes of the obstacles to be considered in the level
			break;
		case 3:
			//ROCKS v3
			index = rock.IsColliding(1498, 1758, pos); //Indicate indexes of the obstacles to be considered in the level
			break;
		}
	}
	return index;
}

bool PowerUp::IsPickedUp() { //If ship picks up the power up
	struct aiMatrix4x4 trafo;
	struct aiVector3D shipMin, shipMax;
	struct aiVector3D powerMin, powerMax;

	/* Calculate hit point of the SHIP */
	shipMin.x = shipMin.y = shipMin.z = 1e10f;
	shipMax.x = shipMax.y = shipMax.z = -1e10f;

	render.get_bounding_box_for_node(render.scene->mRootNode->mChildren[12], &shipMin, &shipMax, &trafo);

	//Add ship movement along z and x
	shipMin.x = shipMin.x + ship.position[0];
	shipMax.x = shipMax.x + ship.position[0];
	shipMin.z = shipMin.z + ship.position[2];
	shipMax.z = shipMax.z + ship.position[2];

	/* Calculate hit point of the POWER UP */
	powerMin.x = powerMin.y = powerMin.z = 1e10f;
	powerMax.x = powerMax.y = powerMax.z = -1e10f;

	render.get_bounding_box_for_node(render.scene->mRootNode->mChildren[760], &powerMin, &powerMax, &trafo);

	//Add ship movement along z and x
	powerMin.x = powerMin.x + position[0];
	powerMax.x = powerMax.x + position[0];
	powerMin.z = powerMin.z + position[2];
	powerMax.z = powerMax.z + position[2];
		
	/* Collision Detection */
	if (((powerMin.x >= shipMin.x && powerMin.x <= shipMax.x) && (powerMin.z >= shipMin.z && powerMin.z <= shipMax.z)) ||
		((powerMax.x >= shipMin.x && powerMax.x <= shipMax.x) && (powerMax.z >= shipMin.z && powerMax.z <= shipMax.z)))
		return true;

	return false;

}