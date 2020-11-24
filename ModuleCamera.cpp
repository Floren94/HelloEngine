#include "Globals.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleEditor.h"
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

	frustum.SetPos(float3(0, 2, -12));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	float4x4 projectionGL = frustum.ProjectionMatrix().Transposed(); //<-- Important to transpose!

	float4x4 view = frustum.ViewMatrix();
	view.Transpose();

	return true;
}

update_status ModuleCamera::PreUpdate()
{

	if (startDelta) {
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

	double speed = SPEED;

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(*(frustum.ProjectionMatrix().Transposed().v));

	if (App->editor->isFocused) {
		return UPDATE_CONTINUE;
	}

	projectionGL = frustum.ProjectionMatrix(); 

	viewMatrix = frustum.ViewMatrix();

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT)) {
		speed *= 2.0f;
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
		int* mouseY = new int(1); 
		int* mouseX = new int(1);
		SDL_GetMouseState(NULL, mouseY);
		SDL_GetMouseState(mouseX, NULL);

		if (mouseY[0] > lastYmouse ) {
			VerticalMove(speed / 2);
			lastYmouse = mouseY[0];
		}
		else if (mouseY[0] < lastYmouse ) {
			VerticalMove(-speed / 2);
			lastYmouse = mouseY[0];
		}
		
		if (mouseX[0] > lastXmouse ) {
			SideMove(speed/2);
			lastXmouse = mouseX[0];
		}
		else if (mouseX[0] < lastXmouse ) {
			SideMove(-speed / 2);
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

		if (mouseY[0] > lastYmouse + 1) {
			Zoom(speed);
			lastYmouse = mouseY[0];
		}
		else if (mouseY[0] < lastYmouse - 1) {
			Zoom(-speed);
			lastYmouse = mouseY[0];
		}
		delete mouseY;
		mouseY = nullptr;
	}
	else if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)) {
		VerticalMove(speed);
	}
	else if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)) {
		VerticalMove(-speed);
	}
	else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)) {
		Move(speed);
	}
	else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)) {
		Move(-speed);
	}
	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)) {
		SideMove(speed);
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)) {
		SideMove(-speed);
	}
	else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)) {
		RotateX(speed / 6);
	}
	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)) {
		RotateX(-speed / 6);
	}
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)) {
		RotateY(-speed * 6);
	}
	else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)) {
		RotateY(speed * 6);
	}else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT) {
		int* mouseY = new int(1);
		int* mouseX = new int(1);
		SDL_GetMouseState(NULL, mouseY);
		SDL_GetMouseState(mouseX, NULL);

		if (mouseY[0] > lastYmouse) {
			RotateX(speed / 10);
			lastYmouse = mouseY[0];
		}
		else if (mouseY[0] < lastYmouse) {
			RotateX(-speed / 10);
			lastYmouse = mouseY[0];
		}

		if (mouseX[0] > lastXmouse) {
			RotateY(speed * 4);
			lastXmouse = mouseX[0];
		}
		else if (mouseX[0] < lastXmouse) {
			RotateY(-speed * 4);
			lastXmouse = mouseX[0];
		}
		delete mouseY;
		delete mouseX;
		mouseY = nullptr;
		mouseX = nullptr;
	}
	else startDelta = false;

	float4x4 view = frustum.ViewMatrix();
	view.Transpose();

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

	startDelta = true;
	float3 newP = frustum.Pos();
	newP.y += dir;
	frustum.SetPos(newP);

}


void ModuleCamera::Move(double dir) {

	startDelta = true;
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
	startDelta = true;
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

	vec oldFront = (frustum.Front() * cos(degrees) + frustum.Up() * sin(degrees)).Normalized();
	frustum.SetFront(oldFront);

	vec oldUp = frustum.WorldRight().Cross(oldFront);
	frustum.SetUp(oldUp);
}

void ModuleCamera::RotateY(double degrees) {

	float3x3 rotationMatrix = frustum.WorldMatrix().RotatePart().RotateY(degrees * DEGTORAD);
	vec oldFront = frustum.Front().Normalized();
	frustum.SetFront(rotationMatrix.MulDir(oldFront));
	vec oldUp = frustum.Up().Normalized();
	frustum.SetUp(rotationMatrix.MulDir(oldUp));

}

void ModuleCamera::Zoom(double dir) {

	startDelta = true;
	vec front = frustum.Front();
	vec pos = frustum.Pos();
	float farDist = frustum.FarPlaneDistance();

	vec lookAt = pos + (front * farDist);

	vec forward = lookAt - pos;
	forward.Normalize();

	vec newP = front + (forward * dir);

	frustum.SetFront(newP);

}

void ModuleCamera::SetFOV(float aspectRadio) {
	frustum.SetHorizontalFovAndAspectRatio(frustum.HorizontalFov(), aspectRadio);
}
