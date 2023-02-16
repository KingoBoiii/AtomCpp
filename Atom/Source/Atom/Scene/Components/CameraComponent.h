#pragma once
#include "Atom/Scene/SceneCamera.h"

namespace Atom::Component
{

	struct Camera
	{
		SceneCamera SceneCamera;
		bool Primary = true; 
		bool FixedAspectRatio = false;
		
		Camera() = default;
		Camera(const Camera&) = default;
	};

}
