#pragma once

#include "gameobject.h"
#include "window.h"


class KeyboardController
{

public:
	struct KeyMappings {
		int moveLeft = GLFW_KEY_A;
		int moveRight = GLFW_KEY_D;
		int moveForward = GLFW_KEY_S;
		int moveBack = GLFW_KEY_W;
		int moveUp = GLFW_KEY_SPACE;
		int moveDown = GLFW_KEY_LEFT_SHIFT;
	};

	KeyMappings keys;
	float moveSpeed{ .3f };
	float lookSpeed{ 1.5f };

	void moveGameObject(GLFWwindow* window, float deltaTime, GameObject gameObject);




};

