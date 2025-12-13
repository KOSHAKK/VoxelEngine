#include <iostream>

#include <glad/glad.h>
#include <glfwpp/glfwpp.h>


#include <common/Log.hpp>
#include <common/ImGuiWrapper.hpp>
#include <common/Input.hpp>

#include <Resources/ResourceManager.hpp>

#include <OpenGL/ShaderProgram.hpp>
#include <OpenGL/VertexBuffer.hpp>
#include <OpenGL/VertexArray.hpp>


#include <Object/Block.hpp>


#include <Physics/PhysicsEngine.hpp>


#include <Render/Camera.hpp>


#include <glm/gtc/quaternion.hpp>

using namespace JPH::literals;


int g_windowSizeX = 640*2;
int g_windowSizeY = 480*2-150;

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_windowSizeX = width;
    g_windowSizeY = height;
    glViewport(0, 0, g_windowSizeX, g_windowSizeY);
}

void glfwKeyCallback(glfw::Window& pWindow, glfw::KeyCode keyCode_, int scanCode_, glfw::KeyState state_, glfw::ModifierKeyBit modifiers_)
{
    if (state_ == glfw::KeyState::Press) {
        Input::PressKey(static_cast<KeyCode>(static_cast<int>(keyCode_)));
    }
    else if (state_ == glfw::KeyState::Release) {
        Input::ReleaseKey(static_cast<KeyCode>(static_cast<int>(keyCode_)));
    }

    if (keyCode_ == glfw::KeyCode::Escape) {
        glfwSetWindowShouldClose(pWindow, GLFW_TRUE);
    }
}


int main(const int argc, const char** argv) try
{
    ResourceManager::init(argv[0]);
    PhysicsEngine::init();

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

    auto proj = ResourceManager::load_shader_program("basic_shader", "res/Shaders/basic.vert", "res/Shaders/basic.frag");


    Camera camera;

    ResourceManager::load_texture("stone_brick_texture", "res/Textures/stone_brick.png");

    Block floor({ 0.f,0.f,0.f }, { 100.f, 1.f, 100.f });
    Block b3({ 2.0f, 1.0f, -1.0f });

    Block b1;


    PhysicsEngine::add_object({ JPH::RVec3(0.0_r, 8.0_r, 0.0_r), JPH::Vec3(1.0f, 1.0f, 1.0f), JPH::EMotionType::Dynamic }, "cube1");
    PhysicsEngine::add_object({ JPH::RVec3(0.8_r, 12.0_r, 0.8_r), JPH::Vec3(1.0f, 1.0f, 1.0f), JPH::EMotionType::Dynamic }, "cube2");
    PhysicsEngine::add_object({ JPH::RVec3(0.0_r, -1.0_r, 0.0_r), JPH::Vec3(100.0f, 2.0f, 100.0f), JPH::EMotionType::Static }, "floor");


    ImGuiWrapper::init_imgui(pWindow);



    while (!glfwWindowShouldClose(pWindow))
    {
        static double lastTime = glfwGetTime();
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);
        lastTime = currentTime;

        
        if (Input::IsKeyPressed(KeyCode::KEY_W)) {
			ImGuiWrapper::debug_camera_position[2] -= 5.0f * deltaTime;
        }
		else if (Input::IsKeyPressed(KeyCode::KEY_S)) {
            ImGuiWrapper::debug_camera_position[2] += 5.0f * deltaTime;
		}

        if (Input::IsKeyPressed(KeyCode::KEY_A)) {
            ImGuiWrapper::debug_camera_position[0] -= 5.0f * deltaTime;
        }
        else if (Input::IsKeyPressed(KeyCode::KEY_D)) {
            ImGuiWrapper::debug_camera_position[0] += 5.0f * deltaTime;
        }

        if (Input::IsKeyPressed(KeyCode::KEY_LEFT_SHIFT)) {
            ImGuiWrapper::debug_camera_position[1] += 5.0f * deltaTime;
        }
        else if (Input::IsKeyPressed(KeyCode::KEY_LEFT_CONTROL)) {
            ImGuiWrapper::debug_camera_position[1] -= 5.0f * deltaTime;
        }


        PhysicsEngine::update(deltaTime);

        auto cube_id = PhysicsEngine::get_object("cube1");
        JPH::Vec3 position = PhysicsEngine::body_interface->GetPosition(cube_id);
		b3.set_position({ position.GetX(), position.GetY(), position.GetZ() });
        JPH::RVec3 p = PhysicsEngine::body_interface->GetPosition(cube_id);
        JPH::Quat q = PhysicsEngine::body_interface->GetRotation(cube_id);

        glm::quat gq(q.GetW(), q.GetX(), q.GetY(), q.GetZ());
        glm::mat4 R = glm::mat4_cast(gq);
        b3.set_rotation_matrix(R); 




        auto cube2_id = PhysicsEngine::get_object("cube2");
        JPH::Vec3 position2 = PhysicsEngine::body_interface->GetPosition(cube2_id);
        b1.set_position({ position2.GetX(), position2.GetY(), position2.GetZ() });
        JPH::RVec3 p2 = PhysicsEngine::body_interface->GetPosition(cube2_id);
        JPH::Quat q2 = PhysicsEngine::body_interface->GetRotation(cube2_id);
       
        glm::quat gq2(q2.GetW(), q2.GetX(), q2.GetY(), q2.GetZ());
        glm::mat4 R2 = glm::mat4_cast(gq2);
        b1.set_rotation_matrix(R2);


        JPH::Vec3 vel = PhysicsEngine::body_interface->GetLinearVelocity(cube_id);

        if (Input::IsKeyPressed(KeyCode::KEY_UP)) {
            vel.SetZ(-ImGuiWrapper::push_strength);
            PhysicsEngine::body_interface->SetLinearVelocity(cube_id, vel);
        }
        if (Input::IsKeyPressed(KeyCode::KEY_DOWN)) {
            vel.SetZ(ImGuiWrapper::push_strength);
            PhysicsEngine::body_interface->SetLinearVelocity(cube_id, vel);
        }
        if (Input::IsKeyPressed(KeyCode::KEY_LEFT)) {
            vel.SetX(-ImGuiWrapper::push_strength);
            PhysicsEngine::body_interface->SetLinearVelocity(cube_id, vel);
        }
        if (Input::IsKeyPressed(KeyCode::KEY_RIGHT)) {
            vel.SetX(ImGuiWrapper::push_strength);
            PhysicsEngine::body_interface->SetLinearVelocity(cube_id, vel);
        }
        if (Input::IsKeyPressed(KeyCode::KEY_SPACE)) {
            vel.SetY(3.0f);
            PhysicsEngine::body_interface->SetLinearVelocity(cube_id, vel);
        }


        /* Render here */
        glClearColor(ImGuiWrapper::clear_color[0], ImGuiWrapper::clear_color[1], ImGuiWrapper::clear_color[2], 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        proj->bind();
		proj->set_float("time", static_cast<float>(glfw::getTime()));


        b1.draw(proj, camera);
        b3.draw(proj, camera);
        floor.draw(proj, camera);






#if 0
        b1.set_position({ ImGuiWrapper::debug_block_position[0], ImGuiWrapper::debug_block_position[1], ImGuiWrapper::debug_block_position[2] });
        b1.set_scale({ ImGuiWrapper::debug_block_scale[0], ImGuiWrapper::debug_block_scale[1], ImGuiWrapper::debug_block_scale[2] });
		b1.set_rotation({ ImGuiWrapper::debug_block_rotation[0], ImGuiWrapper::debug_block_rotation[1], ImGuiWrapper::debug_block_rotation[2] });
#endif

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
catch (const std::exception& e)
{
    LOG_CRITICAL("Unhandled exception: {}", e.what());
    return EXIT_FAILURE;
}