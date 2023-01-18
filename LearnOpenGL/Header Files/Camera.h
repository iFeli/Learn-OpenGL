#ifndef CAMERA_H
#define CAMERA_H

#include <GLAD/glad.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

namespace Pink
{

	// Define options for camera movement.
	enum CameraMovement
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	// Defalt camera values.
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float SPEED = 5.0f;
	const float SENSITIVITY = 0.1f;
	const float ZOOM = 45.0f;

	class Camera
	{
	public:
		Camera(
			glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
			float pitch = PITCH,
			float yaw = YAW
		);
		Camera(float positionX, float positionY, float positionZ, float worldUpX, float worldUpY, float worldUpZ, float yaw, float pitch);

		glm::vec3 front;
		glm::vec3 position;
		glm::vec3 right;
		glm::vec3 up;
		glm::vec3 worldUp;

		float mouseSensitivity;
		float movementSpeed;
		float pitch;
		float yaw;
		float zoom;

		glm::mat4 getViewMatrix();
		void processKeyboard(CameraMovement direction, float deltaTime);
		void processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);
		void processMouseScroll(float yOffset);

	private:
		void updateCameraVectors();
	};

}

#endif