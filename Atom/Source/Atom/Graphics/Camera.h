#pragma once
#include "Atom/Core/Base.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Atom
{

	class ATOM_API Camera
	{
	public:
        Camera() : Camera(glm::vec3(0.0f, 0.0f, 0.0f))
        {
        }
		
        Camera(const glm::vec3 position)
            : position(position),
            front(glm::vec3(0.0f, 0.0f, -1.0f)),
            up(glm::vec3(0.0f, 1.0f, 0.0f)),
            right(glm::vec3(1.0f, 0.0f, 0.0f)),
            worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
            yaw(YAW), pitch(PITCH), zoom(ZOOM)
        {
        }

		virtual ~Camera() = default;

        glm::mat4 GetViewMatrix() const
        {
			return glm::lookAt(position, position + front, up);
        }
    protected:
        void UpdateCameraVectors()
        {
            glm::vec3 front{ };
            front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            front.y = sin(glm::radians(pitch));
            front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            this->front = glm::normalize(front);
            this->right = glm::normalize(glm::cross(this->front, this->worldUp));
            this->up = glm::normalize(glm::cross(this->right, this->front));
        }
	protected:
        // Camera Attributes
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 worldUp;
        // Euler Angles
        float yaw;
        float pitch;
        // Camera options
        float zoom;

        // Default camera values
        constexpr static float YAW = -90.0f;
        constexpr static float PITCH = 0.0f;
        constexpr static float SPEED = 2.5f;
        constexpr static float SENSITIVITY = 0.1f;
        constexpr static float ZOOM = 45.0f;
	};

}
