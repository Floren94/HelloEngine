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
	bool Start() { SetPos(); return true; }
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
	void SetFOV();
	void SetPos();

	float4x4 GetProjection() { return projectionGL; }
	float4x4 GetView() { return viewMatrix; }
	float3 GetPosition() { return frustum.Pos(); }
	float3 GetUp() { return frustum.Up(); }
	float3 GetFront() { return frustum.Front(); }

private:
	int lastYmouse = 0;
	int lastXmouse = 0;
	double aspectRatio;
	double deltaTime;
	bool startDelta = false;
	const double DEG = 0.5f;
	const double SPEED = 0.1f;
	const float DEGTORAD = (math::pi / 180);
	Frustum frustum;
	float4x4 projectionGL, viewMatrix;
};
