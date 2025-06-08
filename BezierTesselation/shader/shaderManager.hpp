#pragma once
#include <unordered_map>
#include "shader.hpp"
#include <string>
#include <memory>

class ShaderManager
{
	const Vector4<float> COLOR_SELECTED = { 1.f, 0.5f, 0.f, 1.f }, COLOR_DESELECTED = { 0.27, 0.27, 0.27, 1.f }, COLOR_DES_OTHER = {0.8f, 0.f, 0.4f, 1.f};
	std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
	std::vector<std::shared_ptr<Shader>> entityShaders;
public:
	void add(const std::string& name, const std::shared_ptr<Shader>& shader, bool isSelectable = true);
	std::shared_ptr<Shader> get(const std::string& name) const;
	void setSelected(bool selected, bool usesOther, bool invert);
	void setColor(const Vector4<float>& color);
	void setColor(unsigned color);
	std::unordered_map<std::string, std::shared_ptr<Shader>>& getShaders();
};