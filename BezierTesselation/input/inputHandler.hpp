#pragma once
#include <unordered_map>
#include <functional>
#include <GLFW\glfw3.h>
#include <imgui.h>
class Window; 

class InputHandler
{
	std::pair<float, float> mouseHoldPos, mousePos;
	double mousedelta_x = 0, mousedelta_y = 0;
	double scroll_delta = 0;

	Window* window;
	
public:
	static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

	bool isPressed(ImGuiKey key, bool held = false) const;
	void setWindowPointer(Window* window);
	void resetMouseHoldPos();

	std::pair<float, float> getMouseMove();
	std::pair<float, float> getMousePos(bool normalized = false) const;
	std::pair<float, float> getMouseHoldPos(bool normalized = false) const;
	int getScrollMove();
};