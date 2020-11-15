#include "Globals.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleDebugDraw.h"
#include "SDL.h"
#include "GL/glew.h"
#include <chrono>


Uint64 now = 0;
Uint64 last = 0;

ModuleCamera::ModuleCamera()
{
}

// Destructor
ModuleCamera::~ModuleCamera()
{

}

// Called before Camera is available
bool ModuleCamera::Init()
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.1f, 200.0f);
	frustum.SetHorizontalFovAndAspectRatio(DEGTORAD * 90.0f, 1.3f);

	frustum.SetPos(float3(0, 1, -2));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	float4x4 projectionGL = frustum.ProjectionMatrix().Transposed(); //<-- Important to transpose!

	float4x4 view = frustum.ViewMatrix();
	view.Transpose();

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(*(frustum.ProjectionMatrix().Transposed().v));

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(*view.v);

	return true;
}

update_status ModuleCamera::PreUpdate()
{

	if (drag) {
		last = now;
		now = SDL_GetPerformanceCounter();
		deltaTime = (double)((now - last) * 1000 / (double)SDL_GetPerformanceFrequency());
	}
	else {
		now = SDL_GetPerformanceCounter();
		last = 0;
		deltaTime = 0;
	}

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleCamera::Update()
{
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
		int* mouseY = new int(1); 
		int* mouseX = new int(1);
		SDL_GetMouseState(NULL, mouseY);
		SDL_GetMouseState(mouseX, NULL);

		if (mouseY[0] > lastYmouse + 5) {
			VerticalMove(SPEED);
			lastYmouse = mouseY[0];
		}
		else if (mouseY[0] < lastYmouse - 5) {
			VerticalMove(-SPEED);
			lastYmouse = mouseY[0];
		}
		
		if (mouseX[0] > lastXmouse + 5) {
			SideMove(SPEED);
			lastXmouse = mouseX[0];
		}
		else if (mouseX[0] < lastXmouse - 5) {
			SideMove(-SPEED);
			lastXmouse = mouseX[0];
		}
		delete mouseY;
		delete mouseX;
		mouseY = nullptr;
		mouseX = nullptr;
	}
	else if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)) {
		int* mouseY = new int(1);
		SDL_GetMouseState(NULL, mouseY);

		if (mouseY[0] > lastYmouse + 5) {
			Zoom(SPEED);
			lastYmouse = mouseY[0];
		}
		else if (mouseY[0] < lastYmouse - 5) {
			Zoom(-SPEED);
			lastYmouse = mouseY[0];
		}
		delete mouseY;
		mouseY = nullptr;
	}
	else if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)) {
		VerticalMove(SPEED);
	}
	else if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)) {
		VerticalMove(-SPEED);
	}
	else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)) {
		Move(SPEED);
	}
	else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)) {
		Move(-SPEED);
	}
	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)) {
		SideMove(SPEED);
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)) {
		SideMove(-SPEED);
	}
	else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)) {
		RotateX(-DEG);
	}
	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)) {
		RotateX(DEG);
	}
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)) {
		RotateY(-DEG);
	}
	else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)) {
		RotateY(DEG);
	}
	else drag = false;

	float4x4 view = frustum.ViewMatrix();
	view.Transpose();

	

	App->debugdraw->Draw(view, frustum.ProjectionMatrix().Transposed(), App->window->screen_surface->w, App->window->screen_surface->h);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(*(frustum.ProjectionMatrix().Transposed().v));

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(*view.v);

	return UPDATE_CONTINUE;
}

update_status ModuleCamera::PostUpdate()
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleCamera::CleanUp()
{
	LOG("Destroying Camera");

	return true;
}

void ModuleCamera::VerticalMove(double dir) {

	drag = true;
	float3 newP = frustum.Pos();
	newP.y += dir;
	frustum.SetPos(newP);

}


void ModuleCamera::Move(double dir) {

	drag = true;
	vec front = frustum.Front();
	vec pos = frustum.Pos();
	float farDist = frustum.FarPlaneDistance();

	vec lookAt = pos + (front * farDist);

	vec forward = lookAt - pos;
	forward.Normalize();

	vec newP = pos + (forward * dir);

	frustum.SetPos(newP);

}

void ModuleCamera::SideMove(double dir) {
	drag = true;
	vec up = frustum.Up();
	vec front = frustum.Front();
	vec pos = frustum.Pos();

	float farDist = frustum.FarPlaneDistance();

	vec lookAt = pos + (front * farDist);

	vec forward = lookAt - pos;
	forward.Normalize();

	vec crossLeft = up.Cross(forward);

	vec newP = pos + (crossLeft * dir);

	frustum.SetPos(newP);

}

void ModuleCamera::RotateX(double degrees) {

	float3x3 rotationMatrix = float3x3::RotateX(degrees * DEGTORAD);
	vec oldFront = frustum.Front().Normalized();
	frustum.SetFront(rotationMatrix * oldFront);
	vec oldUp = frustum.Up().Normalized();
	frustum.SetUp(rotationMatrix * oldUp);

}

void ModuleCamera::RotateY(double degrees) {

	float3x3 rotationMatrix = float3x3::RotateY(degrees * DEGTORAD);
	vec oldFront = frustum.Front().Normalized();
	frustum.SetFront(rotationMatrix * oldFront);
	vec oldUp = frustum.Up().Normalized();
	frustum.SetUp(rotationMatrix * oldUp);

}

void ModuleCamera::Zoom(double dir) {

	drag = true;
	vec front = frustum.Front();
	vec pos = frustum.Pos();
	float farDist = frustum.FarPlaneDistance();

	vec lookAt = pos + (front * farDist);

	vec forward = lookAt - pos;
	forward.Normalize();

	vec newP = front + (forward * dir);

	frustum.SetFront(newP);

}