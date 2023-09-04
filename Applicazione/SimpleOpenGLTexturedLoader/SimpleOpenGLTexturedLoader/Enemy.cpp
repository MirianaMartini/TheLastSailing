#include "Enemy.h"

extern Render render;
extern StateMachine stateMachine;
extern Ship ship;
extern Animations animations;
extern Enemy enemy;
extern Sounds sounds;

Enemy::Enemy(void) {}
Enemy::~Enemy(void) {}

void Enemy::InitEnemy() {
	enemy.position[0] = enemy.position[1] = enemy.position[2] = 0.0;
	animations.firstAnimationEnemy = true;
	animations.firstAnimationEnemyMoving = true;
	animations.firstAnimationSinking = true;
	animations.animationSinking = false;
	animations.firstAnimationShootEnemy = true;
	enemy.isHit = false;
	enemy.isShooting = false;
	enemy.isSinking = false;
	enemy.isAppearing = true;
}

void Enemy::SetPosition(GLfloat x, GLfloat y, GLfloat z) {
	position[0] = x;
	position[1] = y;
	position[2] = z;
}

void Enemy::DecreaseX() {
	position[0] -= step * stateMachine.dt;
}

void Enemy::IncreaseX() {
	position[0] += step * stateMachine.dt;
}

void Enemy::DecreaseIdleX() {
	position[0] -= stepIdle * stateMachine.dt;
	if (position[0] < -1.89)
		position[0] = -1.89;
}

void Enemy::IncreaseIdleX() {
	position[0] += stepIdle * stateMachine.dt;
	if (position[0] > 1.85)
		position[0] = 1.85;
}

void Enemy::DecreaseY() {
	position[1] -= stepSinking * stateMachine.dt;
}

/* ACTIONS */
bool Enemy::IsHitShip_Ship(GLint enemyIndex, GLfloat* enemyPos) {
	struct aiMatrix4x4 trafo;
	struct aiVector3D min, max;
	struct aiVector3D shipMin, shipMax;

	/* Calculate hit point of our ship */
	shipMin.x = shipMin.y = shipMin.z = 1e10f;
	shipMax.x = shipMax.y = shipMax.z = -1e10f;

	render.get_bounding_box_for_node(render.scene->mRootNode->mChildren[12], &shipMin, &shipMax, &trafo);

	//Add ship movement along z and x
	shipMin.x = shipMin.x + ship.position[0];
	shipMax.x = shipMax.x + ship.position[0];
	shipMin.z = shipMin.z + ship.position[2];
	shipMax.z = shipMax.z + ship.position[2];

	/* Calculate hit point of the enemy ship */
	min.x = min.y = min.z = 1e10f;
	max.x = max.y = max.z = -1e10f;

	render.get_bounding_box_for_node(render.scene->mRootNode->mChildren[enemyIndex], &min, &max, &trafo);

	//Add enemy ship movement along z and x
	min.x = min.x + enemyPos[0];
	max.x = max.x + enemyPos[0];
	min.z = min.z + enemyPos[2];
	max.z = max.z + enemyPos[2];

	//Collision Detection
	if (((shipMin.x >= min.x && shipMin.x <= max.x) && (shipMin.z >= min.z && shipMin.z <= max.z)) ||
		((shipMax.x >= min.x && shipMax.x <= max.x) && (shipMax.z >= min.z && shipMax.z <= max.z)))
		return true; //ship-ship hit

	return false;
}

bool Enemy::IsHitShip_Enemy(GLint enemyIndex, GLfloat* bulletPos, GLfloat* enemyPos) {
	struct aiMatrix4x4 trafo;
	struct aiVector3D min, max;
	struct aiVector3D bulletMin, bulletMax;

	/* Calculate hit point of the bullet of our ship */
	bulletMin.x = bulletMin.y = bulletMin.z = 1e10f;
	bulletMax.x = bulletMax.y = bulletMax.z = -1e10f;

	render.get_bounding_box_for_node(render.scene->mRootNode->mChildren[15], &bulletMin, &bulletMax, &trafo);

	//Add ship movement along z and x
	bulletMin.x = bulletMin.x + bulletPos[0];
	bulletMax.x = bulletMax.x + bulletPos[0];
	bulletMin.z = bulletMin.z + bulletPos[2];
	bulletMax.z = bulletMax.z + bulletPos[2];

	/* Calculate hit point of the enemy ship */
	min.x = min.y = min.z = 1e10f;
	max.x = max.y = max.z = -1e10f;

	render.get_bounding_box_for_node(render.scene->mRootNode->mChildren[enemyIndex], &min, &max, &trafo);

	min.x = min.x + enemyPos[0];
	max.x = max.x + enemyPos[0];
	min.z = min.z + enemyPos[2];
	max.z = max.z + enemyPos[2];

	//Collision Detection
	if (((bulletMin.x >= min.x && bulletMin.x <= max.x) && (bulletMin.z >= min.z && bulletMin.z <= max.z)) ||
		((bulletMax.x >= min.x && bulletMax.x <= max.x) && (bulletMax.z >= min.z && bulletMax.z <= max.z)))
		return true; //bullet-enemy hit

	return false;
}

bool Enemy::IsHitEnemy_Ship(GLint enemyIndex, GLfloat* bulletPos) {
	struct aiMatrix4x4 trafo;
	struct aiVector3D shipMin, shipMax;
	struct aiVector3D bulletMin, bulletMax;

	/* Calculate hit point of the bullet of the enemy ship */
	bulletMin.x = bulletMin.y = bulletMin.z = 1e10f;
	bulletMax.x = bulletMax.y = bulletMax.z = -1e10f;

	render.get_bounding_box_for_node(render.scene->mRootNode->mChildren[enemyIndex + 2], &bulletMin, &bulletMax, &trafo);

	//Add ship movement along z and x
	bulletMin.x = bulletMin.x + bulletPos[0];
	bulletMax.x = bulletMax.x + bulletPos[0];
	bulletMin.z = bulletMin.z + bulletPos[2];
	bulletMax.z = bulletMax.z + bulletPos[2];

	/* Calculate hit point of our ship */
	shipMin.x = shipMin.y = shipMin.z = 1e10f;
	shipMax.x = shipMax.y = shipMax.z = -1e10f;

	render.get_bounding_box_for_node(render.scene->mRootNode->mChildren[12], &shipMin, &shipMax, &trafo);

	//Add ship movement along z and x
	shipMin.x = shipMin.x + ship.position[0];
	shipMax.x = shipMax.x + ship.position[0];
	shipMin.z = shipMin.z + ship.position[2];
	shipMax.z = shipMax.z + ship.position[2];

	//Collision Detection
	if (((bulletMin.x >= shipMin.x && bulletMin.x <= shipMax.x) && (bulletMin.z >= shipMin.z && bulletMin.z <= shipMax.z)) ||
		((bulletMax.x >= shipMin.x && bulletMax.x <= shipMax.x) && (bulletMax.z >= shipMin.z && bulletMax.z <= shipMax.z)))
		return true; //bullet-enemy hit

	return false;
}

void Enemy::Shoot() {
	cout << "__ENEMY_SHOOT__";
	animations.firstAnimationShootEnemy = true;
	isShooting = true;
}

/* RENDERS */
void Enemy::ShowEnemies(GLint level, GLfloat shipPos_z) {
	switch (level) {
	case LEVEL_1: 
		//NO enemies in LEVEL_1
		enemy.InitEnemy();
		break;
	case LEVEL_2:
		// ENEMY 1
		if (shipPos_z < -65.0 && shipPos_z > -125.0) {
			animations.AnimationEnemy(82);
		}
		else if (shipPos_z < -125.0 && shipPos_z > -126.0){
			InitEnemy();
		}

		// ENEMY 2
		if (shipPos_z < -185.0 && shipPos_z > -245.0) {
			animations.AnimationEnemy(751);
		}
		else if (shipPos_z < -245.0 && shipPos_z > -246.0) {
			InitEnemy();
		}

		break;
	case LEVEL_3:
		//cout << "Z----> " << shipPos_z;
		// 
		// ENEMY 1
		if (shipPos_z < -65.0 && shipPos_z > -125.0) {
			animations.AnimationEnemyMoving(82);
		}
		else if (shipPos_z < -125.0 && shipPos_z > -126.0) {
			InitEnemy();
		}

		// ENEMY 2
		if (shipPos_z < -127.0 && shipPos_z > -185.0) {
			animations.AnimationEnemyMoving(754);
		}
		else if (shipPos_z < -185.0 && shipPos_z > -186.0) {
			InitEnemy();
		}

		// ENEMY 3
		if (shipPos_z < -187.0 && shipPos_z > -245.0) {
			animations.AnimationEnemyMoving(751);
		}
		else if (shipPos_z < -245.0 && shipPos_z > -246.0) {
			InitEnemy();
		}

		// ENEMY 4
		if (shipPos_z < -275.0 && shipPos_z > -335.0) {
			animations.AnimationEnemyMoving(757);
		}
		else if (shipPos_z < -335.0 && shipPos_z > -336.0) {
			InitEnemy();
		}
		break;
	case GAMEOVER:
		InitEnemy();
		break;
	default:
		break;
	}
}
