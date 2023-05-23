#pragma once
#include <glm/glm.hpp>

namespace SolverColors
{	
	static glm::vec3 getColorFromRGB(glm::vec3 colorRGB)
	{
		return glm::vec3(colorRGB.x / 255.0f, colorRGB.y / 255.0f, colorRGB.z / 255.0f);
	};

	const glm::vec3 DEFAULT_LINE_COLOR = getColorFromRGB(glm::vec3(255, 255, 255));
	const glm::vec3 DEFAULT_POINT_COLOR = getColorFromRGB(glm::vec3(230, 230, 230));

	const glm::vec3 BACKGROUND_COLOR = getColorFromRGB(glm::vec3(19, 21, 21));

	const glm::vec3 COMPLETE_COLOR = getColorFromRGB(glm::vec3(88, 255, 49));

	const glm::vec3 LINE_HIGHLIGHT_COLOR_A = getColorFromRGB(glm::vec3(255, 0, 0));
	const glm::vec3 LINE_HIGHLIGHT_COLOR_B = getColorFromRGB(glm::vec3(255, 255, 0));


}