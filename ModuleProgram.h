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
	ModuleProgram() {};
	~ModuleProgram() {};

	bool Init();
	bool CleanUp();

	unsigned CompileShader(unsigned type, const char* source);
	unsigned CreateProgram(unsigned vtx_shader, unsigned frg_shader);
	char* LoadShaderSource(const char* shader_file_name);

	unsigned int GetProgram() const { return program; }

private:
	unsigned int program, vertex_id, fragment_id;
};

