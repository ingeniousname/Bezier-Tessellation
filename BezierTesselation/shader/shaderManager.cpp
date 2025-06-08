#include "shaderManager.hpp"

void ShaderManager::add(const std::string& name, const std::shared_ptr<Shader>& shader, bool isSelectable)
{
	this->shaders.insert({ name, shader });
	if(isSelectable)
		this->entityShaders.push_back(shader);
}

std::shared_ptr<Shader> ShaderManager::get(const std::string& name) const
{
	auto it = this->shaders.find(name);
	return it == this->shaders.end() ? nullptr : it->second;
}

void ShaderManager::setSelected(bool selected, bool usesOther, bool invert)
{
	Vector4<float> color = selected ? COLOR_SELECTED : usesOther ? COLOR_DES_OTHER : COLOR_DESELECTED;
	if (invert && color.x == COLOR_DESELECTED.x)
		color = { 1 - color.x, 1 - color.y, 1 - color.z, 1.f };

	for (auto& shader : entityShaders)
	{
		shader->setUniformVec4f("color", color);
	}
}

void ShaderManager::setColor(const Vector4<float>& color)
{
	for (auto& shader : entityShaders)
	{
		shader->setUniformVec4f("color", color);
	}
}

void ShaderManager::setColor(unsigned color)
{
	float a = ((color >> 24) & 0xFF) / 255.0f;
	float b = ((color >> 16) & 0xFF) / 255.0f;
	float g = ((color >> 8) & 0xFF) / 255.0f;
	float r = (color & 0xFF) / 255.0f;

	this->setColor({ r, g, b, a });
}

std::unordered_map<std::string, std::shared_ptr<Shader>>& ShaderManager::getShaders()
{
	return this->shaders;
}
