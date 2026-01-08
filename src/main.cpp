#include <iostream>

#include <glad/gl.h>
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

#include <Object/LightSource.hpp>

#include <Voxel/Chunk.hpp>
#include <Render/VoxelMesher.hpp>
#include <Object/Mesh.hpp>

#include <Voxel/World.hpp>

#include <glm/gtc/quaternion.hpp>

#include <chrono>


using namespace JPH::literals;


static int g_windowSizeX = 640*2;
static int g_windowSizeY = 480*2-150;

float ascpect = static_cast<float>(g_windowSizeX) / static_cast<float>(g_windowSizeY);

static void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_windowSizeX = width;
    g_windowSizeY = height;
    ascpect =  static_cast<float>(g_windowSizeX) / static_cast<float>(g_windowSizeY);
    glViewport(0, 0, g_windowSizeX, g_windowSizeY);
}

static void glfwKeyCallback(glfw::Window& pWindow, glfw::KeyCode keyCode_, int scanCode_, glfw::KeyState state_, glfw::ModifierKeyBit modifiers_)
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

void glfwMouseButtonCallback(glfw::Window& window_, glfw::MouseButton button_, glfw::MouseButtonState state_, glfw::ModifierKeyBit modifiers_)
{
    if (state_ == glfw::MouseButtonState::Press) {
        Input::PressMouseButton(static_cast<MouseButton>(static_cast<int>(button_)));
    }
    else if (state_ == glfw::MouseButtonState::Release) {
        Input::ReleaseMouseButton(static_cast<MouseButton>(static_cast<int>(button_)));
    }
}

bool is_mouse_moving = false;
glm::vec2 mouse_delta;
void glfwMousePosCallback(glfw::Window& window_, double xpos, double ypos)
{
    static glm::vec2 mouse_prev_pos;

    double x;
    double y;

    glfwGetCursorPos(window_, &x, &y);

    mouse_delta.x = (mouse_prev_pos.x - x) / 5.f;
    mouse_delta.y = (mouse_prev_pos.y - y) / 5.f;

    mouse_prev_pos = { x,y };

    is_mouse_moving = true;
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
    pWindow.mouseButtonEvent.setCallback(glfwMouseButtonCallback);
    pWindow.cursorPosEvent.setCallback(glfwMousePosCallback);

    glfw::makeContextCurrent(pWindow);

    if (!gladLoadGL(glfwGetProcAddress))
    {
        LOG_CRITICAL("Can't load GLAD!");
    }
    
    ImGuiWrapper::init_imgui(pWindow);

    LOG_INFO("Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    LOG_INFO("OpenGL version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));


    glEnable(GL_DEPTH_TEST);

    Camera camera;
    camera.set_position({ 8.f, 8.f, 24.f });

     
    ResourceManager::load_texture("debug_texture", "res/Textures/block.png");
    auto shared = ResourceManager::load_shader_program("voxel_shared", "res/Shaders/main.glslv", "res/Shaders/main.glslf");


   // Block mesh("debug_texture", { 0.f, 0.f, 0.f }, {1.f, 1.f, 1.f});

    ImGuiWrapper::aspect = ascpect;

    const auto start{ std::chrono::steady_clock::now() };
    
    std::shared_ptr<World> w = std::make_shared<World>(2, 1, 2, "debug_texture");

    const auto finish{ std::chrono::steady_clock::now() };
    const std::chrono::duration<double> elapsed_seconds{ finish - start };
    LOG_INFO("World has been created for {}s", elapsed_seconds.count());

    //glfw::swapInterval(1);
    while (!glfwWindowShouldClose(pWindow))
    {
        ImGuiWrapper::aspect = ascpect;
        static double lastTime = glfwGetTime();
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);
        lastTime = currentTime;

        if (Input::IsKeyPressed(KeyCode::KEY_W)) {
            camera.move_forward(ImGuiWrapper::push_strength, deltaTime);
        }
        else if (Input::IsKeyPressed(KeyCode::KEY_S)) {
            camera.move_forward(-ImGuiWrapper::push_strength, deltaTime);
        }

        if (Input::IsKeyPressed(KeyCode::KEY_A)) {
            camera.move_right(-ImGuiWrapper::push_strength, deltaTime);
        }
        else if (Input::IsKeyPressed(KeyCode::KEY_D)) {
            camera.move_right(ImGuiWrapper::push_strength, deltaTime);
        }

        if (Input::IsKeyPressed(KeyCode::KEY_LEFT_SHIFT)) {
            camera.move_up(ImGuiWrapper::push_strength, deltaTime);
        }
        else if (Input::IsKeyPressed(KeyCode::KEY_LEFT_CONTROL)) {
            camera.move_up(-ImGuiWrapper::push_strength, deltaTime);
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
        shared->set_float("aspect_ratio", ascpect);



        w->draw(shared, camera);

        if (is_mouse_moving && Input::IsMouseButtonPressed(MouseButton::MOUSE_BUTTON_2)) {
            camera.set_rotate_delta(mouse_delta, deltaTime);
            is_mouse_moving = false;
        } 


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