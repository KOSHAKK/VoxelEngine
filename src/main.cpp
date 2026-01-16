#include <iostream>
#include <chrono>


#include <glad/gl.h>
#include <glfwpp/glfwpp.h>

#include <Core/Window.hpp>


#include <common/Log.hpp>
#include <common/ImGuiWrapper.hpp>
#include <common/Input.hpp>

#include <Resources/ResourceManager.hpp>

#include <OpenGL/ShaderProgram.hpp>
#include <OpenGL/VertexBuffer.hpp>
#include <OpenGL/VertexArray.hpp>


#include <Physics/PhysicsEngine.hpp>


#include <Render/Camera.hpp>

#include <Voxel/Chunk.hpp>
#include <Render/VoxelMesher.hpp>
#include <Object/Mesh.hpp>

#include <Voxel/World.hpp>

#include <glm/gtc/quaternion.hpp>


using namespace JPH::literals;


int main(const int argc, const char** argv) try
{
    ResourceManager::init(argv[0]);
    PhysicsEngine::init();

    spdlog::set_pattern("%^[%l]%$ %v");

    [[maybe_unused]] auto GLFW = glfw::init();

    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << "GLFW Error " << error << ": " << description << std::endl;
    });

    Window window(1700, 950, "VoxelEngine");


    if (!gladLoadGL(glfwGetProcAddress))
    {
        LOG_CRITICAL("Can't load GLAD!");
    }
    
    ImGuiWrapper::init_imgui(window.get_window());

    LOG_INFO("Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    LOG_INFO("OpenGL version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
     

    glEnable(GL_DEPTH_TEST);

    Camera camera;
    camera.set_position({ 8.f, 8.f, 24.f });

     
    ResourceManager::load_texture("debug_texture", "res/Textures/block.png");
    auto shared = ResourceManager::load_shader_program("voxel_shared", "res/Shaders/main.glslv", "res/Shaders/main.glslf");

    glm::ivec3 world_size = ImGuiWrapper::world_size;
    

    const auto start{ std::chrono::steady_clock::now() };
    std::shared_ptr<World> w = std::make_shared<World>(world_size.x, world_size.y, world_size.z, "debug_texture");
    const auto finish{ std::chrono::steady_clock::now() };
    const std::chrono::duration<double> elapsed_seconds{ finish - start };
    LOG_INFO("World has been created for {}s", elapsed_seconds.count());


    //glfw::swapInterval(1);
    while (!glfwWindowShouldClose(window.get_window()))
    {
        camera.set_aspect_ratio(window.get_aspect());

        if (Input::IsKeyPressed(KeyCode::KEY_ESCAPE)) glfwSetWindowShouldClose(window.get_window(), GLFW_TRUE);


        if (world_size != ImGuiWrapper::world_size)
        {
            world_size = ImGuiWrapper::world_size;
            const auto start{ std::chrono::steady_clock::now() };
            w = std::make_shared<World>(world_size.x, world_size.y, world_size.z, "debug_texture");
            const auto finish{ std::chrono::steady_clock::now() };
            const std::chrono::duration<double> elapsed_seconds{ finish - start };
            LOG_INFO("World has been created for {}s", elapsed_seconds.count());
        }


        static double lastTime = glfwGetTime();
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);
        lastTime = currentTime;

        if (Input::IsKeyPressed(KeyCode::KEY_W)) {
            camera.move_forward(ImGuiWrapper::camera_speed, deltaTime);
        }
        else if (Input::IsKeyPressed(KeyCode::KEY_S)) {
            camera.move_forward(-ImGuiWrapper::camera_speed, deltaTime);
        }

        if (Input::IsKeyPressed(KeyCode::KEY_A)) {
            camera.move_right(-ImGuiWrapper::camera_speed, deltaTime);
        }
        else if (Input::IsKeyPressed(KeyCode::KEY_D)) {
            camera.move_right(ImGuiWrapper::camera_speed, deltaTime);
        }

        if (Input::IsKeyPressed(KeyCode::KEY_LEFT_SHIFT)) {
            camera.move_up(ImGuiWrapper::camera_speed, deltaTime);
        }
        else if (Input::IsKeyPressed(KeyCode::KEY_LEFT_CONTROL)) {
            camera.move_up(-ImGuiWrapper::camera_speed, deltaTime);
        }


        PhysicsEngine::update(deltaTime);


        ImGuiWrapper::camera_pos_string = std::to_string((int)camera.get_position().x) + " " + std::to_string((int)camera.get_position().y) + " " + std::to_string((int)camera.get_position().z);



        /* Render here */
        glClearColor(ImGuiWrapper::clear_color[0], ImGuiWrapper::clear_color[1], ImGuiWrapper::clear_color[2], 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //mesh.set_position({ImGuiWrapper::debug_light_position[0], ImGuiWrapper::debug_light_position[1], ImGuiWrapper::debug_light_position[2]});
        //mesh.set_scale({ImGuiWrapper::debug_light_scale[0], ImGuiWrapper::debug_light_scale[1], ImGuiWrapper::debug_light_scale[2]});


        //mesh.draw(shared, camera);
        shared->bind();

        w->draw(shared, camera);

        if (Input::IsMouseButtonPressed(MouseButton::MOUSE_BUTTON_2)) {
            camera.set_rotate_delta(Input::get_mouse_delta(), deltaTime);
        } 


        if (ImGuiWrapper::perspective_mode)
            camera.set_projection_mode(Camera::ProjectionMode::Perspective);
        else
            camera.set_projection_mode(Camera::ProjectionMode::Orthographic);

        ImGuiWrapper::update_imgui();


        window.get_window().swapBuffers();
        window.update();
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