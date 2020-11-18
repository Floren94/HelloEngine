#pragma once

#include "Module.h"
#include "Globals.h"
#include "ModuleProgram.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleTexture.h"
#include "ModuleCamera.h"
#include "SDL.h"
#include "GL/glew.h"
#include "src/Math/float3x3.h"
#include "src/Math/float4x4.h"

class ModuleProgram : public Module
{
public:
	ModuleProgram();
	~ModuleProgram();

	bool CleanUp();

	unsigned CreateTriangleVBO();
	unsigned CompileShader(unsigned type, const char* source);
	unsigned CreateProgram(unsigned vtx_shader, unsigned frg_shader);
	char* LoadShaderSource(const char* shader_file_name);
	void RenderVBO(unsigned vbo, unsigned program);
	void DestroyVBO(unsigned vbo);

private:
	void* context;
};

