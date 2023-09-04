#include "Rock.h"

extern Rock rock;
extern Render render;
extern Ship ship;

Rock::Rock(void){}
Rock::~Rock(void){}

/* ACTIONS */
GLint Rock::IsHit(GLint minI, GLint maxI) { //min-max indexes of rocks
	struct aiMatrix4x4 trafo;
	struct aiVector3D min, max;
	struct aiVector3D shipMin, shipMax;

	/* Calculate hit point of the ship */
	shipMin.x = shipMin.y = shipMin.z = 1e10f;
	shipMax.x = shipMax.y = shipMax.z = -1e10f;

	render.get_bounding_box_for_node(render.scene->mRootNode->mChildren[12], &shipMin, &shipMax, &trafo);
	
	//Add ship movement along z and x
	shipMin.x = shipMin.x + ship.position[0];
	shipMax.x = shipMax.x + ship.position[0];
	shipMin.z = shipMin.z + ship.position[2];
	shipMax.z = shipMax.z + ship.position[2];

	for (int i = minI; i <= maxI; ++i) {
		
		/* Calculate hit point of the rocks */
		min.x = min.y = min.z = 1e10f;
		max.x = max.y = max.z = -1e10f;

		render.get_bounding_box_for_node(render.scene->mRootNode->mChildren[i], &min, &max, &trafo);
		
		//Collision Detection
		if (((shipMin.x >= min.x && shipMin.x <= max.x) && (shipMin.z >= min.z && shipMin.z <= max.z)) || 
			((shipMax.x >= min.x && shipMax.x <= max.x) && (shipMax.z >= min.z && shipMax.z <= max.z))) 
			return i;
	}
	
	return -1;
}

GLint Rock::IsColliding(GLint minI, GLint maxI, GLfloat* posPowerUp) { //min-max indexes of rocks
	struct aiMatrix4x4 trafo;
	struct aiVector3D min, max;
	struct aiVector3D powerMin, powerMax;

	/* Calculate hit point of the powerUp */
	powerMin.x = powerMin.y = powerMin.z = 1e10f;
	powerMax.x = powerMax.y = powerMax.z = -1e10f;

	render.get_bounding_box_for_node(render.scene->mRootNode->mChildren[760], &powerMin, &powerMax, &trafo);

	//Add powerUps offsets along z and x
	powerMin.x = powerMin.x + posPowerUp[0];
	powerMax.x = powerMax.x + posPowerUp[0];
	powerMin.z = powerMin.z + posPowerUp[2];
	powerMax.z = powerMax.z + posPowerUp[2];

	for (int i = minI; i <= maxI; ++i) {

		/* Calculate hit point of the rocks */
		min.x = min.y = min.z = 1e10f;
		max.x = max.y = max.z = -1e10f;

		render.get_bounding_box_for_node(render.scene->mRootNode->mChildren[i], &min, &max, &trafo);

		//Collision Detection
		if (((powerMin.x >= min.x && powerMin.x <= max.x) && (powerMin.z >= min.z && powerMin.z <= max.z)) ||
			((powerMax.x >= min.x && powerMax.x <= max.x) && (powerMax.z >= min.z && powerMax.z <= max.z)))
			return i;
	}

	return -1;
}