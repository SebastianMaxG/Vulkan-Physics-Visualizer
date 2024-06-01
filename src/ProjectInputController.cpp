#include "ProjectInputController.h"
#include "FirstApp.h"

namespace lsmf
{
	void ProjectInputController::MoveInPlaneXZ(GLFWwindow* window, float deltaTime, ProjectGameObject& gameObject)
	{
		glm::vec3 rotate{ 0 };
		if (glfwGetKey(window, keys.lookRight)	== GLFW_PRESS){ rotate.y -= 1.f; }
		if (glfwGetKey(window, keys.lookLeft)	== GLFW_PRESS){ rotate.y += 1.f; }
		if (glfwGetKey(window, keys.lookUp)		== GLFW_PRESS){ rotate.x -= 1.f; }
		if (glfwGetKey(window, keys.lookDown)	== GLFW_PRESS){ rotate.x += 1.f; }

		if (rotate != glm::vec3{ 0 })
		{
			gameObject.m_Transform.rotation += glm::normalize(rotate) * lookSpeed * deltaTime;
		}

		gameObject.m_Transform.rotation.x = glm::clamp(gameObject.m_Transform.rotation.x, -glm::half_pi<float>(), glm::half_pi<float>());
		gameObject.m_Transform.rotation.y = glm::mod(gameObject.m_Transform.rotation.y, glm::two_pi<float>());

		float yaw = gameObject.m_Transform.rotation.y;
		const glm::vec3 forward = glm::vec3{ glm::sin(yaw), 0.f, glm::cos(yaw) };
		const glm::vec3 right = glm::vec3{ forward.z, 0.f, -forward.x };
		const glm::vec3 up = glm::vec3{ 0.f, -1.f, 0.f };

		glm::vec3 move{ 0 };
		if (glfwGetKey(window, keys.moveForward)	== GLFW_PRESS) { move -= forward; }
		if (glfwGetKey(window, keys.moveBackward)	== GLFW_PRESS) { move += forward; }
		if (glfwGetKey(window, keys.moveRight)		== GLFW_PRESS) { move += right; }
		if (glfwGetKey(window, keys.moveLeft)		== GLFW_PRESS) { move -= right; }
		if (glfwGetKey(window, keys.moveUp)			== GLFW_PRESS) { move += up; }
		if (glfwGetKey(window, keys.moveDown)		== GLFW_PRESS) { move -= up; }

		if (move != glm::vec3{ 0 })
		{
			gameObject.m_Transform.translation += glm::normalize(move) * moveSpeed * deltaTime;
		}
	}
	void ProjectInputController::updateParameters(GLFWwindow* window, float deltaTime, btDiscreteDynamicsWorld* world, FirstApp* app)
	{
		bool dirty = false;
		if (glfwGetKey(window, keys.gravUp) == GLFW_PRESS)
		{
			btVector3 gravity = world->getGravity();
			gravity.setY(gravity.getY() + gravSpeed * deltaTime);
			world->setGravity(gravity);
			dirty = true;
		}

		if (glfwGetKey(window, keys.gravDown) == GLFW_PRESS)
		{
			btVector3 gravity = world->getGravity();
			gravity.setY(gravity.getY() - gravSpeed * deltaTime);
			world->setGravity(gravity);
			 dirty = true;
		}
		if (dirty)
		{
			for (int i = 0; i < world->getNumCollisionObjects(); i++)
			{
				world->getCollisionObjectArray()[i]->activate();
			}
		}

		if (glfwGetKey(window, keys.reset) == GLFW_PRESS)
		{
			app->ResetGameObjects();
		}

		if (spawndelay > 0)
		{
			spawndelay -= deltaTime;
		}
		else
		{
			if (glfwGetKey(window, keys.spawnCube) == GLFW_PRESS)
			{
				app->SpawnObject("../models/cube.obj", false, { 0,-5.f,-3 });
				spawndelay = spawndelayMax;
			}
			if (glfwGetKey(window, keys.spawnBall) == GLFW_PRESS)
			{
				app->SpawnObject("../models/ball.obj", false, { 0,-5.f,2.5f });
				spawndelay = spawndelayMax;
			}
			if (glfwGetKey(window, keys.spawnCone) == GLFW_PRESS)
			{
				app->SpawnObject("../models/cone.obj", false, { 4.f,-5.f,0 });
				spawndelay = spawndelayMax;
			}
		}
	}
}