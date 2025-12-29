#include <glad/gl.h>                  
#include <GLFW/glfw3.h>                 

#include <imgui.h>

#include "ImGuiWrapper.hpp"


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
	ImGui::DragFloat3("Position", ImGuiWrapper::debug_light_position, 0.1f, -10.f, 10.f);
	ImGui::DragFloat3("Scale", ImGuiWrapper::debug_light_scale, 0.01f, -1.f, 1.f);
    ImGui::Separator();
	ImGui::Checkbox("Perspective mode", &ImGuiWrapper::perspective_mode);
	ImGui::DragFloat("Camera speed", &ImGuiWrapper::push_strength, 1.0f, 10.f, 50.f);

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
