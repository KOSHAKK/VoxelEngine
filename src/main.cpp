#include "common/Log.hpp"

#include <glad/glad.h>
#include <glfwpp/glfwpp.h>

#include <iostream>

#include "OpenGL/ShaderProgram.hpp"

#include "OpenGL/VertexBuffer.hpp"
#include "OpenGL/VertexArray.hpp"

#include "Object/Block.hpp"

#include <Render/Camera.hpp>

#include <common/ImGuiWrapper.hpp>


#include <Physics/PhysicsEngine.hpp>

int g_windowSizeX = 640*2;
int g_windowSizeY = 480*2-150;

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_windowSizeX = width;
    g_windowSizeY = height;
    glViewport(0, 0, g_windowSizeX, g_windowSizeY);
}

bool jump = false;
void glfwKeyCallback(glfw::Window& pWindow, glfw::KeyCode keyCode_, int scanCode_, glfw::KeyState state_, glfw::ModifierKeyBit modifiers_)
{
    if (keyCode_ == glfw::KeyCode::Escape && state_ == glfw::KeyState::Press)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
    if (keyCode_ == glfw::KeyCode::W && state_ == glfw::KeyState::Press)
    {
        jump = true;
	}
}


int main(const int argc, const char** argv)
{



    spdlog::set_pattern("%^[%l]%$ %v");

    [[maybe_unused]] auto GLFW = glfw::init();

    glfw::WindowHints{ .contextVersionMajor = 4,
                        .contextVersionMinor = 6,
                        .openglProfile = glfw::OpenGlProfile::Core }.apply();

    glfw::Window pWindow{ g_windowSizeX, g_windowSizeY, "VoxelEngine" };




    pWindow.framebufferSizeEvent.setCallback(glfwWindowSizeCallback);

    pWindow.keyEvent.setCallback(glfwKeyCallback);



    glfw::makeContextCurrent(pWindow);

    if (!gladLoadGL())
    {
        LOG_CRITICAL("Can't load GLAD!");
    }
    

    LOG_INFO("Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    LOG_INFO("OpenGL version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));


    glEnable(GL_DEPTH_TEST);

    const char* src_vert = R"(
        #version 460
        
        layout(location = 0) in vec3 vertex_position;
        layout(location = 1) in vec3 vertex_color;
        out vec3 color;
        
        uniform mat4 MVP;
        uniform float time;

        void main()
        {
           color = vertex_color;
        
           gl_Position = MVP * vec4(vertex_position, 1.0);

        }
    )";

    const char* src_frag = R"(
        #version 460

        in vec3 color;
        out vec4 frag_color;
        uniform float time;
        void main()
        {
            frag_color = vec4(color, 1.0);
        }
    )";


    ShaderProgram proj(src_vert, src_frag);

    Camera camera;


    Block b1;
    Block b2;
    Block b3({ 2.0f, 1.0f, -1.0f });



    ImGuiWrapper::init_imgui(pWindow);


    const float cDeltaTime = 1.0f / 60.0f;

    PhysicsEngine::init();
    JPH::uint step = 0;

    JPH::TempAllocatorImpl  temp_allocator{ 10 * 1024 * 1024 };

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(pWindow))
    {
		JPH::RVec3 position(0.f, 0.f, 0.f);

        if (PhysicsEngine::body_interface->IsActive(PhysicsEngine::cube_id)) {
            ++step;

            position = PhysicsEngine::body_interface->GetCenterOfMassPosition(PhysicsEngine::cube_id);
            JPH::Vec3 velocity = PhysicsEngine::body_interface->GetLinearVelocity(PhysicsEngine::cube_id);
            std::cout << "Step " << step << ": Position = (" << position.GetX() << ", " << position.GetY() << ", " << position.GetZ() << "), Velocity = (" << velocity.GetX() << ", " << velocity.GetY() << ", " << velocity.GetZ() << ")" << std::endl;


            PhysicsEngine::physics_system.Update(1.f / 60.f, 1, &temp_allocator, PhysicsEngine::job_system);
        }

		b3.set_position({ position.GetX(), position.GetY(), position.GetZ() });

        if (jump)
        {
            JPH::Vec3 vel = PhysicsEngine::body_interface->GetLinearVelocity(PhysicsEngine::cube_id);
            vel.SetY(5.0f);
            PhysicsEngine::body_interface->SetLinearVelocity(PhysicsEngine::cube_id, vel);
            jump = false;
		}
        


        /* Render here */
        glClearColor(ImGuiWrapper::clear_color[0], ImGuiWrapper::clear_color[1], ImGuiWrapper::clear_color[2], 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        proj.bind();
		proj.set_float("time", static_cast<float>(glfw::getTime()));


        b1.draw(proj, camera);
        //b2.draw(proj, camera);
        b3.draw(proj, camera);

        b1.set_position({ ImGuiWrapper::debug_block_position[0], ImGuiWrapper::debug_block_position[1], ImGuiWrapper::debug_block_position[2] });
        b1.set_scale({ ImGuiWrapper::debug_block_scale[0], ImGuiWrapper::debug_block_scale[1], ImGuiWrapper::debug_block_scale[2] });
		b1.set_rotation({ ImGuiWrapper::debug_block_rotation[0], ImGuiWrapper::debug_block_rotation[1], ImGuiWrapper::debug_block_rotation[2] });


		camera.set_position({ ImGuiWrapper::debug_camera_position[0], ImGuiWrapper::debug_camera_position[1], ImGuiWrapper::debug_camera_position[2] });
		camera.set_rotation({ ImGuiWrapper::debug_camera_rotation[0], ImGuiWrapper::debug_camera_rotation[1], ImGuiWrapper::debug_camera_rotation[2] });
        
        if (ImGuiWrapper::perspective_mode)
            camera.set_projection_mode(Camera::ProjectionMode::Perspective);
        else
            camera.set_projection_mode(Camera::ProjectionMode::Orthographic);

        ImGuiWrapper::update_imgui();


        pWindow.swapBuffers();
        glfw::pollEvents();


    }
    ImGuiWrapper::destroy_imgui_context();
    return 0;
}
