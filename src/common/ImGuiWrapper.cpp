#include "ImGuiWrapper.hpp"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>


void ImGuiWrapper::init_imgui(glfw::Window& pWindow)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(pWindow, true);
    ImGui_ImplOpenGL3_Init();

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = std::get<0>(pWindow.getSize());
    io.DisplaySize.y = std::get<1>(pWindow.getSize());
}


void ImGuiWrapper::update_imgui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

	ImGui::Begin("Debug window");



	ImGui::ColorEdit3("Clear color", ImGuiWrapper::clear_color);
	ImGui::DragFloat3("Position", ImGuiWrapper::debug_position, 0.01f, -1.f, 1.f);
	ImGui::DragFloat3("Scale", ImGuiWrapper::debug_scale, 0.01f, -1.f, 1.f);


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
