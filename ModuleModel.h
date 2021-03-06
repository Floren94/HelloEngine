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
	ModuleModel() {};
	~ModuleModel() {};

	bool Init();
	update_status Update();
	bool CleanUp();

	void Load(const char* file_name);
	void ReloadTexture(const char* file_name);
	void Draw();

	int GetVertices();
	int GetVertices(int mesh) { return scene->mMeshes[mesh]->mNumVertices; }
	int GetMeshes()  { return scene->mNumMeshes; }
	int GetFaces(int mesh)  { return scene->mMeshes[mesh]->mNumFaces; }
	int GetTextureWidth()  { return App->texture->GetWidth(); }
	int GetTextureHeight()  { return App->texture->GetHeight(); }
	unsigned GetMaterial(int material) const { return material_vec[material]; }
	vec GetCenter() const { return centerP; }
	double GetOuterZ() const { return outerZ; }

private:
	void LoadMaterials();
	void LoadMeshes();

	const aiScene* scene;
	std::vector<unsigned int> material_vec;
	std::vector<Mesh> mesh_vec;
	std::string filepath;
	vec centerP;
	double outerZ;
};