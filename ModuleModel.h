#pragma once
#include "Module.h"
#include "Globals.h"
#include "Mesh.h"
#include "Assimp/mesh.h"
#include "Assimp/scene.h"
#include <vector>

class ModuleModel : public Module
{
public:
	ModuleModel();
	~ModuleModel();

	bool Init();
	update_status Update();
	bool CleanUp();

	void Load(const char* file_name);
	void Draw();

	int GetVertices();
	int GetTriangles();

private:
	void LoadMaterials();
	void LoadMeshes();

	const aiScene* scene;
	std::vector<unsigned int> material_vec;
	std::vector<Mesh> mesh_vec;
};