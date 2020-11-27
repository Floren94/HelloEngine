#include "Application.h"
#include "ModuleModel.h"
#include "ModuleTexture.h"
#include "ModuleEditor.h"
#include "Assimp/cimport.h"
#include "Assimp/postprocess.h"
#include "Assimp/Importer.hpp"

ModuleModel::ModuleModel()
{
}

// Destructor
ModuleModel::~ModuleModel()
{
}

// Called before render is available
bool ModuleModel::Init()
{
	centerP = { 0,0,0 };
	return true;
}


// Called every draw update
update_status ModuleModel::Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleModel::CleanUp()
{
	for (unsigned i = 0; i < material_vec.size(); ++i) {
		App->texture->FreeTexture(material_vec[i]);
	}

	for (unsigned i = 0; i < mesh_vec.size(); ++i) {
		mesh_vec[i].DeleteBuff();
	}
	material_vec.clear();
	mesh_vec.clear();
	return true;
}

void ModuleModel::Load(const char* file_name) {
	scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene) {
		LoadMaterials();
		LoadMeshes();
	}
	else {
		LOG("Error loading %s: %s", aiGetErrorString());
	}
}

void ModuleModel::LoadMaterials() {
	aiString file;

	//material_vec.reserve(scene->mNumMaterials);
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS)
		{
			material_vec.push_back(App->texture->LoadTexture(file.data));
		}
	}
}

void ModuleModel::LoadMeshes() {

	//mesh_vec.reserve(scene->mNumMaterials);
	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh* my_Mesh = scene->mMeshes[i];
		Mesh new_mesh;
		new_mesh.LoadVBO(my_Mesh);
		new_mesh.LoadEBO(my_Mesh);
		new_mesh.CreateVAO();
		mesh_vec.push_back(new_mesh);

		if (i == 0){
			centerP = new_mesh.GetCenter();
		}else{
			centerP = (centerP + new_mesh.GetCenter());
			centerP = { centerP.x / 2,centerP.y / 2 ,centerP.z / 2 };
		}

		double modelOutZ = new_mesh.GetOuterZ();
		if (modelOutZ > outerZ) outerZ = modelOutZ;
	}
}

void ModuleModel::Draw() {
	for (unsigned i = 0; i < mesh_vec.size(); ++i) {
		mesh_vec[i].Draw(material_vec);
	}
}

int ModuleModel::GetVertices() {
	int vertices = 0;
	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		vertices += scene->mMeshes[i]->mNumVertices;
	}

	return vertices;
}

int ModuleModel::GetTriangles() {

	return GetVertices() / 3;
}