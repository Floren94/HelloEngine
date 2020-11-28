#include "Globals.h"
#include "Application.h"
#include "ModuleRenderExercise.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "ModuleInput.h"
#include "ModuleDebugDraw.h"
#include "ModuleEditor.h"
#include "ModuleModel.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_main.h"
#include "GL/glew.h"
#include "il.h"
#include "ilu.h"
#include "assimp/cimport.h"
#include "src/Math/float4x4.h"
#include "src/Math/float3x3.h"

ModuleRenderExercise::ModuleRenderExercise()
{
}

// Destructor
ModuleRenderExercise::~ModuleRenderExercise()
{
}

void myCallback(const char* msg, char* userData) {
	App->editor->AddLog(msg);
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

	context = SDL_GL_CreateContext(App->window->window);

	GLenum err = glewInit();

	glEnable(GL_DEPTH_TEST); // Enable depth test
	glEnable(GL_CULL_FACE); // Enable cull backward faces
	glFrontFace(GL_CCW); // Front faces will be counter clockwise

	struct aiLogStream stream;
	stream.callback = myCallback;
	aiAttachLogStream(&stream);

	return true;
}
bool ModuleRenderExercise::Start() {
	App->model->Load("BakerHouse.fbx");
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
	int w, h;

	SDL_GetWindowSize(App->window->window, &w, &h);

	App->debugdraw->Draw(App->camera->GetView() , App->camera->GetProjection(), w, h);

	App->model->Draw();
	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::PostUpdate()
{
	App->editor->RenderUi();

	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderExercise::CleanUp()
{
	LOG("Destroying RenderExercise");
	SDL_GL_DeleteContext(context);
	return true;
}

void ModuleRenderExercise::LoadDropModel(char* filename) {
	std::string s = (std::string)filename;

	size_t i = s.rfind('.', s.length());

	s = s.substr(i + 1, s.length() - i);
	
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);

	if (s == "fbx") {
		App->model->CleanUp();
		App->model->Load(filename);
		SDL_free(filename);
	}else if (s == "png" || s == "dds" || s == "jpg") {
		App->model->ReloadTexture(filename);
		SDL_free(filename);
	}
	else {
		LOG("%s file not supported", s);
	}
}