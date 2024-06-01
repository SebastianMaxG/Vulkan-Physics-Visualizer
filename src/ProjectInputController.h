#pragma once

#include "ProjectGameObject.h"
#include "ProjectWindow.h"

namespace lsmf
{
	class FirstApp;
}

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

			int gravUp = GLFW_KEY_KP_ADD;
			int gravDown = GLFW_KEY_KP_SUBTRACT;

			int pickup = GLFW_MOUSE_BUTTON_LEFT;

			int reset	  = GLFW_KEY_R;
			int spawnCube = GLFW_KEY_I;
			int spawnCone = GLFW_KEY_P;
			int spawnBall = GLFW_KEY_O;

		};

		ProjectInputController() = default;
		~ProjectInputController() = default;

		ProjectInputController(const ProjectInputController&) = delete;
		ProjectInputController& operator=(const ProjectInputController&) = delete;


		void MoveInPlaneXZ(GLFWwindow* window, float deltaTime, ProjectGameObject &gameObject);


		void updateParameters(GLFWwindow* window, float deltaTime, btDiscreteDynamicsWorld* world, FirstApp* app);

		const KeyMappings keys{};

		float moveSpeed = 12.0f;
		float lookSpeed = 3.f;
		float gravSpeed = 4.f;

		float spawndelay{};
		const float spawndelayMax {0.5f};
		//void handleInput(std::vector<ProjectGameObject> &gameObjects);
		
	};
}
