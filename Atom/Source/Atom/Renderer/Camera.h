#pragma once
#include "Atom/Core/Base.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Atom
{

	class Camera
	{
	public:
		Camera() = default;
		virtual ~Camera() = default;

        virtual const glm::mat4& GetViewMatrix() const = 0;
        virtual glm::mat4 GetProjectionMatrix() const = 0;
	};

}
