cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

set(PROJECT_NAME VoxelEngine)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} 
	src/main.cpp
	src/common/Log.hpp
	
	src/OpenGL/ShaderProgram.cpp
	src/OpenGL/ShaderProgram.hpp
	
	src/OpenGL/VertexBuffer.cpp
	src/OpenGL/VertexBuffer.hpp
	
	src/OpenGL/VertexArray.cpp
	src/OpenGL/VertexArray.hpp
	
	src/OpenGL/IndexBuffer.cpp
	src/OpenGL/IndexBuffer.hpp
	
	src/Object/Block.cpp
	src/Object/Block.hpp
	
	src/common/ImGuiWrapper.cpp
	src/common/ImGuiWrapper.hpp

)

target_include_directories(${PROJECT_NAME} PUBLIC src)


set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)


# add_subdirectory(external/glfw)
# target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)

add_subdirectory(external/spdlog)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog)

include_directories(external/glm)


set(GLFWPP_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("external/glfwpp")
target_link_libraries(${PROJECT_NAME} PRIVATE GLFWPP)


set(IMGUI_INCLUDES
	external/imgui/imgui.h
	external/imgui/backends/imgui_impl_glfw.h
	external/imgui/backends/imgui_impl_opengl3.h
)

set(IMGUI_SOURCES
	external/imgui/imgui.cpp
	external/imgui/backends/imgui_impl_glfw.cpp
	external/imgui/backends/imgui_impl_opengl3.cpp
	external/imgui/imgui_demo.cpp
	external/imgui/imgui_tables.cpp
	external/imgui/imgui_widgets.cpp
	external/imgui/imgui_draw.cpp
)

add_library(ImGui STATIC
	${IMGUI_INCLUDES}
	${IMGUI_SOURCES}
)


target_include_directories(ImGui PUBLIC external)
target_include_directories(ImGui PUBLIC external/imgui)
target_link_libraries(ImGui PRIVATE glad glfw)

target_link_libraries(${PROJECT_NAME} PRIVATE ImGui)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
