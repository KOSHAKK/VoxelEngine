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
)

target_include_directories(${PROJECT_NAME} PUBLIC src)


set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)


add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

add_subdirectory(external/spdlog)
target_link_libraries(${PROJECT_NAME} spdlog)

include_directories(external/glm)



set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
