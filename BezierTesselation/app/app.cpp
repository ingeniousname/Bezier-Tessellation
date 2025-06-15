#include "app.hpp"
#include <iostream>
#include "GL\glew.h"
#include "shader\shaderBuilder.hpp"
#include <vector\vector.hpp>
#include <imgui.h>
#include <backends\imgui_impl_opengl3.h>
#include <backends\imgui_impl_glfw.h>

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

	std::vector<Vector4<float>> controlPoints;
	int totalControlPointsDim = 3 * 4 + 1;
	controlPoints.reserve(totalControlPointsDim * totalControlPointsDim);

	float spacing = 1.0f;
	float start = -3.0f * spacing;

	for (int row = 0; row < totalControlPointsDim; ++row) {
		for (int col = 0; col < totalControlPointsDim; ++col) {
			float x = start + col * spacing;
			float z = start + row * spacing;

			float y = 0.9f * std::cos(0.5f * x) * std::sin(0.5f * z);

			controlPoints.emplace_back(x, y, z, 1.0f);
		}
	}

	bezierSurface[0] = std::make_shared<BezierC0Surface>(controlPoints, 4, 4);

	std::vector<Vector4<float>> controlPoints2;
	controlPoints2.reserve(totalControlPointsDim * totalControlPointsDim);

	for (int row = 0; row < totalControlPointsDim; ++row) {
		for (int col = 0; col < totalControlPointsDim; ++col) {
			float x = start + col * spacing;
			float z = start + row * spacing;

			float y = -0.1f * (x * x + z * z);

			controlPoints2.emplace_back(x, y, z, 1.0f);
		}
	}

	bezierSurface[1] = std::make_shared<BezierC0Surface>(controlPoints2, 4, 4);

	this->addShader("default", "res/simpleVertex.glsl", "res/simpleFragment.glsl");

	this->addShader("bezierC0Surface", "res/bezierC0Vertex.glsl", "res/phongFragment.glsl", "",
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
		this->bezierSurface[currSurfaceIdx]->calculateLOD(this->camera.getViewMatrix());
		this->render();
		this->showUI();
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

		shaderManager.get("bezierC0Surface")->setUniformVec3f("light_pos", { 0, 5.f, 0 });
		shaderManager.get("bezierC0Surface")->setUniformVec3f("view_pos", camera.getPosition());

		bezierSurface[currSurfaceIdx]->draw(shaderManager);
	}
}

void App::showUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse;


	ImGui::SetNextWindowPos(ImVec2(window.getViewportWidth(), 0));
	ImGui::SetNextWindowSize(ImVec2(window.getWidth() - window.getViewportWidth(), window.getHeight()));

	// Example ImGui window
	ImGui::Begin("Hello, ImGui!", nullptr, windowFlags);
	ImGui::Dummy(ImVec2(0.f, 10.f));

	ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

	if (ImGui::Checkbox("Wireframe", &wireframe))
	{
		if(wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_CULL_FACE);
			glLineWidth(1.0f);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_CULL_FACE);
		}
	}

	bezierSurface[currSurfaceIdx]->ShareParameterUI();

	const char* items[] = { "Sine curve", "Paraboloid" };

	ImGui::Combo("Select surface", &currSurfaceIdx, items, IM_ARRAYSIZE(items));


	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	ImGui::EndFrame();
}

void App::processInput()
{
	const float subdivDelta = 0.1f;
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
	if (inputHandler.isPressed(ImGuiKey_E))
	{
		bezierSurface[currSurfaceIdx]->changeOuterSubdiv(subdivDelta);
	}
	if (inputHandler.isPressed(ImGuiKey_D))
	{
		bezierSurface[currSurfaceIdx]->changeOuterSubdiv(-subdivDelta);
	}
	if (inputHandler.isPressed(ImGuiKey_R))
	{
		bezierSurface[currSurfaceIdx]->changeInnerSubdiv(subdivDelta);
	}
	if (inputHandler.isPressed(ImGuiKey_F))
	{
		bezierSurface[currSurfaceIdx]->changeInnerSubdiv(-subdivDelta);
	}

	camera.zoom(inputHandler.getScrollMove());
}
