#include "Application.h"
#include "ModuleTexture.h"
#include "ModuleProgram.h"
#include "il.h"
#include "ilu.h"

ModuleTexture::ModuleTexture()
{

}

// Destructor
ModuleTexture::~ModuleTexture()
{
}

// Called before render is available
bool ModuleTexture::Init()
{
	return true;
}


// Called every draw update
update_status ModuleTexture::Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleTexture::CleanUp()
{
	return true;
}