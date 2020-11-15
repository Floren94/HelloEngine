#pragma once

#include "Module.h"
#include "Globals.h"
#include "ModuleProgram.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "SDL.h"
#include "GL/glew.h"

class ModuleProgram : public Module
{
public:
	ModuleProgram();
	~ModuleProgram();

	bool CleanUp();

	unsigned CreateTriangleVBO()
	{
		float vtx_data[] = { 
		-1.0f, -1.0f, 0.0f, // ? v0 pos
		1.0f, -1.0f, 0.0f, // ? v1 pos
		0.0f, 1.0f, 0.0f, // ? v2 pos

		0.0f, 0.0f, // ? v0 texcoord
		1.0f, 0.0f, // ? v1 texcoord
		0.5f, 1.0f // ? v2 texcoord
		};
		unsigned vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active
		glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);

		return vbo;
	}
	// This function must be called one time at destruction of vertex buffer
	void DestroyVBO(unsigned vbo)
	{
		glDeleteBuffers(1, &vbo);
	}

	//// This function must be called each frame for drawing the triangle
	//void RenderVBO(unsigned vbo)
	//{
	//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//	glEnableVertexAttribArray(0);
	//	// size = 3 float per vertex
	//	// stride = 0 is equivalent to stride = sizeof(float)*3
	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//	// 1 triangle to draw = 3 vertices
	//	glDrawArrays(GL_TRIANGLES, 0, 3);
	//}

	char* LoadShaderSource(const char* shader_file_name)
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

	unsigned CompileShader(unsigned type, const char* source)
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

	unsigned CreateProgram(unsigned vtx_shader, unsigned frg_shader)
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

	// This function must be called each frame for drawing the triangle
	void RenderVBO(unsigned vbo, unsigned program)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(1);
		// size = 3 float per vertex
		// stride = 0 is equivalent to stride = sizeof(float)*3
		GLintptr vertex_texcoord_offset = 3 * 3 * sizeof(float);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)vertex_texcoord_offset);
		glUseProgram(program);
		// 1 triangle to draw = 3 vertices

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}


private:
	void* context;
};

