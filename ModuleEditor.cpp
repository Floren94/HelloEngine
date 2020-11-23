#pragma once

#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleRenderExercise.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "GL/glew.h"

ModuleEditor::ModuleEditor()
{
}

// Destructor
ModuleEditor::~ModuleEditor()
{
}

// Called before Editor is available
bool ModuleEditor::Init()
{
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->rendererExercise->GetContext());
    ImGui_ImplOpenGL3_Init();

    return true;
}

// Called before quitting
bool ModuleEditor::CleanUp()
{
    LOG("Destroying Editor");
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    return true;
}

update_status ModuleEditor::PreUpdate()
{

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App->window->window);
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();
    return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update()
{
    ImGui::ShowDemoWindow(0);

    ImGui::Begin("Test");
    ImGui::Text("Test Window");
    ImGui::SetWindowSize({ 300,400 });
    ImGui::End();

    return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate()
{
    return UPDATE_CONTINUE;
}


void ModuleEditor::RenderUi()
{
    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

