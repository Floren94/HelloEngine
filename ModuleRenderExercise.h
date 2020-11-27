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
	bool CleanUp();

	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();

	void* GetContext() { return context; }
	void LoadDropModel(char* filename);

private:
	void* context;/*
	unsigned int triangle;
	unsigned int program;*/

};
