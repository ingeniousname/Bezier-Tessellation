#include "app.hpp"
#include <iostream>
#include "GL\glew.h"
#include "shader\shaderBuilder.hpp"
#include <vector\vector.hpp>

GLFWApplication::GLFWApplication()
{
	if (!glfwInit())
		std::cout << "error initializing glfw!\n";
}

GLFWApplication::~GLFWApplication()
{
	glfwTerminate();
}

void App::addShader(const std::string& name, const std::string& vsPath,
	const std::string& psPath, const std::string& gsPath, const std::pair<std::string, std::string>& tessPath,
	bool usedForEntities)
{
	ShaderBuilder builder;
	builder.addVertexShader(vsPath.c_str());
	builder.addFragmentShader(psPath.c_str());
	if (!gsPath.empty())
		builder.addGeometryShader(gsPath.c_str());
	if (!tessPath.first.empty())
	{
		builder.addTesselationControlShader(tessPath.first.c_str());
		builder.addTesselationEvalShader(tessPath.second.c_str());
	}

	shaderManager.add(name, builder.compileAndGetShaderProgram(), usedForEntities);
}

App::App(int width, int height, int ui_width) : GLFWApplication{}, window(width, height, ui_width)
{
	GLuint status = glewInit();
	if (status != GLEW_OK)
		std::cout << "error initializing glew\n";

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	glViewport(0, 0, width - ui_width, height);

	window.setupCallbacks(inputHandler);

	camera.rotate(-3.1415f / 4, -3.1415f / 4);
	glClearColor(1.f, 1.f, 1.f, 1.f);

	bezierSurface = std::make_shared<BezierC0Surface>(
		std::vector<Vector4<float>>{
			Vector4<float>(-1.5f, 0.0f, -1.5f, 1.0f), Vector4<float>(-0.5f, 0.2f, -1.5f, 1.0f),
			Vector4<float>(0.5f, 0.2f, -1.5f, 1.0f), Vector4<float>(1.5f, 0.0f, -1.5f, 1.0f),

			Vector4<float>(-1.5f, 0.2f, -0.5f, 1.0f), Vector4<float>(-0.5f, 0.5f, -0.5f, 1.0f),
			Vector4<float>(0.5f, 0.5f, -0.5f, 1.0f), Vector4<float>(1.5f, 0.2f, -0.5f, 1.0f),

			Vector4<float>(-1.5f, 0.2f, 0.5f, 1.0f), Vector4<float>(-0.5f, 0.5f, 0.5f, 1.0f),
			Vector4<float>(0.5f, 0.5f, 0.5f, 1.0f), Vector4<float>(1.5f, 0.2f, 0.5f, 1.0f),

			Vector4<float>(-1.5f, 0.0f, 1.5f, 1.0f), Vector4<float>(-0.5f, 0.2f, 1.5f, 1.0f),
			Vector4<float>(0.5f, 0.2f, 1.5f, 1.0f), Vector4<float>(1.5f, 0.0f, 1.5f, 1.0f),
	}, 1, 1);

	this->addShader("default", "res/simpleVertex.glsl", "res/simpleFragment.glsl");

	this->addShader("bezierC0Surface", "res/bezierC0Vertex.glsl", "res/simpleFragment.glsl", "",
		{ "res/bezierC0SurfaceTessC.glsl", "res/bezierC0SurfaceTessE.glsl" });
}

App::~App()
{
}

void App::loop()
{
	while (!window.shouldClose())
	{
		this->processInput();
		this->render();
		window.handleImGui();
		window.finishFrame();
	}
}

void App::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (window.shouldDraw())
	{
		for (const auto& shader : shaderManager.getShaders())
		{
			shader.second->setUniformMat4fv("projection", window.getProjectionMatrix());
			shader.second->setUniformMat4fv("view", camera.getViewMatrix());
		}

		bezierSurface->draw(shaderManager);
	}
}

void App::processInput()
{
	//if (inputHandler.isPressed(ImGuiKey_MouseMiddle, true))
	if (inputHandler.isPressed(ImGuiKey_MouseRight, true))
	{
		auto mouseMove = inputHandler.getMouseMove();
		if (inputHandler.isPressed(ImGuiKey_LeftShift, true))
		{
			camera.pan(mouseMove.first * cameraMoveSpeed, -mouseMove.second * cameraMoveSpeed);
		}
		else
		{
			camera.rotate(mouseMove.first * cameraRotSpeed, mouseMove.second * cameraRotSpeed);
		}
	}

	camera.zoom(inputHandler.getScrollMove());
}
