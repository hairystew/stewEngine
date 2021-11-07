#include "window.h"

#include <stdexcept>

Window::keyboardInput Window::inputs{};

Window::Window(int w, int h, std::string name) : width{ w }, height{ h }, windowName{ name }{
	initWindow();
}

Window::~Window() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::update()
{
	glfwGetCursorPos(window, &camXPos, &camYPos);
	inputs.mouseX = camXPos;
	inputs.mouseY = camYPos;
}

void Window::frameBufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto localWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	localWindow->frameBufferResized = true;




}

void Window::initWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	inputs.isRight = true;
}


void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
	if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface");
	}
}


void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key) {
	case GLFW_KEY_D:
		if (action == GLFW_PRESS)
			inputs.isRight = true;
		if (action == GLFW_RELEASE)
			inputs.isRight = false;
		break;
	case GLFW_KEY_A:
		if (action == GLFW_PRESS)
			inputs.isLeft = true;
		if (action == GLFW_RELEASE)
			inputs.isLeft = false;
		break;
	case GLFW_KEY_S:
		if (action == GLFW_PRESS)
			inputs.isBack = true;
		if (action == GLFW_RELEASE)
			inputs.isBack = false;
		break;
	case GLFW_KEY_W:
		if (action == GLFW_PRESS)
			inputs.isForward = true;
		if (action == GLFW_RELEASE)
			inputs.isForward = false;
		break;
	case GLFW_KEY_SPACE:
		if (action == GLFW_PRESS)
			inputs.isUp = true;
		if (action == GLFW_RELEASE)
			inputs.isUp = false;
		break;
	case GLFW_KEY_LEFT_CONTROL:
		if (action == GLFW_PRESS)
			inputs.isDown = true;
		if (action == GLFW_RELEASE)
			inputs.isDown = false;
		break;
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, true);
		break;
	}

}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	inputs.mouseDelX = xpos - inputs.mouseX;
	inputs.mouseDelY = ypos - inputs.mouseY;
}