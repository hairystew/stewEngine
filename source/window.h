#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>



struct keyboardInput {
	bool isRight = false;
	bool isLeft = false;
	bool isForward = false;
	bool isBack = false;
	int moveLeft = GLFW_KEY_A;
	int moveRight = GLFW_KEY_D;
	int moveForward = GLFW_KEY_S;
	int moveBack = GLFW_KEY_W;
	int moveUp = GLFW_KEY_SPACE;
	int moveDown = GLFW_KEY_LEFT_SHIFT;
	float mouseX = 0.f;
	float mouseY = 0.f;
};


class Window
{
public:
	struct keyboardInput {
		bool isRight = false;
		bool isLeft = false;
		bool isForward = false;
		bool isBack = false;
		bool isUp = false;
		bool isDown = false;
		int moveLeft = GLFW_KEY_A;
		int moveRight = GLFW_KEY_D;
		int moveForward = GLFW_KEY_S;
		int moveBack = GLFW_KEY_W;
		int moveUp = GLFW_KEY_SPACE;
		int moveDown = GLFW_KEY_LEFT_SHIFT;
		float mouseX = 0.f;
		float mouseY = 0.f;
		float mouseDelX = 0.f;
		float mouseDelY = 0.f;
	};

	static keyboardInput inputs;
	Window(int w, int h, std::string name);
	~Window();

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void update();

	

	bool shouldClose() { return glfwWindowShouldClose(window); }
	bool wasWindowResized() { return frameBufferResized; };
	void resetWindowResizedFlag() { frameBufferResized = false; };
	VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }

	void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

private:
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void frameBufferResizeCallback(GLFWwindow* window, int width, int height);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	void initWindow();

	double camXPos, camYPos;
	int width;
	int height;
	bool frameBufferResized = false;
	std::string windowName;
	GLFWwindow* window;



};

