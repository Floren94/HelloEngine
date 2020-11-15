#include "Globals.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "SDL.h"
#include "GL/glew.h"


ModuleProgram::ModuleProgram()
{
}

// Destructor
ModuleProgram::~ModuleProgram()
{
}


// Called before quitting
bool ModuleProgram::CleanUp()
{
	LOG("Destroying Program");

	return true;
}
