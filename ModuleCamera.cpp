#include "Globals.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleDebugDraw.h"
#include "ModuleModel.h"
#include "SDL.h"
#include "GL/glew.h"
#include <chrono>



bool ModuleCamera::Init()
{
	SDL_DisplayMode DM;
	SDL_GetDesktopDisplayMode(0, &DM);

	double deskW = DM.w;
	double deskH = DM.h;


	aspectRatio = deskW / deskH;

	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.1f, 200.0f);
	frustum.SetHorizontalFovAndAspectRatio(DEGTORAD * 90.0f, aspectRatio);

	frustum.SetPos(float3(0, 0, 0));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	float4x4 projectionGL = frustum.ProjectionMatrix().Transposed(); 

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
	double degree = DEG;

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(*(frustum.ProjectionMatrix().Transposed().v));

	if (App->editor->isFocused) {
		return UPDATE_CONTINUE;
	}

	projectionGL = frustum.ProjectionMatrix();
	viewMatrix = frustum.ViewMatrix();

	//doubled speed
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT)) {
		speed *= 2.0f;
		degree *= 2.0f;
	}

	if (App->input->GetKey(SDL_SCANCODE_F)) {
		//focus
		SetPos();
	}
	else if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT) ||
		App->input->GetWheel() != 0) {
		//zoom
		int* mouseY = new int(1);
		SDL_GetMouseState(NULL, mouseY);

		int wheelMotion = App->input->GetWheel();

		if (mouseY[0] > lastYmouse + 1 || wheelMotion < 0) {
			Zoom(speed);
			lastYmouse = mouseY[0];
		}
		else if (mouseY[0] < lastYmouse - 1 || wheelMotion > 0) {
			Zoom(-speed);
			lastYmouse = mouseY[0];
		}
		delete mouseY;
		mouseY = nullptr;
	}
	//up and down 
	else if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)) {
		VerticalMove(speed);
	}
	else if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)) {
		VerticalMove(-speed);
	}
	//forward and backward
	else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)) {
		Move(speed);
	}
	else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)) {
		Move(-speed);
	}
	//lateral move with A and D
	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)) {
		SideMove(speed);
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)) {
		SideMove(-speed);
	}
	//pitch arrows
	else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)) {
		RotateX(speed * 3);
	}
	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)) {
		RotateX(-speed * 3);
	}
	//yaw arrows
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)) {
		RotateY(-speed * 3);
	}
	else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT &&
		(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)) {
		RotateY(speed * 3);
	}
	//vertical + hotizontal move mouse
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT) {
		int* mouseY = new int(1);
		int* mouseX = new int(1);
		SDL_GetMouseState(NULL, mouseY);
		SDL_GetMouseState(mouseX, NULL);

		if (mouseY[0] > lastYmouse + 1) {
			VerticalMove((speed * 0.3f));
			lastYmouse = mouseY[0];
		}
		else if (mouseY[0] < lastYmouse - 1) {
			VerticalMove(-(speed * 0.3f));
			lastYmouse = mouseY[0];
		}

		if (mouseX[0] > lastXmouse + 1) {
			SideMove((speed * 0.4f));
			lastXmouse = mouseX[0];
		}
		else if (mouseX[0] < lastXmouse - 1) {
			SideMove(-(speed) * 0.4f);
			lastXmouse = mouseX[0];
		}
		delete mouseY;
		delete mouseX;
		mouseY = nullptr;
		mouseX = nullptr;
	}
	//Orbit
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT)) {
		int* mouseY = new int(1);
		int* mouseX = new int(1);
		SDL_GetMouseState(NULL, mouseY);
		SDL_GetMouseState(mouseX, NULL);

		if (mouseX[0] > lastXmouse + 1) {
			Orbit(degree);
			lastXmouse = mouseX[0];
		}
		else if (mouseX[0] < lastXmouse - 1) {
			Orbit(-degree);
			lastXmouse = mouseX[0];
		}
	}
	//pitch + yaw mouse
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
		int* mouseY = new int(1);
		int* mouseX = new int(1);
		SDL_GetMouseState(NULL, mouseY);
		SDL_GetMouseState(mouseX, NULL);

		if (mouseY[0] > lastYmouse + 1) {
			RotateX(speed * 2);
			lastYmouse = mouseY[0];
		}
		else if (mouseY[0] < lastYmouse - 1) {
			RotateX(-speed * 2);
			lastYmouse = mouseY[0];
		}

		if (mouseX[0] > lastXmouse + 1) {
			RotateY(speed * 2);
			lastXmouse = mouseX[0];
		}
		else if (mouseX[0] < lastXmouse - 1) {
			RotateY(-speed * 2);
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

	vec newFront = (frustum.Front() * cos(degrees * DEGTORAD) + frustum.Up() * sin(degrees * DEGTORAD)).Normalized();
	frustum.SetFront(newFront);

	vec newUp = frustum.WorldRight().Cross(newFront);
	frustum.SetUp(newUp);
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

void ModuleCamera::Orbit(double dir) {

	double rotatedX = (math::Cos(dir * DEGTORAD) * frustum.Pos().x) - (math::Sin(dir * DEGTORAD) * frustum.Pos().z);
	double rotatedZ = (math::Sin(dir * DEGTORAD) * frustum.Pos().x) + (math::Cos(dir * DEGTORAD) * frustum.Pos().z);

	frustum.SetPos({ (float)rotatedX, frustum.Pos().y , (float)rotatedZ });

	lookAtModel();
}

void ModuleCamera::SetFOV() {
	frustum.SetHorizontalFovAndAspectRatio(frustum.HorizontalFov(), aspectRatio);
}

void ModuleCamera::lookAtModel() {
	double modelPosX = App->model->GetCenter().x;

	vec pos = frustum.Pos();
	vec modelPos = { App->model->GetCenter().x, 0 , App->model->GetCenter().z };

	vec forward = modelPos - pos;
	forward.Normalize();

	vec newUp = Cross(Cross(forward, float3::unitY), forward);

	frustum.SetUp(newUp);
	frustum.SetFront(forward);
}

void ModuleCamera::SetPos() {

	double camPosZ = App->model->GetCenter().z;
	double outerZ = App->model->GetOuterZ();
	double modelPosX = App->model->GetCenter().x;

	LOG("modelPosZ = %f, outer z = %f", camPosZ, outerZ);

	vec pos = float3(modelPosX, 5, -(camPosZ + outerZ) * 5);
	frustum.SetPos(pos);

	lookAtModel();
}
