#pragma once
#include "GL\glew.h"
#include "window/window.hpp"
#include "input/inputHandler.hpp"
#include <memory>
#include "camera\camera.hpp"
#include "shader\shaderManager.hpp"
#include "entity\bezierC0Surface.hpp"

class GLFWApplication {
protected:
	GLFWApplication();
	virtual ~GLFWApplication();
};

enum class AppState {
	SELECT, ROTATION, TRANSLATION, SCALE
};

class App : GLFWApplication {
	Window window;
	InputHandler inputHandler;
	Camera camera;
	ShaderManager shaderManager;
	std::shared_ptr<BezierC0Surface> bezierSurface;

	const float cameraRotSpeed = 0.02f;
	const float cameraMoveSpeed = 0.005f;
	const float zoomSpeed = 0.2f;

	void addShader(const std::string& name, const std::string& vsPath,
		const std::string& psPath, const std::string& gsPath = "", const std::pair<std::string, std::string>& tessPath = {"", ""},
		bool usedForEntities = false);
public:
	App(int width, int height, int ui_width);
	~App();
	void loop();
	void render();
	void processInput();
};