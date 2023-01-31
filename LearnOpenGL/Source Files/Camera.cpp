#include "Camera.h"

namespace Pink {

	/*
	*
	* Constructor & Destructor
	*
	*/
	Camera::Camera(
		glm::vec3 position,
		glm::vec3 worldUp,
		float pitch,
		float yaw
	) :
		front(glm::vec3(0.0f, 0.0f, -1.0f)),
		position(position),
		worldUp(worldUp),
		mouseSensitivity(SENSITIVITY),
		movementSpeed(SPEED),
		pitch(pitch),
		yaw(yaw),
		zoom(ZOOM)
	{
		updateCameraVectors();
	}

	Camera::Camera(
		float positionX, float positionY, float positionZ,
		float worldUpX, float worldUpY, float worldUpZ,
		float yaw, float pitch
	) :
		front(glm::vec3(0.0f, 0.0f, -1.0f)),
		position(glm::vec3(positionX, positionY, positionZ)),
		worldUp(glm::vec3(worldUpX, worldUpY, worldUpZ)),
		mouseSensitivity(SENSITIVITY),
		movementSpeed(SPEED),
		pitch(pitch),
		yaw(yaw),
		zoom(ZOOM)
	{
		updateCameraVectors();
	}

	/*
	*
	* Private Methods
	*
	*/
	void Camera::updateCameraVectors()
	{
		// Calculate the front vector.
		glm::vec3 newFront = glm::vec3(0.0f);
		newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		newFront.y = sin(glm::radians(pitch));
		newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		front = glm::normalize(newFront);

		// Recalculate the right and up vectors.
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}

	/*
	*
	* Public Methods
	*
	*/

	glm::mat4 Camera::getViewMatrix() const
	{
		return glm::lookAt(position, position + front, up);
	}

	void Camera::processKeyboard(CameraMovement direction, float deltaTime)
	{
		float velocity = movementSpeed * deltaTime;

		if (direction == FORWARD)
		{
			position += front * velocity;
		}

		if (direction == BACKWARD)
		{
			position -= front * velocity;
		}

		if (direction == LEFT)
		{
			position -= right * velocity;
		}

		if (direction == RIGHT)
		{
			position += right * velocity;
		}
	}

	void Camera::processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch)
	{
		xOffset *= mouseSensitivity;
		yOffset *= mouseSensitivity;

		yaw += xOffset;
		pitch += yOffset;

		// Make sure that, when the pitch is out of bounds, the screen doesn't get flipped.
		if (constrainPitch)
		{
			if (pitch > 89.0f)
			{
				pitch = 89.0f;
			}
			else if (pitch < -89.0f)
			{
				pitch = -89.0f;
			}
		}

		// Update front, right, and up vectors using the updated Euler angles.
		updateCameraVectors();
	}

	void Camera::processMouseScroll(float yOffset)
	{
		zoom -= static_cast<float>(yOffset);

		if (zoom < 1.0f)
		{
			zoom = 1.0f;
		}
		else if (zoom > 45.0f)
		{
			zoom = 45.0f;
		}
	}

}