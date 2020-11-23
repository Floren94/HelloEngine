#pragma once
#include "Module.h"
#include "Globals.h"


class ModuleRenderExercise : public Module
{
public:
	ModuleRenderExercise();
	~ModuleRenderExercise();

	bool Init();
	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void* GetContext() { return context; }

private:
	void* context;/*
	unsigned int triangle;
	unsigned int program;*/

};
