#pragma once

#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleRenderExercise.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleModel.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "GL/glew.h"
#include <sstream>


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
    fps_vec.clear();
    return true;
}

update_status ModuleEditor::PreUpdate()
{

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App->window->window);
    ImGui::NewFrame();
    return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update()
{
    //ImGui::ShowDemoWindow(0);

    static int seconds, milisecs, perfms;
    if (seconds == 30) {
        timer.StopTimer();
    }
    else {
        timer.StartTimer();
    }
    seconds = timer.GetTime() / 1000;

    timer.StartPerformanceTimer();
    if (seconds == 1) {
        perfms = milisecs;
    }

    float menuSize = 0.0f;
    static float framerateMax = 0;

    ImGuiIO& io = ImGui::GetIO();
    fps_vec.push_back(io.Framerate);
    frames++;

    SDL_GetWindowSize(App->window->window, &w, &h);
    float fW = (float)w;
    float fH = (float)h;

    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("Options")) {
        ImGui::MenuItem("Configuration Window", NULL, &config);
        ImGui::MenuItem("Console", NULL, &console);
        ImGui::MenuItem("Properties", NULL, &properties);
        ImGui::MenuItem("About", NULL, &about); 
        if (ImGui::MenuItem("GitHup Repository")) {
            ShellExecute(0, 0, "https://github.com/Floren94/HelloEngine", 0, 0, SW_SHOWNORMAL);
        }
        if (ImGui::MenuItem("Quit")) {
            return UPDATE_STOP;
        }
        ImGui::EndMenu();
    }
    menuSize = ImGui::GetWindowSize().y;
    ImGui::EndMainMenuBar();


    if (about) {
        ImGui::SetNextWindowPos({ (fW / 2) -200, fH / 2 - 100 });

        ImGui::Begin("About");
        ImGui::Text("This is Hello Engine, made by Floren Magrinya at UPC");

        ImGui::SetWindowSize({ 400, 50 });

        if (ImGui::IsWindowFocused()) isFocused = true;
        else isFocused = false;

        ImGui::End();
    }

    fW = fW / 5;
    fH = fH - (fH / 4);
    ImVec2 size(fW, fH);

    if (config) {

        ImVec2 pos(0.0f, menuSize);
        ImGui::SetNextWindowPos(pos);
        ImGui::Begin("Configuration", 0, ImGuiWindowFlags_NoMove);

        ImGui::Text("Timer: %g seconds", (double)seconds);
        ImGui::Text("DeltaTime: %g seconds", App->camera->GetDelta());

        ImVec2 mousePos = ImGui::GetMousePos();

        ImGui::Text("Mouse Position: %g %g", mousePos.x, mousePos.y);

        //ImGui::Text("Performance Timer: %g miliseconds", (double)perfms);

        ImGui::Text("FPS Graph");

        ImGui::PlotHistogram("250 fps", &fps_vec[0], fps_vec.size(), 0, "FPS", 0.0f, 250.0f, ImVec2(fW * 0.8f, 100));

        if (ImGui::CollapsingHeader("System Information")) {

            SDL_version compiled;

            SDL_VERSION(&compiled);

            ImGui::Text("CPUs: %d", SDL_GetCPUCount());
            ImGui::Text("RAM: %.2f GB", SDL_GetSystemRAM() / 1024.0f);
            ImGui::Text("SDL version compiled: %d.%d.%d", compiled.major, compiled.minor, compiled.patch);
            ImGui::Text("OpenGL version: %s", (char*)glGetString(GL_VERSION));
        }

        ImGui::SetWindowSize(size);

        if (ImGui::IsWindowFocused()) isFocused = true;
        else isFocused = false;

        ImGui::End();
    }

    if (properties) {
        fW = (float)w;
        fW = fW - (fW / 5);

        ImVec2 pos2(fW, menuSize);
        ImGui::SetNextWindowPos(pos2);
        ImGui::Begin("Properties", 0, ImGuiWindowFlags_NoMove);

        if (ImGui::CollapsingHeader("Camera Transformation")) {
            ImGui::Text("Front:    (%f, %f, %f)", App->camera->GetFront().x, App->camera->GetFront().y, App->camera->GetFront().z);
            ImGui::Text("Up:       (%f, %f, %f)", App->camera->GetUp().x, App->camera->GetUp().y, App->camera->GetUp().z);
            ImGui::Text("Position: (%f, %f, %f)", App->camera->GetPosition().x, App->camera->GetPosition().y, App->camera->GetPosition().z);
        }

        if (ImGui::CollapsingHeader("Geometry")) {

            ImGui::Text("Number of Meshes: %d", App->model->GetMeshes());
            if (ImGui::CollapsingHeader("Number of Vertices")) {
                for (unsigned i = 0; i < App->model->GetMeshes(); ++i) {
                    ImGui::Text("   Mesh %d: %d", i + 1, App->model->GetVertices(i));
                }
            }
            if (ImGui::CollapsingHeader("Number of Feces")) {
                for (unsigned i = 0; i < App->model->GetMeshes(); ++i) {
                    ImGui::Text("   Mesh %d: %d", i + 1, App->model->GetFaces(i));
                }
            }
        }

        if (ImGui::CollapsingHeader("Texture")) {
            int textWidth = App->model->GetTextureWidth();
            int textHeight = App->model->GetTextureHeight();

            ImGui::Text("Texture size: %d %d", textWidth, textHeight);
            ImGui::Image((ImTextureID)App->model->GetMaterial(0), { size.x * 0.8f, size.x * 0.8f });

        }

        ImGui::SetWindowSize(size);

        if (ImGui::IsWindowFocused() || isFocused) isFocused = true;
        else isFocused = false;

        ImGui::End();
    }

    if (console) {
        fH = (float)h;
        fH = fH - (fH / 4);

        ImVec2 pos3(0.0f, fH + menuSize);
        ImGui::SetNextWindowPos(pos3);
        ImGui::Begin("Console", 0, ImGuiWindowFlags_NoMove);
        ImGui::TextUnformatted(textBuffer.begin());
        fH = (float)h;
        fH = fH / 4;
        size = { (float)w,fH - menuSize };
        ImGui::SetWindowSize(size);

        if (ImGui::IsWindowFocused() || isFocused) isFocused = true;
        else isFocused = false;

        ImGui::End();
    }

    milisecs = timer.GetPerformanceTime();

    return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate()
{
    if (frames == 5) {
        fps_vec.erase(fps_vec.begin());
        frames = 0;
    }

    return UPDATE_CONTINUE;
}


void ModuleEditor::RenderUi()
{
    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ModuleEditor::AddLog(const char* fmt)
{
    va_list args;
    va_start(args, fmt);
    textBuffer.appendf(fmt, args);
    va_end(args);
    scrollToBottom = true;
}

