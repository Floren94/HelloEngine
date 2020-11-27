#pragma once
#include "Assimp/mesh.h"
#include "Application.h"
#include "ModuleRenderExercise.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include <iostream>
#include "src/Math/float2.h"
#include "src/Math/float4x4.h"
#include "src/Math/float3x3.h"

class Mesh 
{
public :
	Mesh() {};
	~Mesh() {};
	
	void LoadVBO(const aiMesh* mesh);
	void LoadEBO(const aiMesh* mesh);
	void CreateVAO();
	void Draw(const std::vector<unsigned>& model_textures);
	void DeleteBuff();

	vec	 GetCenter();
	double GetOuterZ() { return maxZ; };

private :
	unsigned int vbo = 0, ebo = 0, vao = 0;
	int materialID = 0, nIndices = 0;
	double minX, maxX, minY, maxY, minZ, maxZ;
	vec centerP;
};

inline 
void Mesh::LoadVBO(const aiMesh* mesh) {
	minX = maxX = minY = maxY = minZ = maxZ = 0;
	centerP = { 0,0,0 };

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	unsigned vertex_size = (sizeof(float) * 3) + (sizeof(float) * 2);
	unsigned buffer_size = vertex_size * mesh->mNumVertices;
	glBufferData(GL_ARRAY_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);;

	float* vertices = (float*)(glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE));

	for (unsigned i = 0; i < mesh->mNumVertices; ++i)
	{
		*(vertices++) = mesh->mVertices[i].x;
		*(vertices++) = mesh->mVertices[i].y;
		*(vertices++) = mesh->mVertices[i].z;
		*(vertices++) = mesh->mTextureCoords[0][i].x;
		*(vertices++) = mesh->mTextureCoords[0][i].y;

		if (mesh->mVertices[i].x > maxX) maxX = mesh->mVertices[i].x;
		if (mesh->mVertices[i].x < minX) minX = mesh->mVertices[i].x;
		if (mesh->mVertices[i].y > maxY) maxY = mesh->mVertices[i].y;
		if (mesh->mVertices[i].y < minY) minY = mesh->mVertices[i].y;
		if (mesh->mVertices[i].z > maxZ) maxZ = mesh->mVertices[i].z;
		if (mesh->mVertices[i].z < minZ) minZ = mesh->mVertices[i].z;
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);
	materialID = mesh->mMaterialIndex;
}

inline
void Mesh::LoadEBO(const aiMesh* mesh) {

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	unsigned index_size = sizeof(unsigned) * mesh->mNumFaces * 3;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size, nullptr, GL_STATIC_DRAW);
	unsigned* indices = (unsigned*)(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_WRITE));

	for (unsigned i = 0; i < mesh->mNumFaces; ++i)
	{
		assert(mesh->mFaces[i].mNumIndices == 3); 
		*(indices++) = mesh->mFaces[i].mIndices[0];
		*(indices++) = mesh->mFaces[i].mIndices[1];
		*(indices++) = mesh->mFaces[i].mIndices[2];
	}

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	nIndices = mesh->mNumFaces * 3;
}

inline
void Mesh::CreateVAO() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (sizeof(float) * 5), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (sizeof(float) * 5), (void*)(sizeof(float) * 3));

	glBindVertexArray(0);
}

inline
void Mesh::Draw(const std::vector<unsigned>& model_textures) {
	unsigned int program = App->program->GetProgram();
	const float4x4& view = App->camera->GetView();
	const float4x4& proj = App->camera->GetProjection();
	float4x4 model = float4x4::identity;
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, (const float*)&model);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, (const float*)&view);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, (const float*)&proj);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model_textures[materialID]);
	glUniform1i(glGetUniformLocation(program, "diffuse"), 0);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, nullptr);
}
inline
void Mesh::DeleteBuff() {
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

inline
vec Mesh::GetCenter() {

	float midX, midY, midZ;

	midX = (maxX + minX) / 2;
	midY = (maxY + minY) / 2;
	midZ = (maxZ + minZ) / 2;

	centerP = { midX, midY, midZ };

	return centerP;
}