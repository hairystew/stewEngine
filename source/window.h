#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

class Window
{
public:
	Window(int w, int h, std::string name);
	~Window();

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;


	

	bool shouldClose() { return glfwWindowShouldClose(window); }
	bool wasWindowResized() { return frameBufferResized; };
	void resetWindowResizedFlag() { frameBufferResized = false; };
	VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }

	void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

private:
	static void frameBufferResizeCallback(GLFWwindow* window, int width, int height);
	void initWindow();

	int width;
	int height;
	bool frameBufferResized = false;

	std::string windowName;

	GLFWwindow* window;



};

