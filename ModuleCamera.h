#pragma once
#include "Module.h"
#include "Globals.h"
#include "GL/glew.h"
#include "src/Geometry/Frustum.h"
#include "src/Math/float3x3.h"

class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void VerticalMove(double dir);
	void Move(double dir);
	void SideMove(double dir);
	void RotateX(double degree);
	void RotateY(double degree);
	void Zoom(double dir);

private:
	Frustum frustum;
	int lastYmouse = 0;
	int lastXmouse = 0;
	double deltaTime;
	const double DEG = 0.5f;
	const double SPEED = 0.01f;
	bool drag = false;
	const float DEGTORAD = (3.1216f / 180);
};