#pragma once
#include "Module.h"
#include "Globals.h"
#include "il.h"
#include "ilu.h"

class ModuleTexture : public Module
{
public:
	ModuleTexture();
	~ModuleTexture();

	bool Init();
	update_status Update();
	bool CleanUp();
	unsigned textureID;
};