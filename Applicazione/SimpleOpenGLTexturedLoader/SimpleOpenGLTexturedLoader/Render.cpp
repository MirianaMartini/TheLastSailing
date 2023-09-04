#include "Render.h"

extern StateMachine stateMachine;
extern Mouse mouse;
extern Render render;
extern Ship ship;
extern Rock rock;
extern Score score;

Enemy enemyShip;
Animations animationBullet;

PowerUp* powerUp1;
PowerUp* powerUp2;
PowerUp* powerUp3;
PowerUp* powerUp4;
PowerUp* powerUp5;
PowerUp* powerUp6;

Render::Render(void) {}
Render::~Render(void) {}

#pragma region OpenGLUtilities

void Render::reshape(int width, int height) {
	const double aspectRatio = (float)width / height, fieldOfView = 45.0;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fieldOfView, aspectRatio, 1.0, 1000.0);  // Znear and Zfar 
	glViewport(0, 0, width, height);

	stateMachine.changedWindow = true;
}

void Render::get_bounding_box_for_node(const struct aiNode* nd, struct aiVector3D* min, struct aiVector3D* max, struct aiMatrix4x4* trafo) {
	struct aiMatrix4x4 prev;
	unsigned int n = 0, t;

	prev = *trafo;
	aiMultiplyMatrix4(trafo, &nd->mTransformation);

	for (; n < nd->mNumMeshes; ++n) {
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t) {

			struct aiVector3D tmp = mesh->mVertices[t];
			aiTransformVecByMatrix4(&tmp, trafo);

			min->x = aisgl_min(min->x, tmp.x);
			min->y = aisgl_min(min->y, tmp.y);
			min->z = aisgl_min(min->z, tmp.z);

			max->x = aisgl_max(max->x, tmp.x);
			max->y = aisgl_max(max->y, tmp.y);
			max->z = aisgl_max(max->z, tmp.z);
		}
	}

	for (n = 0; n < nd->mNumChildren; ++n) {
		get_bounding_box_for_node(nd->mChildren[n], min, max, trafo);
	}
	*trafo = prev;
}

void Render::get_bounding_box(struct aiVector3D* min, struct aiVector3D* max) {
	struct aiMatrix4x4 trafo;
	aiIdentityMatrix4(&trafo);

	min->x = min->y = min->z = 1e10f;
	max->x = max->y = max->z = -1e10f;
	get_bounding_box_for_node(scene->mRootNode, min, max, &trafo);
}

void Render::color4_to_float4(const struct aiColor4D* c, float f[4]) {
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

void Render::set_float4(float f[4], float a, float b, float c, float d) {
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}

void Render::color4f(const struct aiColor4D* color) {
	// Can't send color down as a pointer to aiColor4D because AI colors are ABGR.
	glColor4f(color->r, color->g, color->b, color->a);
}

void Render::apply_material(const struct aiMaterial* mtl) {
	float c[4];

	GLenum fill_mode;
	int ret1, ret2;
	struct aiColor4D diffuse;
	struct aiColor4D specular;
	struct aiColor4D ambient;
	struct aiColor4D emission;
	float shininess, strength;
	int two_sided;
	int wireframe;
	int max;

	int texIndex = 0;
	aiString texPath;	//contains filename of texture

	if (AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath)) {
		//bind texture
		unsigned int texId = *textureIdMap[texPath.data];
		glBindTexture(GL_TEXTURE_2D, texId);
	}

	set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		color4_to_float4(&diffuse, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
		color4_to_float4(&specular, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

	set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
		color4_to_float4(&ambient, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
		color4_to_float4(&emission, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

	max = 1;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, (unsigned int*)&max);
	max = 1;
	ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, (unsigned int*)&max);
	if ((ret1 == AI_SUCCESS) && (ret2 == AI_SUCCESS))
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
	else {
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	}

	max = 1;
	if (AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, (unsigned int*)&max))
		fill_mode = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

	max = 1;
	if ((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, (unsigned int*)&max)) && two_sided)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
}

void Render::recursive_render(const struct aiScene* sc, const struct aiNode* nd, float scale) {
	unsigned int i;
	unsigned int n = 0, t;
	struct aiMatrix4x4 m = nd->mTransformation;

	//printf("Node name: %s\n", nd->mName.data);

	//m.Scaling(aiVector3D(scale, scale, scale), m);

	// update transform
	m.Transpose();
	glPushMatrix();
	glMultMatrixf((float*)&m);

	// draw all meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n) {
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];

		///
		//printf("Drawing MESH with this name: %s\n", mesh->mName.data);

		apply_material(sc->mMaterials[mesh->mMaterialIndex]);


		if (mesh->HasTextureCoords(0))
			glEnable(GL_TEXTURE_2D);
		else
			glDisable(GL_TEXTURE_2D);

		if (mesh->mNormals == NULL)
			glDisable(GL_LIGHTING);
		else
			glEnable(GL_LIGHTING);

		if (mesh->mColors[0] != NULL)
			glEnable(GL_COLOR_MATERIAL);
		else
			glDisable(GL_COLOR_MATERIAL);

		for (t = 0; t < mesh->mNumFaces; ++t) {
			const struct aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;

			switch (face->mNumIndices) {
			case 1: face_mode = GL_POINTS; break;
			case 2: face_mode = GL_LINES; break;
			case 3: face_mode = GL_TRIANGLES; break;
			default: face_mode = GL_POLYGON; break;
			}

			glBegin(face_mode);

			for (i = 0; i < face->mNumIndices; i++) {		// go through all vertices in face
				int vertexIndex = face->mIndices[i];	// get group index for current index
				if (mesh->mColors[0] != NULL)
					color4f(&mesh->mColors[0][vertexIndex]);
				if (mesh->mNormals != NULL)

					if (mesh->HasTextureCoords(0)) {		//HasTextureCoords(texture_coordinates_set)
						glTexCoord2f(mesh->mTextureCoords[0][vertexIndex].x, 1 - mesh->mTextureCoords[0][vertexIndex].y); //mTextureCoords[channel][vertex]
					}

				glNormal3fv(&mesh->mNormals[vertexIndex].x);
				glVertex3fv(&mesh->mVertices[vertexIndex].x);
			}
			glEnd();
		}
	}

	// draw all children
	for (n = 0; n < nd->mNumChildren; ++n) {
		recursive_render(sc, nd->mChildren[n], scale);
	}

	glPopMatrix();
}

int Render::loadasset(const char* path) {
	// we are taking one of the postprocessing presets to avoid
	// writing 20 single postprocessing flags here.
	scene = aiImportFile(path, aiProcessPreset_TargetRealtime_Quality);

	if (scene) {
		get_bounding_box(&scene_min, &scene_max);
		scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
		scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
		scene_center.z = (scene_min.z + scene_max.z) / 2.0f;
		return 0;
	}

	return 1;
}
#pragma endregion

#pragma region Utilities

void Render::ShowStars(void) {
	//STARS BORDER
	for (int i = 297; i <= 302; ++i)
		recursive_render(scene, scene->mRootNode->mChildren[i], 1.0);

	//STARS --- Renders the three stars depending on the current state of life
	if (ship.life == 0) {}
	else if (ship.life > 0)
		render.recursive_render(render.scene, render.scene->mRootNode->mChildren[291], 1.0);
	if (ship.life > 0.5)
		render.recursive_render(render.scene, render.scene->mRootNode->mChildren[292], 1.0);
	if (ship.life > 1.0)
		render.recursive_render(render.scene, render.scene->mRootNode->mChildren[293], 1.0);
	if (ship.life > 1.5)
		render.recursive_render(render.scene, render.scene->mRootNode->mChildren[294], 1.0);
	if (ship.life > 2.0)
		render.recursive_render(render.scene, render.scene->mRootNode->mChildren[295], 1.0);
	if (ship.life > 2.5)
		render.recursive_render(render.scene, render.scene->mRootNode->mChildren[296], 1.0);
}

void Render::ShowIsland(void) {
	int current_state = stateMachine.GetCurrentState();

	switch (current_state) {
	case LEVEL_1:
		recursive_render(scene, scene->mRootNode->mChildren[85], 1.0);
		break;
	case LEVEL_2:
		glPushMatrix();
		glTranslatef(0.f, 0.f, -LEVEL_2_ISLAND_OFFSET);
		recursive_render(scene, scene->mRootNode->mChildren[85], 1.0);
		glPopMatrix();
		break;
	case LEVEL_3:
		glPushMatrix();
		glTranslatef(0.f, 0.f, -LEVEL_3_ISLAND_OFFSET);
		recursive_render(scene, scene->mRootNode->mChildren[85], 1.0);
		glPopMatrix();
		break;
	}
}

void Render::ShowRocks(int level) {
	switch (level) {
		case LEVEL_1:
			switch (stateMachine.randomMap) {
				case 1:
					//ROCKS v1
					for (int i = 16; i < 82; ++i)
						recursive_render(scene, scene->mRootNode->mChildren[i], 1.0);
					break;
				case 2:
					//ROCKS v2
					for (int i = 761; i < 827; ++i)
						recursive_render(scene, scene->mRootNode->mChildren[i], 1.0);
					break;
				case 3:
					//ROCKS v3
					for (int i = 827; i < 893; ++i)
						recursive_render(scene, scene->mRootNode->mChildren[i], 1.0);
					break;
			}
			break;
		case LEVEL_2:
			switch (stateMachine.randomMap) {
			case 1:
				//ROCKS v1
				for (int i = 318; i < 490; ++i)
					recursive_render(scene, scene->mRootNode->mChildren[i], 1.0);
				break;
			case 2:
				//ROCKS v2
				for (int i = 893; i < 1065; ++i)
					recursive_render(scene, scene->mRootNode->mChildren[i], 1.0);
				break;
			case 3:
				//ROCKS v3
				for (int i = 1065; i < 1237; ++i)
					recursive_render(scene, scene->mRootNode->mChildren[i], 1.0);
				break;
			}
			break;
		case LEVEL_3:
			switch (stateMachine.randomMap) {
			case 1:
				//ROCKS v1
				for (int i = 490; i < 751; ++i)
					recursive_render(scene, scene->mRootNode->mChildren[i], 1.0);
				break;
			case 2:
				//ROCKS v2
				for (int i = 1237; i < 1498; ++i)
					recursive_render(scene, scene->mRootNode->mChildren[i], 1.0);
				break;
			case 3:
				//ROCKS v3
				for (int i = 1498; i < 1759; ++i)
					recursive_render(scene, scene->mRootNode->mChildren[i], 1.0);
				break;
			}
			break;
	}
}

#pragma endregion


void Render::Menu(void) {
	// current rotation angle
	static float angle = 0.f;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.f, 0.f, 3.f, 0.f, 0.f, -5.f, 0.f, 1.f, 0.f);

	//BEST SCORES
	score.RenderLeaderboard();

	// rotate it around the y axis
	glRotatef(angle, 0.f, 1.f, 0.f);

	//glScalef(tmp, tmp, tmp);
	glScalef(0.7, 0.7, 0.7);

	// center the model
	glTranslatef(-scene_center.x, -scene_center.y, -scene_center.z);

	recursive_render(scene, scene->mRootNode->mChildren[3], 1.0); //Background Image

	if (mouse.StartHover()) recursive_render(scene, scene->mRootNode->mChildren[273], 1.0); //StartHover Button
	else recursive_render(scene, scene->mRootNode->mChildren[0], 1.0); //Start Button

	if (mouse.RulesHover()) recursive_render(scene, scene->mRootNode->mChildren[274], 1.0); //RulestHover Button
	else recursive_render(scene, scene->mRootNode->mChildren[1], 1.0); //Rules Button

	if (mouse.CommandHover()) recursive_render(scene, scene->mRootNode->mChildren[275], 1.0); //CommandHover Button
	else recursive_render(scene, scene->mRootNode->mChildren[2], 1.0); //Command Button
}

void Render::Rules(void) {
	// current rotation angle
	static float angle = 0.f;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.f, 0.f, 3.f, 0.f, 0.f, -5.f, 0.f, 1.f, 0.f);

	// rotate it around the y axis
	glRotatef(angle, 0.f, 1.f, 0.f);

	//glScalef(tmp, tmp, tmp);
	glScalef(0.7, 0.7, 0.7);

	// center the model
	glTranslatef(-scene_center.x, -scene_center.y, -scene_center.z);

	recursive_render(scene, scene->mRootNode->mChildren[308], 1.0); //Background Image
	recursive_render(scene, scene->mRootNode->mChildren[305], 1.0); //Title
	recursive_render(scene, scene->mRootNode->mChildren[306], 1.0); //Text
	if (mouse.BackToMenuRulesHover()) recursive_render(scene, scene->mRootNode->mChildren[309], 1.0); //Back To Menu HOVER
	else recursive_render(scene, scene->mRootNode->mChildren[307], 1.0); //Back To Menu
}

void Render::Commands(void) {
	// current rotation angle
	static float angle = 0.f;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.f, 0.f, 3.f, 0.f, 0.f, -5.f, 0.f, 1.f, 0.f);

	// rotate it around the y axis
	glRotatef(angle, 0.f, 1.f, 0.f);

	//glScalef(tmp, tmp, tmp);
	glScalef(0.7, 0.7, 0.7);

	// center the model
	glTranslatef(-scene_center.x, -scene_center.y, -scene_center.z);

	recursive_render(scene, scene->mRootNode->mChildren[311], 1.0); //Background Image

	recursive_render(scene, scene->mRootNode->mChildren[310], 1.0); //Title
	recursive_render(scene, scene->mRootNode->mChildren[312], 1.0); //SubTitle

	recursive_render(scene, scene->mRootNode->mChildren[313], 1.0); //Title
	recursive_render(scene, scene->mRootNode->mChildren[314], 1.0); //SubTitle
		
	recursive_render(scene, scene->mRootNode->mChildren[315], 1.0); //Mouse Icon
	if (mouse.BackToMenuCommandsHover()) recursive_render(scene, scene->mRootNode->mChildren[317], 1.0); //Back To Menu HOVER
	else recursive_render(scene, scene->mRootNode->mChildren[316], 1.0); //Back To Menu
}

void Render::Level_1(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* CAMERA ****************************************************************/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.f, 1.f, 2.1, 0.0f, 0.0f, -5.f, 0.f, 1.f, 0.f);

	//SKY
	recursive_render(scene, scene->mRootNode->mChildren[10], 1.0); //It follows the camera so it doesn't feel like getting closer

	//LIFE
	ship.ShowLife();

	//SCORE
	score.RenderScore();	

	glScalef(0.7, 0.7, 0.7); // scale the whole asset to fit into our view frustum 	
	glTranslatef(-scene_center.x, -scene_center.y, -scene_center.z); // center the model
	glTranslatef(0.0, 0.0, -ship.position[2]); // move the camera forwards
	/**************************************************************************/

	//POWERUP
	powerUp1->RenderPowerUp();
	powerUp2->RenderPowerUp();
	
	//SEA	
	recursive_render(scene, scene->mRootNode->mChildren[11], 1.0);

	//ROCKS
	switch (stateMachine.randomMap) {
	case 1:
		//ROCKS v1
		for (int i = 16; i < 82; ++i)
			recursive_render(scene, scene->mRootNode->mChildren[i], 1.0);
		break;
	case 2:
		//ROCKS v2
		for (int i = 761; i < 827; ++i)
			recursive_render(scene, scene->mRootNode->mChildren[i], 1.0);
		break;
	case 3:
		//ROCKS v3
		for (int i = 827; i < 893; ++i)
			recursive_render(scene, scene->mRootNode->mChildren[i], 1.0);
		break;
	}
	
	//ROCKS ENVIRONMENT
	for (int i = 87; i < 271; ++i)
		recursive_render(scene, scene->mRootNode->mChildren[i], 1.0);

	//ISLAND
	ShowIsland();

	//SHIP	
	glTranslatef(ship.position[0], ship.position[1], ship.position[2]);
	ship.ShowShip();
	//ship.ShowPath();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void Render::Level_2(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* CAMERA ****************************************************************/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.f, 1.f, 2.1, 0.0f, 0.0f, -5.f, 0.f, 1.f, 0.f);

	//SKY
	recursive_render(scene, scene->mRootNode->mChildren[10], 1.0); //It follows the camera so it doesn't feel like getting closer

	//LIFE
	ship.ShowLife();

	//SCORE
	score.RenderScore();

	glScalef(0.7, 0.7, 0.7); // scale the whole asset to fit into our view frustum 	
	glTranslatef(-scene_center.x, -scene_center.y, -scene_center.z); // center the model
	glTranslatef(0.0, 0.0, -ship.position[2]); // move the camera forwards
	/**************************************************************************/

	//POWERUP
	powerUp1->RenderPowerUp();
	powerUp2->RenderPowerUp();
	powerUp3->RenderPowerUp();
	powerUp4->RenderPowerUp();

	//SEA	
	recursive_render(scene, scene->mRootNode->mChildren[11], 1.0);

	//ROCKS
	switch (stateMachine.randomMap) {
	case 1:
		//ROCKS v1
		for (int i = 318; i < 490; ++i)
			recursive_render(scene, scene->mRootNode->mChildren[i], 1.0);
		break;
	case 2:
		//ROCKS v2
		for (int i = 893; i < 1065; ++i)
			recursive_render(scene, scene->mRootNode->mChildren[i], 1.0);
		break;
	case 3:
		//ROCKS v3
		for (int i = 1065; i < 1237; ++i)
			recursive_render(scene, scene->mRootNode->mChildren[i], 1.0);
		break;
	}

	//ROCKS ENVIRONMENT
	for (int i = 87; i < 271; ++i)
		recursive_render(scene, scene->mRootNode->mChildren[i], 1.0);

	//ENEMY
	enemyShip.ShowEnemies(LEVEL_2, ship.position[2]); //Depending on the positinos of the ship, enemies spawn

	//ISLAND
	ShowIsland();

	//BULLET
	if (ship.isShooting) {
		animationBullet.AnimationShoot();
	}

	//SHIP	
	glTranslatef(ship.position[0], ship.position[1], ship.position[2]);
	ship.ShowShip();
	//ship.ShowPath();

}

void Render::Level_3(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* CAMERA ****************************************************************/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.f, 1.f, 2.1, 0.0f, 0.0f, -5.f, 0.f, 1.f, 0.f);

	//SKY
	recursive_render(scene, scene->mRootNode->mChildren[10], 1.0); //It follows the camera so it doesn't feel like getting closer

	//LIFE
	ship.ShowLife();

	//SCORE
	score.RenderScore();

	glScalef(0.7, 0.7, 0.7); // scale the whole asset to fit into our view frustum 	
	glTranslatef(-scene_center.x, -scene_center.y, -scene_center.z); // center the model
	glTranslatef(0.0, 0.0, -ship.position[2]); // move the camera forwards
	/**************************************************************************/

	//POWERUP
	powerUp1->RenderPowerUp();
	powerUp2->RenderPowerUp();
	powerUp3->RenderPowerUp();
	powerUp4->RenderPowerUp();
	powerUp5->RenderPowerUp();
	powerUp6->RenderPowerUp();

	//SEA	
	recursive_render(scene, scene->mRootNode->mChildren[11], 1.0);

	//ROCKS
	switch (stateMachine.randomMap) {
	case 1:
		//ROCKS v1
		for (int i = 490; i < 751; ++i)
			recursive_render(scene, scene->mRootNode->mChildren[i], 1.0);
		break;
	case 2:
		//ROCKS v2
		for (int i = 1237; i < 1498; ++i)
			recursive_render(scene, scene->mRootNode->mChildren[i], 1.0);
		break;
	case 3:
		//ROCKS v3
		for (int i = 1498; i < 1759; ++i)
			recursive_render(scene, scene->mRootNode->mChildren[i], 1.0);
		break;
	}

	//ROCKS ENVIRONMENT
	for (int i = 87; i < 271; ++i)
		recursive_render(scene, scene->mRootNode->mChildren[i], 1.0);

	//ENEMY
	enemyShip.ShowEnemies(LEVEL_3, ship.position[2]); //Depending on the positinos of the ship, enemies spawn

	//ISLAND
	ShowIsland();

	//BULLET
	if (ship.isShooting) {
		animationBullet.AnimationShoot();
	}

	//SHIP	
	glTranslatef(ship.position[0], ship.position[1], ship.position[2]);
	ship.ShowShip();
	//ship.ShowPath();

}

void Render::Game_Over(int level) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* CAMERA ****************************************************************/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.f, 1.f, 2.1, 0.0f, 0.0f, -5.f, 0.f, 1.f, 0.f);

	//SKY
	recursive_render(scene, scene->mRootNode->mChildren[10], 1.0); //It follows the camera so it doesn't feel like getting closer

	//LIFE
	ship.ShowLife();

	//GAMEOVER
	recursive_render(scene, scene->mRootNode->mChildren[276], 1.0); //GameOver Title
	if (mouse.BackToMenuGameoverHover()) recursive_render(scene, scene->mRootNode->mChildren[283], 1.0); //Back To Menu HOVER
	else recursive_render(scene, scene->mRootNode->mChildren[277], 1.0); //Back To Menu 
	if (mouse.TryAgainHover()) recursive_render(scene, scene->mRootNode->mChildren[282], 1.0); //Try Again HOVER
	else recursive_render(scene, scene->mRootNode->mChildren[278], 1.0); //Try Again

	switch (level) {
	case LEVEL_1:
		recursive_render(scene, scene->mRootNode->mChildren[279], 1.0); //Level 1
		break;
	case LEVEL_2:
		recursive_render(scene, scene->mRootNode->mChildren[280], 1.0); //Level 2
		break;
	case LEVEL_3:
		recursive_render(scene, scene->mRootNode->mChildren[281], 1.0); //Level 3
		break;
	}

	glScalef(0.7, 0.7, 0.7); // scale the whole asset to fit into our view frustum 	
	glTranslatef(-scene_center.x, -scene_center.y, -scene_center.z); // center the model
	glTranslatef(0.0, 0.0, -ship.position[2]); // move the camera forwards
	/**************************************************************************/

	//SEA	
	recursive_render(scene, scene->mRootNode->mChildren[11], 1.0);

	//ROCKS
	ShowRocks(stateMachine.GetOldState());

	//ROCKS ENVIRONMENT
	for (int i = 87; i < 271; ++i)
		recursive_render(scene, scene->mRootNode->mChildren[i], 1.0);

	//ENEMY
	enemyShip.ShowEnemies(GAMEOVER, ship.position[2]); //Depending on the positinos of the ship, enemies spawn

	//ISLAND
	ShowIsland();

	//SHIP	
	glTranslatef(ship.position[0], ship.position[1], ship.position[2]);
	ship.ShowShip();
	//ship.ShowPath();

}

void Render::Victory(int level) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* CAMERA ****************************************************************/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.f, 1.f, 2.1, 0.0f, 0.0f, -5.f, 0.f, 1.f, 0.f);

	//SKY
	recursive_render(scene, scene->mRootNode->mChildren[10], 1.0); //It follows the camera so it doesn't feel like getting closer

	//LIFE
	ship.ShowLife();

	//SCORE
	score.RenderObtainedScore(stateMachine.GetOldState());

	//VICTORY
	recursive_render(scene, scene->mRootNode->mChildren[285], 1.0); //Victory Title
	recursive_render(scene, scene->mRootNode->mChildren[284], 1.0); //Pergamena
	if (mouse.BackToMenuVictoryHover()) recursive_render(scene, scene->mRootNode->mChildren[304], 1.0); //Back To Menu HOVER
	else recursive_render(scene, scene->mRootNode->mChildren[289], 1.0); //Back To Menu

	switch (level) {
	case LEVEL_1:
		recursive_render(scene, scene->mRootNode->mChildren[286], 1.0); //Level 1
		if (mouse.NextLevelHover()) recursive_render(scene, scene->mRootNode->mChildren[303], 1.0); //Next Lv HOVER
		else recursive_render(scene, scene->mRootNode->mChildren[290], 1.0); //Next Lv
		break;
	case LEVEL_2:
		recursive_render(scene, scene->mRootNode->mChildren[287], 1.0); //Level 2
		if (mouse.NextLevelHover()) recursive_render(scene, scene->mRootNode->mChildren[303], 1.0); //Next Lv HOVER
		else recursive_render(scene, scene->mRootNode->mChildren[290], 1.0); //Next Lv
		break;
	case LEVEL_3:
		recursive_render(scene, scene->mRootNode->mChildren[288], 1.0); //Level 3
		break;
	}

	//STARS
	ShowStars();

	glScalef(0.7, 0.7, 0.7); // scale the whole asset to fit into our view frustum 	
	glTranslatef(-scene_center.x, -scene_center.y, -scene_center.z); // center the model
	glTranslatef(0.0, 0.0, -ship.position[2]); // move the camera forwards
	/**************************************************************************/

	//SEA	
	recursive_render(scene, scene->mRootNode->mChildren[11], 1.0);

	//ISLAND
	ShowIsland();

	//SHIP	
	glTranslatef(ship.position[0], ship.position[1], ship.position[2]);
	ship.ShowShip();
	//ship.ShowPath();

}

void Render::RenderScene(void) {
	//Machine State Following
	int current_state = stateMachine.GetCurrentState();

	switch (current_state) {
	case MENU:
		if (stateMachine.changed || stateMachine.changedWindow || first) {
			if (first) {
				first = false;
				cout << "MENU ////////////////\n";
			}
			if (stateMachine.changed) stateMachine.changed = false;
			if (stateMachine.changedWindow) stateMachine.changedWindow = false;
			Menu();
		}
		break;
	case RULES:
		if (stateMachine.changed || stateMachine.changedWindow || first) {
			if (first) {
				first = false;
				cout << "RULES ////////////////\n";
			}
			if (stateMachine.changed) stateMachine.changed = false;
			if (stateMachine.changedWindow) stateMachine.changedWindow = false;
			Rules();
		}
		break;
	case COMMANDS:
		if (stateMachine.changed || stateMachine.changedWindow || first) {
			if (first) {
				first = false;
				cout << "COMMANDS ////////////////\n";
			}
			if (stateMachine.changed) stateMachine.changed = false;
			if (stateMachine.changedWindow) stateMachine.changedWindow = false;
			Commands();
		}
		break;
	case LEVEL_1:
		if (first) {
			first = false;
			cout << "LEVEL 1 ////////////////\n";
			//[10, 353]
			powerUp1 = new PowerUp(60, 210);
			powerUp2 = new PowerUp(220, 300);
			cout << "PowerUp1 --> [x: " << powerUp1->position[0] << "] [z: " << powerUp1->position[2] << "]\n";
			cout << "PowerUp2 --> [x: " << powerUp2->position[0] << "] [z: " << powerUp2->position[2] << "]\n";
		}
		Level_1();
		break;
	case LEVEL_2:
		if (first) {
			first = false;
			cout << "LEVEL 2 ////////////////\n";
			//[10, 433]
			powerUp1 = new PowerUp(20, 90);
			powerUp2 = new PowerUp(100, 170);
			powerUp3 = new PowerUp(180, 290);
			powerUp4 = new PowerUp(300, 400);
		}
		Level_2();
		break;
	case LEVEL_3:
		if (first) {
			first = false;
			cout << "LEVEL 3 ////////////////\n";
			//[10, 503]
			powerUp1 = new PowerUp(20, 90);
			powerUp2 = new PowerUp(110, 190);
			powerUp3 = new PowerUp(200, 290);
			powerUp4 = new PowerUp(300, 370);
			powerUp5 = new PowerUp(380, 440);
			powerUp6 = new PowerUp(450, 480);
		}
		Level_3();
		break;
	case GAMEOVER:
		if (stateMachine.changed || first) {
			if (first) {
				first = false;
				cout << "GAME OVER ////////////////\n";
			}
			if (stateMachine.changed) stateMachine.changed = false;
			Game_Over(stateMachine.GetOldState());
		}
		break;
	case VICTORY_1:
		if (stateMachine.changed || first) {
			if (first) {
				first = false;
				cout << "VICTORY LEVEL 1 ////////////////\n";
			}
			if (stateMachine.changed) stateMachine.changed = false;
			Victory(LEVEL_1);
		}
		break;
	case VICTORY_2:
		if (stateMachine.changed || first) {
			if (first) {
				first = false;
				cout << "VICTORY LEVEL 2 ////////////////\n";
			}
			if (stateMachine.changed) stateMachine.changed = false;
			Victory(LEVEL_2);
		}
		break;
	case VICTORY_3:
		if (stateMachine.changed || first) {
			if (first) {
				first = false;
				cout << "VICTORY LEVEL 3 ////////////////\n";
			}
			if (stateMachine.changed) stateMachine.changed = false;
			Victory(LEVEL_3);
		}
		break;
	default:
		break;
	}

	glutSwapBuffers();
}