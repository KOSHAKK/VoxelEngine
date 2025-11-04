#include "ResourceManager.hpp"

#include <sstream>
#include <fstream>

#include <common/Log.hpp>


void ResourceManager::init(const std::string& executable_path)
{
	size_t i = executable_path.find_last_of("/\\");
	m_executable_path = executable_path.substr(0, i);
}

std::string ResourceManager::get_file_text(const std::string& file_path)
{
	std::string path = m_executable_path + "/" + file_path;

	std::ofstream file;
	std::ifstream f;
	f.open(path);

	if (!f.is_open())
	{
		LOG_ERROR("Error open file {}", path);
	}

	std::stringstream ss;
	ss << f.rdbuf();
	return ss.str().c_str();
}

ShaderProgram& ResourceManager::load_shader_program(const std::string& name, const std::string& vertex_shader_path, const std::string& fragment_shader_path)
{
	std::string vs = get_file_text(vertex_shader_path);
	std::string fs = get_file_text(fragment_shader_path);


	return m_shader_programs.emplace(name, ShaderProgram(vs.c_str(), fs.c_str())).first->second;
}

ShaderProgram& ResourceManager::get_shader_program(const std::string& name)
{
	auto found = m_shader_programs.find(name);

	if (found != m_shader_programs.end())
	{
		return found->second;
	}
	LOG_ERROR("Can't find shader program with name: {}", name);
	throw std::runtime_error("Shader program not found: " + name);
}
