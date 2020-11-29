#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow() {};
	~ModuleWindow() {};

	bool Init();
	bool CleanUp();

	void SetFlag(SDL_WindowFlags flag, bool active);

public:
	SDL_Window* window = NULL;

	SDL_Surface* screen_surface = NULL;
};

#endif // __ModuleWindow_H__