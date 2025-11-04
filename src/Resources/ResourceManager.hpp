#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include <OpenGL/ShaderProgram.hpp>

#include <imgui/imgui.h>

using shaderMap = std::unordered_map<std::string, ShaderProgram>;
using fontMap = std::unordered_map<std::string, ImFont*>;

class ResourceManager 
{
public:
	ResourceManager() = delete;

	static void init(const std::string& executable_path);
	static std::string get_file_text(const std::string& file_path);

	static ShaderProgram& load_shader_program(const std::string& name,
											  const std::string& vertex_shader_path,
											  const std::string& fragment_shader_path);


	static ShaderProgram& get_shader_program(const std::string& name);

	static ImFont& load_font(const std::string& name, const std::string& font_path, unsigned int size);



private:
	static inline std::string m_executable_path;
	static inline shaderMap m_shader_programs;
	static inline fontMap m_font;
};