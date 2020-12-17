#pragma once
#include "Module.h"
#include "Globals.h"


class ModuleRenderExercise : public Module
{
public:
	ModuleRenderExercise() {};
	~ModuleRenderExercise() {};

	bool Init();
	bool Start();
	bool CleanUp();

	update_status Update();
	update_status PostUpdate();

	void* GetContext() const { return context; }
	unsigned int GetRenderText() {
		RenderToTexture();
		return renderedTexture; }
	void LoadDropModel(char* filename);

private:
	void* context;
	unsigned int renderedTexture;
	unsigned int depthrenderbuffer;

	void RenderToTexture();

};
