#include "Globals.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "SDL.h"
#include "GL/glew.h"


ModuleProgram::ModuleProgram()
{
}

// Destructor
ModuleProgram::~ModuleProgram()
{
}

bool ModuleProgram::Init()
{
	SDL_GL_SetAttribute(
		SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG
	);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	vertex_id = CompileShader(GL_VERTEX_SHADER, LoadShaderSource("Shaders/Vertex.vert"));
	fragment_id = CompileShader(GL_FRAGMENT_SHADER, LoadShaderSource("Shaders/Fragment.frag"));

	program = CreateProgram(vertex_id, fragment_id);
	return true;
}

// Called before quitting
bool ModuleProgram::CleanUp()
{
	LOG("Destroying Program");

	glDeleteProgram(program);
	return true;
}

//
//unsigned ModuleProgram::CreateTriangleVBO()
//{
//	float vtx_data[] = {
//	-1.0f, -1.0f, 0.0f, // ← v0 pos tri1
//	1.0f, 1.0f, 0.0f, // ← v1 pos tri1
//	-1.0f, 1.0f, 0.0f, // ← v2 pos tri1
//
//	-1.0f, -1.0f, 0.0f, // ← v0 pos tri2
//	1.0f, -1.0f, 0.0f,// ← v1 pos tri2
//	1.0f, 1.0f, 0.0f,// ← v2 pos tri2
//
//	0.0f, 0.0f, // ← v0 texcoord tri1
//	1.0f, 1.0f, // ← v1 texcoord tri1
//	0.0f, 1.0f,  // ← v2 texcoord tri1
//
//	0.0f, 0.0f, // ← v0 texcoord tri2
//	1.0f, 0.0f, // ← v1 texcoord tri2
//	1.0f, 1.0f // ← v2 texcoord tri2
//	};
//	unsigned vbo;
//	glGenBuffers(1, &vbo);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);
//
//	return vbo;
//}
//// This function must be called one time at destruction of vertex buffer
//void ModuleProgram::DestroyVBO(unsigned vbo)
//{
//	glDeleteBuffers(1, &vbo);
//}

char* ModuleProgram::LoadShaderSource(const char* shader_file_name)
{
	char* data = nullptr;
	FILE* file = nullptr;
	fopen_s(&file, shader_file_name, "rb");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		data = (char*)malloc(size + 1);
		fseek(file, 0, SEEK_SET);
		fread(data, 1, size, file);
		data[size] = 0;
		fclose(file);
	}
	return data;
}

unsigned ModuleProgram::CompileShader(unsigned type, const char* source)
{
	unsigned shader_id = glCreateShader(type);
	glShaderSource(shader_id, 1, &source, 0);
	glCompileShader(shader_id);
	int res = GL_FALSE;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE)
	{
		int len = 0;
		glGetShaderiv(GL_COMPILE_STATUS, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			int written = 0;
			char* info = (char*)malloc(len);
			glGetShaderInfoLog(shader_id, len, &written, info);
			LOG("Log Info: %s", info);
			free(info);
		}
	}
	return shader_id;
}

unsigned ModuleProgram::CreateProgram(unsigned vtx_shader, unsigned frg_shader)
{
	unsigned program_id = glCreateProgram();
	glAttachShader(program_id, vtx_shader);
	glAttachShader(program_id, frg_shader);
	glLinkProgram(program_id);
	int res;
	glGetProgramiv(program_id, GL_LINK_STATUS, &res);
	if (res == GL_FALSE)
	{
		int len = 0;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			int written = 0;
			char* info = (char*)malloc(len);
			glGetProgramInfoLog(program_id, len, &written, info);
			LOG("Program Log Info: %s", info);
			free(info);
		}
	}
	glDeleteShader(vtx_shader);
	glDeleteShader(frg_shader);
	return program_id;
}

//// This function must be called each frame for drawing the triangle
//void ModuleProgram::RenderVBO(unsigned vbo, unsigned program)
//{
//	float4x4 proj, view, model;
//
//	proj = App->camera->GetProjection();
//	view = App->camera->GetView();
//	model = float4x4::FromTRS(float3(0.0f, 0.0f, 0.0f),
//		float4x4::RotateZ(pi),
//		float3(1.0f, 1.0f, 0.0f));
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	glEnableVertexAttribArray(0);
//	glEnableVertexAttribArray(1);
//	// size = 3 float per vertex
//	// stride = 0 is equivalent to stride = sizeof(float)*3
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 6 * 3));
//	glUseProgram(program);
//	//1 triangle to draw = 3 vertices
//
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, App->texture->textureID);
//	glUniform1i(glGetUniformLocation(program, "mytexture"), 0);
//
//	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, &proj[0][0]);
//	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, &view[0][0]);
//	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &model[0][0]);
//
//	glDrawArrays(GL_TRIANGLES, 0, 6);
//}