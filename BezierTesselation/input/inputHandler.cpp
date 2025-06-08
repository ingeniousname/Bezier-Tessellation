#include "../window/window.hpp"
#include "inputHandler.hpp"

void InputHandler::cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	InputHandler* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
	if (handler) {
		handler->mousedelta_x = xpos - handler->mousePos.first;
		handler->mousedelta_y = ypos - handler->mousePos.second;

		handler->mousePos = { xpos, ypos };
	}
}

void InputHandler::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	InputHandler* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
	if (handler) {
		handler->scroll_delta += yoffset;
	}
}

void InputHandler::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	InputHandler* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
	if (handler) {
		handler->window->handleWindowResize(width, height);
	}

}

bool InputHandler::isPressed(ImGuiKey key, bool held) const
{
	if (ImGui::GetIO().WantTextInput)
		return false;

	if (held)
		return ImGui::IsKeyDown(key);
	else return ImGui::IsKeyPressed(key);
}

void InputHandler::setWindowPointer(Window* window)
{
	this->window = window;
}

void InputHandler::resetMouseHoldPos()
{
	this->mouseHoldPos = this->mousePos;
}

std::pair<float, float> InputHandler::getMouseMove()
{
	auto delta = ImGui::GetIO().MouseDelta;
	return { delta.x, delta.y };
}

std::pair<float, float> InputHandler::getMousePos(bool normalized) const
{
	if (normalized)
	{
		std::pair<int, int> center = { (window->getViewportWidth() - 1) / 2, (window->getHeight() - 1) / 2 };
		int minCoordinate = std::min(window->getViewportWidth(), window->getHeight());
		return { 2 * (mousePos.first - center.first) / minCoordinate, -2 * (mousePos.second - center.second) / minCoordinate };
	}
	else
		return mousePos;
}

std::pair<float, float> InputHandler::getMouseHoldPos(bool normalized) const
{
	if (normalized)
	{
		std::pair<int, int> center = { (window->getViewportWidth() - 1) / 2, (window->getHeight() - 1) / 2 };
		int minCoordinate = std::min(window->getViewportWidth(), window->getHeight());
		return { 2 * (mouseHoldPos.first - center.first) / minCoordinate, -2 * (mouseHoldPos.second - center.second) / minCoordinate };
	}
	else
		return mouseHoldPos;
}

int InputHandler::getScrollMove()
{
	ImVec2 mousePos = ImGui::GetMousePos();

	int windowHeight = window->getHeight();
	int viewportWidth = window->getViewportWidth();

	bool isMouseInViewport =
		mousePos.x >= 0 && mousePos.x < viewportWidth &&
		mousePos.y >= 0 && mousePos.y < windowHeight;

	if (isMouseInViewport)
	{
		int delta = scroll_delta;
		scroll_delta = 0;
		return delta;
	}

	return scroll_delta = 0;
}