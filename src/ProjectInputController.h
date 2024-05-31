#pragma once

#include "ProjectGameObject.h"
#include "ProjectWindow.h"

namespace lsmf
{
	class ProjectInputController
	{
	public: 
		struct KeyMappings
		{
			int moveLeft = GLFW_KEY_A;
			int moveRight = GLFW_KEY_D;
			int moveForward = GLFW_KEY_W;
			int moveBackward = GLFW_KEY_S;
			int moveUp = GLFW_KEY_SPACE;
			int moveDown = GLFW_KEY_LEFT_SHIFT;
			int lookLeft = GLFW_KEY_LEFT;
			int lookRight = GLFW_KEY_RIGHT;
			int lookUp = GLFW_KEY_UP;
			int lookDown = GLFW_KEY_DOWN;
		};

		ProjectInputController() = default;
		~ProjectInputController() = default;

		ProjectInputController(const ProjectInputController&) = delete;
		ProjectInputController& operator=(const ProjectInputController&) = delete;


		void MoveInPlaneXZ(GLFWwindow* window, float deltaTime, ProjectGameObject &gameObject);

		const KeyMappings keys{};

		float moveSpeed = 3.0f;
		float lookSpeed = 1.5f;


		//void handleInput(std::vector<ProjectGameObject> &gameObjects);
		
	};
}