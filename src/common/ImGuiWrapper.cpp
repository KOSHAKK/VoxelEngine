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
    io.DisplaySize.x = static_cast<float>(std::get<0>(pWindow.getSize()));
    io.DisplaySize.y = static_cast<float>(std::get<1>(pWindow.getSize()));
}


void ImGuiWrapper::update_imgui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

	ImGui::Begin("Debug window");



	ImGui::ColorEdit3("Clear color", ImGuiWrapper::clear_color);
    ImGui::Checkbox("Draw line", &ImGuiWrapper::draw_line);
	ImGui::Separator();
	ImGui::DragFloat3("Position", ImGuiWrapper::debug_block_position, 0.1f, -10.f, 10.f);
	ImGui::DragFloat3("Scale", ImGuiWrapper::debug_block_scale, 0.01f, -1.f, 1.f);
	ImGui::DragFloat3("Rotation", ImGuiWrapper::debug_block_rotation, 1.f, 0.f, 360.f);
    ImGui::Separator();
    ImGui::DragFloat3("Camera position", ImGuiWrapper::debug_camera_position, 0.1f, -10.f, 10.f);
    ImGui::DragFloat3("Camera rotation", ImGuiWrapper::debug_camera_rotation, 1.f, 0.f, 360.f);
	ImGui::Checkbox("Perspective mode", &ImGuiWrapper::perspective_mode);

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
