#pragma once
#include "Module.h"
#include "Globals.h"
#include "il.h"
#include "ilu.h"

class ModuleTexture : public Module
{
public:
	ModuleTexture() {};
	~ModuleTexture() {};

	bool Init();
	update_status Update();
	bool CleanUp();

	unsigned LoadTexture(const char* file_name);
	void FreeTexture(unsigned textID);

	int GetWidth() { return width; }
	int GetHeight() { return height; }
private:
	int width = 0, height = 0;
};