#pragma once

#include "GL\glew.h"
#include <GLFW/glfw3.h>

#include <matrix\matrix.hpp>

class InputHandler;

class Window {
	GLFWwindow* window;
	int width, height, ui_width;

	bool useStereoscopy;
	float iod, convergence;
	void initImGui();
public:
	Window(int width, int height, int ui_width);
	~Window();

	int getWidth() const;
	int getViewportWidth() const;
	int getHeight() const;
	bool shouldClose() const;
	bool shouldDraw() const;
	void finishFrame();

	void setupCallbacks(InputHandler& handler);
	void handleWindowResize(int width, int height);

	void handleImGui();

	Matrix4x4<float> getProjectionMatrix() const;
	Matrix4x4<float> getInverseProjectionMatrix() const;

	Matrix4x4<float> getLeftProjectionMatrix() const;
	Matrix4x4<float> getRightProjectionMatrix() const;

	GLFWwindow* getWindow() const;
};