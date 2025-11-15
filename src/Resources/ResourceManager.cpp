#include "ResourceManager.hpp"

#include <sstream>
#include <fstream>

#include <common/Log.hpp>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include <Resources/stb_image.hpp>

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

	if (!f.is_open()) {
		LOG_ERROR("Can't open file: {}", path);
		return "";
	}

	std::stringstream ss;
	ss << f.rdbuf();
	return ss.str().c_str();
}

std::shared_ptr<ShaderProgram> ResourceManager::load_shader_program(const std::string& name, const std::string& vertex_shader_path, const std::string& fragment_shader_path)
{
	std::string vs = get_file_text(vertex_shader_path);
	std::string fs = get_file_text(fragment_shader_path);


	return m_shader_programs.emplace(name, std::make_shared<ShaderProgram>(vs.c_str(), fs.c_str())).first->second;
}

std::shared_ptr<ShaderProgram> ResourceManager::get_shader_program(const std::string& name)
{
	auto found = m_shader_programs.find(name);

	if (found == m_shader_programs.end()) {
		LOG_ERROR("Can't find shader program with name: {}", name);
		return nullptr;
	}

	return found->second;
}

std::shared_ptr<Texture2D> ResourceManager::load_texture(const std::string& name, const std::string& texture_path)
{
	int channel{}, widht{}, height{};
	
	stbi_set_flip_vertically_on_load(true);

	unsigned char* pixels = stbi_load((m_executable_path + "/" + texture_path).c_str(), &widht, &height, &channel, 0);

	if (!pixels) {
		LOG_ERROR("Can't load image from path: {}", texture_path);
		return nullptr;
	}

	auto it = m_textures.emplace(name, std::make_shared<Texture2D>(widht, height, pixels, channel)).first;

	stbi_image_free(pixels);

	return it->second;
}

std::shared_ptr<Texture2D> ResourceManager::get_texture(const std::string& name)
{
	auto found = m_textures.find(name);

	if (found == m_textures.end()) {
		LOG_ERROR("Can't find texture with name: {}", name);
		return nullptr;
	}

	return found->second;
}


std::shared_ptr<ImFont> ResourceManager::load_font(const std::string& name, const std::string& font_path, unsigned int size)
{
	std::shared_ptr<ImFont> ptr_font(ImGui::GetIO().Fonts->AddFontFromFileTTF((m_executable_path + "/" + font_path).c_str(), size), [](ImFont*) { /* do nothing */ });

	return m_font.emplace(name, ptr_font).first->second;
}
