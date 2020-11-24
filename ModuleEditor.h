#pragma once
#include "Module.h"
#include "Globals.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include <vector>

class ModuleEditor : public Module
{
public:
	ModuleEditor();
	~ModuleEditor();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void RenderUi();
	void AddLog(const char* fmt);

	bool isFocused = false;

private :

	ImGuiTextBuffer textBuffer;
	std::vector<float> fps_vec;
	int frames = 0;
	bool scrollToBottom = false; 
	bool config = true;
	bool console = true;
	bool properties = true;
	int w = 0, h = 0;

};

