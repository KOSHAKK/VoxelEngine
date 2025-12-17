cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

cmake_policy(SET CMP0091 NEW)


set(PROJECT_NAME VoxelEngine)
project(${PROJECT_NAME})


set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} 
	src/main.cpp
	
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
	
	src/Object/LightSource.hpp
	src/Object/LightSource.cpp
	
	src/common/ImGuiWrapper.cpp
	src/common/ImGuiWrapper.hpp
	
	src/common/Log.hpp
	
	src/common/Keys.hpp
	
	src/common/Input.cpp
	src/common/Input.hpp
	
	src/Render/Camera.cpp
	src/Render/Camera.hpp
	
	src/Render/Texture2D.cpp
	src/Render/Texture2D.hpp
	
	src/Physics/PhysicsEngine.cpp
	src/Physics/PhysicsEngine.hpp

	src/Resources/ResourceManager.cpp
	src/Resources/ResourceManager.hpp
	
	src/Resources/stb_image.hpp

	src/bindings/imgui_impl_glfw.cpp
    src/bindings/imgui_impl_glfw.h
    src/bindings/imgui_impl_opengl3.cpp
    src/bindings/imgui_impl_opengl3.h
    src/bindings/imgui_impl_opengl3_loader.h

)

# if (MSVC)
    # target_compile_options(GLFWPP INTERFACE /W0)
# else()
    # target_compile_options(GLFWPP INTERFACE -w)
# endif()


target_include_directories(${PROJECT_NAME} PUBLIC src)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

set(GLFWPP_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("external/glfwpp")
target_link_libraries(${PROJECT_NAME} PRIVATE GLFWPP)

find_package(glad REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)

# find_package(glfw3 REQUIRED)
# target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

find_package(spdlog REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)

find_package(glm REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)

find_package(imgui REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)

find_package(Jolt REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Jolt::Jolt)


# target_compile_definitions(${PROJECT_NAME} PRIVATE
    # JPH_DEBUG_RENDERER
	# JPH_FLOATING_POINT_EXCEPTIONS_ENABLED
	# JPH_PROFILE_ENABLED
	# JPH_OBJECT_STREAM
# )


set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)