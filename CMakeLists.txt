cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

cmake_policy(SET CMP0091 NEW)


set(PROJECT_NAME VoxelEngine)
project(${PROJECT_NAME})


set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE PROJECT_SOURCES
    CONFIGURE_DEPENDS
    src/*.cpp
    src/*.hpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})


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

find_package(spdlog REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)

find_package(glm REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)

find_package(imgui REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)

find_package(Jolt REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Jolt::Jolt)


if (MSVC)
    target_compile_options(GLFWPP INTERFACE /W0)
else()
    target_compile_options(GLFWPP INTERFACE -w)
endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)