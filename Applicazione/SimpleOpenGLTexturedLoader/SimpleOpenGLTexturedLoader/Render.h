#pragma once
#include "Settings.h"

class Render {
public:
	Render(void);
	~Render(void);

	void RenderScene(void);
	void Menu(void);
	void Rules(void);
	void Commands(void);
	void Level_1(void);
	void Level_2(void);
	void Level_3(void);
	void Game_Over(int level);
	void Victory(int level);

	/* UTILITIES */
	void ShowStars();
	void ShowIsland();
	void ShowRocks(int level);

	/* DEFAULT METHODS */
	void reshape(int width, int height);
	void get_bounding_box_for_node(const struct aiNode* nd, struct aiVector3D* min, struct aiVector3D* max, struct aiMatrix4x4* trafo);
	void get_bounding_box(struct aiVector3D* min, struct aiVector3D* max);
	void color4_to_float4(const struct aiColor4D* c, float f[4]);
	void set_float4(float f[4], float a, float b, float c, float d);
	void color4f(const struct aiColor4D* color);
	void apply_material(const struct aiMaterial* mtl);
	void recursive_render(const struct aiScene* sc, const struct aiNode* nd, float scale);
	int loadasset(const char* path);
	/***************************************/

	/* VARIABLES */
	std::map<std::string, GLuint*> textureIdMap;	// map image filenames to textureIds
	struct aiVector3D scene_min, scene_max, scene_center;
	const struct aiScene* scene = NULL;
	bool first = true;

private:

};

