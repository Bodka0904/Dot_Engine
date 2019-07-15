#pragma once
#include <glm/glm.hpp>

namespace Dot {
	struct camera_matrices
	{
		glm::mat4 ViewProjectionMatrix;
	};

	struct light_props
	{
		glm::vec4 lightColor;
		glm::vec4 lightPos;

		float	 lightStrength;
		float	 specStrength;
	};
	
}