#include <glad/gl.h>                  
#include <GLFW/glfw3.h>                 

#include <imgui.h>

#include "ImGuiWrapper.hpp"

#include <common/Log.hpp>

#include <bindings/imgui_impl_glfw.h>
#include <bindings/imgui_impl_opengl3.h>

#include <Resources/ResourceManager.hpp>

void ImGuiWrapper::init_imgui(glfw::Window& pWindow)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(pWindow, true);
    ImGui_ImplOpenGL3_Init();

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = static_cast<float>(std::get<0>(pWindow.getSize()));
    io.DisplaySize.y = static_cast<float>(std::get<1>(pWindow.getSize()));


	ResourceManager::load_font("ARCADECLASSIC", "res/Fonts/ProggyClean.ttf", 16.f);

    io.Fonts->Build();
}


void ImGuiWrapper::update_imgui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

	ImGui::Begin("Debug window");

    float fps = ImGui::GetIO().Framerate;

    ImGui::Text("FPS: %.1f", fps);


	ImGui::ColorEdit3("Clear color", ImGuiWrapper::clear_color);
    ImGui::Checkbox("Draw line", &ImGuiWrapper::draw_line);
	ImGui::Separator();
	ImGui::Checkbox("Perspective mode", &ImGuiWrapper::perspective_mode);
    ImGui::Separator();
    ImGui::Text("%s", camera_pos_string.c_str());
	ImGui::DragFloat("Camera speed", &ImGuiWrapper::camera_speed, 1.0f, 10.f, 50.f);
    ImGui::DragFloat("Camera fov", &ImGuiWrapper::camera_fov, 1, 30.f, 120.f);
    ImGui::SliderInt2("Camera sensivity", &ImGuiWrapper::camera_sensivity.x, 10, 100);

    ImGui::Separator();
    ImGui::Text("World settings");
    ImGui::SliderInt3("World size", &world_size.x, 1, 10);
	ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void ImGuiWrapper::destroy_imgui_context()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
