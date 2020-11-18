#include "Globals.h"
#include "Application.h"
#include "ModuleRenderExercise.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "ModuleInput.h"
#include "ModuleDebugDraw.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_main.h"
#include "GL/glew.h"
#include "il.h"
#include "ilu.h"

ModuleRenderExercise::ModuleRenderExercise()
{
}

// Destructor
ModuleRenderExercise::~ModuleRenderExercise()
{
	App->program->DestroyVBO(triangle);
}

// Called before RenderExercise is available
bool ModuleRenderExercise::Init()
{
	LOG("Creating Renderer context");

	glewInit();
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); // desired version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // we want a double buffer
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // we want to have a depth buffer with 24 bits
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8); // we want to have a stencil buffer with 8 bits

	SDL_GL_CreateContext(App->window->window);

	GLenum err = glewInit();

	glEnable(GL_DEPTH_TEST); // Enable depth test
	glEnable(GL_CULL_FACE); // Enable cull backward faces
	glFrontFace(GL_CCW); // Front faces will be counter clockwise

	triangle = App->program->CreateTriangleVBO();

	unsigned int vertex_id = App->program->CompileShader(GL_VERTEX_SHADER, App->program->LoadShaderSource("Shaders/Vertex.vert"));
	unsigned int fragment_id = App->program->CompileShader(GL_FRAGMENT_SHADER, App->program->LoadShaderSource("Shaders/Fragment.frag"));

	program = App->program->CreateProgram(vertex_id, fragment_id);

	return true;
}

update_status ModuleRenderExercise::PreUpdate()
{

	int w = 0;
	int h = 0;
	SDL_GetWindowSize(App->window->window, &w, &h);
	glViewport(0, 0, w, h);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRenderExercise::Update()
{
	App->program->RenderVBO(triangle, program);
	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::PostUpdate()
{
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderExercise::CleanUp()
{
	LOG("Destroying RenderExercise");
	return true;
}