#include "window.hpp"
#include "../input/inputHandler.hpp"
#include <imgui.h>
#include <backends\imgui_impl_opengl3.h>
#include <backends\imgui_impl_glfw.h>

void Window::initImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiIO& io{ ImGui::GetIO() }; (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 430");
}

Window::Window(int width, int height, int ui_width) : width(width), height(height), ui_width(ui_width), iod(0.3f), useStereoscopy(false), convergence(2.f)
{
	window = glfwCreateWindow(width, height, "Torus", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);
	initImGui();
}

Window::~Window()
{
	glfwDestroyWindow(window);
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

int Window::getWidth() const
{
	return width;
}

int Window::getViewportWidth() const
{
	return width - ui_width;
}

int Window::getHeight() const
{
	return height;
}

bool Window::shouldClose() const
{
	return glfwWindowShouldClose(window);
}

bool Window::shouldDraw() const
{
	return !glfwGetWindowAttrib(window, GLFW_ICONIFIED);
}

void Window::finishFrame()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Window::setupCallbacks(InputHandler& handler)
{
	glfwSetWindowUserPointer(window, &handler);
	glfwSetScrollCallback(window, InputHandler::scrollCallback);
	//glfwSetCursorPosCallback(window, InputHandler::cursorPosCallback);

	handler.setWindowPointer(this);
	glfwSetFramebufferSizeCallback(window, InputHandler::framebufferSizeCallback);
}

void Window::handleWindowResize(int width, int height)
{
	this->width = width;
	this->height = height;
	if(this->width > ui_width && this->height > 0)
		glViewport(0, 0, width - ui_width, height);
}

void Window::handleImGui()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse;


	ImGui::SetNextWindowPos(ImVec2(this->getViewportWidth(), 0));
	ImGui::SetNextWindowSize(ImVec2(this->getWidth() - this->getViewportWidth(), this->getHeight()));

	// Example ImGui window
	ImGui::Begin("Hello, ImGui!", nullptr, windowFlags);
	ImGui::Dummy(ImVec2(0.f, 10.f));

	ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	ImGui::EndFrame();
}

Matrix4x4<float> Window::getProjectionMatrix() const
{
	return Matrix4x4<float>::perspectiveRH(3.1415f / 3, (float)width / height, 0.1f, 100.f);
}

Matrix4x4<float> Window::getInverseProjectionMatrix() const
{
	return Matrix4x4<float>::inversePerspectiveRH(3.1415f / 3, (float)width / height, 0.1f, 100.f);
}

Matrix4x4<float> Window::getLeftProjectionMatrix() const
{
	return Matrix4x4<float>::stereoPerspectiveRH(3.1415f / 3, (float)width / height, 0.1f, 100.f, iod, convergence, true);
}

Matrix4x4<float> Window::getRightProjectionMatrix() const
{
	return Matrix4x4<float>::stereoPerspectiveRH(3.1415f / 3, (float)width / height, 0.1f, 100.f, iod, convergence, false);
}

GLFWwindow* Window::getWindow() const
{
	return window;
}
