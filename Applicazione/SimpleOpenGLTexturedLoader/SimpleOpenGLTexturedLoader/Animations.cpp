#include "Animations.h"

extern Animations animations;
extern Render render;
extern Sounds sounds;
extern StateMachine stateMachine;
extern Ship ship;
extern Bullet bullet;
extern Enemy enemy;
extern Score score;

Animations::Animations(void) {}
Animations::~Animations(void) {}

void Animations::AnimationHit(GLint times) {
	static GLfloat counter = 0.f;
	GLfloat max = 2.0 * stateMachine.dt;

	if (firstAnimationHit) {
		counter = stateMachine.dt;
		sounds.PlayHit();
		if (times > 0) {
			ship.PowerDown(); //Rock hit case
			score.RockHit();
		}
		if (times > 1) {
			ship.PowerDown(); //If it's the enemy hitting the ship, then we lose a whole heart, we call PowerDown() twice
			score.EnemyHit();
		}
		firstAnimationHit = false;
	}
	if (counter <= max) {
		animationHit = true;
		// HIT SHIP
		render.recursive_render(render.scene, render.scene->mRootNode->mChildren[13], 1.0);
		counter += counter * stateMachine.dt;
	}
	else {
		counter = stateMachine.dt;
		animationHit = false;
		firstAnimationHit = true;
	}
}

void Animations::AnimationShoot(void) {
	static GLfloat counter = 0;
	static bool first = true;
	GLfloat max = 2.7 * stateMachine.dt;

	if (first) {
		counter = stateMachine.dt;
		sounds.PlayBullet();
		first = false;
		bullet.SetPosition(ship.position[0], ship.position[1], ship.position[2]);
	}
	if (counter <= max) {
		bullet.IncreaseZ();

		// BULLET
		glPushMatrix();
		glTranslatef(bullet.position[0], bullet.position[1], bullet.position[2]);
		render.recursive_render(render.scene, render.scene->mRootNode->mChildren[15], 1.0);
		glPopMatrix();

		counter += counter * stateMachine.dt;
	}
	else {
		counter = stateMachine.dt;
		first = true;
		ship.isShooting = false;
		bullet.SetPosition(0.0, 0.0, 0.0);
	}
}

void Animations::AnimationShootEnemy(GLint index, GLfloat* enemyPos) {
	static GLfloat counter = 0;
	static Bullet bullet;
	static Enemy enemy;
	GLfloat max = 2.7 * stateMachine.dt;

	if (firstAnimationShootEnemy) {
		counter = stateMachine.dt;
		sounds.PlayBullet();
		firstAnimationShootEnemy = false;
		bullet.SetPosition(enemyPos[0], enemyPos[1], enemyPos[2]);

		// BULLET
		glPushMatrix();
		glTranslatef(bullet.position[0], bullet.position[1], bullet.position[2]);
		render.recursive_render(render.scene, render.scene->mRootNode->mChildren[index], 1.0);
		glPopMatrix();
	}
	if (counter <= max) {
		bullet.DecreaseZ();

		/* MANAGE ENEMY BULLET HITTING OUR SHIP */
		if (enemy.IsHitEnemy_Ship(index - 2, bullet.position)) {
			ship.isHitFromEnemy = true;
			cout << "ENEMY___SHIP\n";
		}
		else ship.isHitFromEnemy = false;

		// BULLET
		glPushMatrix();
		glTranslatef(bullet.position[0], bullet.position[1], bullet.position[2]);
		render.recursive_render(render.scene, render.scene->mRootNode->mChildren[index], 1.0);
		glPopMatrix();

		counter += counter * stateMachine.dt;
	}
	else {
		counter = stateMachine.dt;
		firstAnimationShootEnemy = true;
		bullet.SetPosition(enemyPos[0], enemyPos[1], enemyPos[2]);
	}
}

void Animations::AnimationEnemy(GLint index) {
	GLfloat offset = 5.4;
	static Animations animation;

	if (firstAnimationEnemy) {
		sounds.PlayEnemyAppearing(true);
		firstAnimationEnemy = false;
		enemy.SetPosition(offset, 0.0, 0.0); //initialize with some offset towards right
		enemy.DecreaseX();
	}
	else {
		if (enemy.isHit) {
			enemy.isShooting = false;
			enemy.isSinking = true;

			GLfloat posTMP[3] = { enemy.position[0] - offset, enemy.position[1], enemy.position[2] };
			AnimationSinking(index+1, posTMP);
		}
		else {
			GLfloat enemyPosTMP[3] = { enemy.position[0] - offset, enemy.position[1], enemy.position[2] };

			/* MANAGE ENEMY SHIP HITTING OUR SHIP */
			if (enemy.IsHitShip_Ship(index, enemyPosTMP)) {
				sounds.PlayHit();
				cout << "SHIP___SHIP\n";
				score.ShipShip();
				ship.GameOver();
			}
			/*************************************/

			/* MANAGE OUR SHIP HITTING ENEMY SHIP */
			if (enemy.IsHitShip_Enemy(index, bullet.position, enemyPosTMP)) { //if we hit the enemy ship
				sounds.PlayHit();
				cout << "SHIP___ENEMY\n";
				score.EnemyDefeated();
				enemy.isHit = true;
			}
			/*************************************/

			if (enemy.position[0] >= 0.0) { //Move
				enemy.DecreaseX();

				// ENEMY
				glPushMatrix();
				glTranslatef(enemy.position[0] - offset, enemy.position[1], enemy.position[2]);
				render.recursive_render(render.scene, render.scene->mRootNode->mChildren[index], 1.0);
				glPopMatrix();
			}
			else {
				//Stop moving and start shooting
				glPushMatrix();
				glTranslatef(enemy.position[0] - offset, enemy.position[1], enemy.position[2]);
				render.recursive_render(render.scene, render.scene->mRootNode->mChildren[index], 1.0);
				glPopMatrix();

				if (!enemy.isShooting)
					enemy.Shoot();
				else { //is Shooting
					GLfloat posTMP[3] = { enemy.position[0] - offset, enemy.position[1], enemy.position[2] };
					animation.AnimationShootEnemy(index + 2, posTMP);
				}
			}
		}
	}
}

void Animations::AnimationEnemyMoving(GLint index) {
	GLfloat offset = 5.4;
	static Animations animation;

	if (firstAnimationEnemyMoving) {
		animation.firstAnimationShootEnemy = true;
		sounds.PlayEnemyAppearing(true);
		firstAnimationEnemyMoving = false;
		enemy.SetPosition(offset, 0.0, 0.0); //initialize with some offset towards right
		enemy.DecreaseX();
	}
	else {
		if (enemy.isHit) {
			enemy.isShooting = false;
			enemy.isSinking = true;

			GLfloat posTMP[3] = { enemy.position[0] - offset, enemy.position[1], enemy.position[2] };
			AnimationSinking(index + 1, posTMP);
		}
		else {
			GLfloat enemyPosTMP[3] = { enemy.position[0] - offset, enemy.position[1], enemy.position[2] };

			/* MANAGE ENEMY SHIP HITTING OUR SHIP */
			if (enemy.IsHitShip_Ship(index, enemyPosTMP)) {
				sounds.PlayHit();
				cout << "SHIP___SHIP\n";
				score.ShipShip();
				ship.GameOver();
			}
			/*************************************/

			/* MANAGE OUR SHIP HITTING ENEMY SHIP */
			if (enemy.IsHitShip_Enemy(index, bullet.position, enemyPosTMP)) { //if we hit the enemy ship
				sounds.PlayHit();
				cout << "SHIP___ENEMY\n";
				score.EnemyDefeated();
				enemy.isHit = true;
			}
			/*************************************/

			if (enemy.position[0] >= 0.0 && enemy.isAppearing) { //Move
				enemy.DecreaseX();

				if (enemy.position[0]<0.0) enemy.isAppearing = false; //Enemy got the center of the map

				// ENEMY
				glPushMatrix();
				glTranslatef(enemy.position[0] - offset, enemy.position[1], enemy.position[2]);
				render.recursive_render(render.scene, render.scene->mRootNode->mChildren[index], 1.0);
				glPopMatrix();
			}
			else {

				//Enemy Idle Moving
				if (ship.position[0] > 0) {
					if (enemy.position[0] < ship.position[0]) {
						enemy.IncreaseIdleX();
					}		
				}
				else if (ship.position[0] < 0) {
					if (enemy.position[0] > ship.position[0])
						enemy.DecreaseIdleX();
				}
				if (enemy.position[0] > 0) {
					if (ship.position[0] < enemy.position[0]) {
						enemy.DecreaseIdleX();
					}
				}
				else if (enemy.position[0] < 0) {
					if (ship.position[0] > enemy.position[0])
						enemy.IncreaseIdleX();
				}

				//Shoot
				glPushMatrix();
				glTranslatef(enemy.position[0] - offset, enemy.position[1], enemy.position[2]);
				render.recursive_render(render.scene, render.scene->mRootNode->mChildren[index], 1.0);
				glPopMatrix();

				if (!enemy.isShooting)
					enemy.Shoot();
				else { //is Shooting
					GLfloat posTMP[3] = { enemy.position[0] - offset, enemy.position[1], enemy.position[2] };
					animation.AnimationShootEnemy(index + 2, posTMP);
				}
			}
		}
	}
}

void Animations::AnimationSinking(GLint index, GLfloat* enemyPos) {
	static GLfloat counter = stateMachine.dt;
	GLfloat max = 4.5 * stateMachine.dt;
	GLfloat maxStand = 1.5 * stateMachine.dt;

	if (firstAnimationSinking) {
		sounds.PlayEnemySinking();
		counter = stateMachine.dt;
		animationSinking = true;
		enemy.SetPosition(enemyPos[0], enemyPos[1], enemyPos[2]);

		glPushMatrix();
			glTranslatef(enemy.position[0], enemy.position[1], enemy.position[2]);
			render.recursive_render(render.scene, render.scene->mRootNode->mChildren[index], 1.0);//broken enemy
		glPopMatrix();
		firstAnimationSinking = false;
	}
	if (counter <= maxStand && animationSinking) {
		animationSinking = true;

		// ENEMY STANDING
		glPushMatrix();
		glTranslatef(enemy.position[0], enemy.position[1], enemy.position[2]);
		render.recursive_render(render.scene, render.scene->mRootNode->mChildren[index], 1.0);//broken enemy
		glPopMatrix();

		counter += counter * stateMachine.dt;
	}
	else if (counter <= max && animationSinking) {
		animationSinking = true;

		enemy.DecreaseY();

		// ENEMY SINKING
		glPushMatrix();
			glTranslatef(enemy.position[0], enemy.position[1], enemy.position[2]);
			render.recursive_render(render.scene, render.scene->mRootNode->mChildren[index], 1.0);//broken enemy
		glPopMatrix();
		
		counter += counter * stateMachine.dt;
	}
	else {
		counter = stateMachine.dt;
		animationSinking = false;
	}
}