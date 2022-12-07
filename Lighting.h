#pragma once

#include <glm/glm.hpp>

class LightNode
{
public: 
	LightNode():
		lightPosition(glm::vec3()), lightColour(glm::vec3(1)), lightPower(1) {};

	LightNode(glm::vec3 _lightPosition, glm::vec3 _lightColour, float _lightPower): 
		lightPosition(_lightPosition), lightColour(_lightColour), lightPower(_lightPower){};

	glm::vec3 lightPosition;
	glm::vec3 lightColour;
	float lightPower;
};

