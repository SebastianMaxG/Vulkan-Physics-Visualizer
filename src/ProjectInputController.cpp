#include "ProjectInputController.h"

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
}