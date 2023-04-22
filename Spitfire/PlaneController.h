#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Controls.h"
#include "GameObject.h"

class PlaneController
{
public:
	PlaneController(GameObject* _plane);
	void start();
	void Update(float dt);

	float speed = 1.5;


private:
	void GetInput(glm::vec3 & movementInput, glm::vec3 & steeringInput);

	GameObject* plane;

};

