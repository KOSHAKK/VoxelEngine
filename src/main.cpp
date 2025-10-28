#include "common/Log.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "OpenGL/ShaderProgram.hpp"

#include "OpenGL/VertexBuffer.hpp"
#include "OpenGL/VertexArray.hpp"

#include "Object/Block.hpp"


int g_windowSizeX = 640;
int g_windowSizeY = 480;

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_windowSizeX = width;
    g_windowSizeY = height;
    glViewport(0, 0, g_windowSizeX, g_windowSizeY);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main(const int argc, const char** argv)
{
    spdlog::set_pattern("%^[%l]%$ %v");

    /* Initialize the library */
    if (!glfwInit())
    {
        LOG_CRITICAL("glfwInit failed!");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* pWindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "VoxelEngine", nullptr, nullptr);
    if (!pWindow)
    {
        LOG_CRITICAL("glfwCreateWindow failed!");
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGL())
    {
        LOG_CRITICAL("Can't load GLAD!");
    }
    
    LOG_INFO("Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    LOG_INFO("OpenGL version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));


    glClearColor(0, 0, 0, 1);
   // glEnable(GL_DEPTH_TEST);

    const char* src_vert = R"(
        #version 460
        
        layout(location = 0) in vec3 vertex_position;
        layout(location = 1) in vec3 vertex_color;
        out vec3 color;
        
        uniform mat4 scale_matrix;
        uniform mat4 pos_matrix;

        void main()
        {
           color = vertex_color;
        
           gl_Position =  (pos_matrix * scale_matrix) * vec4(vertex_position, 1.0);
        
        }
    )";

    const char* src_frag = R"(
       #version 460

        in vec3 color;
        out vec4 frag_color;
        
        void main()
        {
           frag_color = vec4(color, 1.0);
        }
    )";


    ShaderProgram proj(src_vert, src_frag);

    Block b1({1.1f, 1.1f, 1.1f}, { 1.0f, 1.0f, 1.0f });
    Block b2({1.1f, 1.1f, 1.1f}, { 1.0f, 1.0f, 1.0f });


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(pWindow))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        b1.draw(proj);
        b2.draw(proj);

        b1.set_position({ cos(glfwGetTime())/2, sin(glfwGetTime()) / 2, 0.f });
        b1.set_scale({ cos(glfwGetTime()), sin(glfwGetTime()), 0.f });


        b2.set_position({ sin(glfwGetTime()) / 2, cos(glfwGetTime()) / 2, 0.f });
        b2.set_scale({ sin(glfwGetTime()), cos(glfwGetTime()), 0.f });


        /* Swap front and back buffers */
        glfwSwapBuffers(pWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}