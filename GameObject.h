#pragma once

#include "LoadShaders.hpp"

#include <random>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Models.h"
#include "Lighting.h"

class GameObject
{
public:
	GameObject(GLuint _program, Model _model, LightNode* lightNodes);
	void SetupTransform(glm::vec3 _position, glm::vec3 _scale, glm::vec3 _rotation);
	static GameObject* CreateGameObject(std::string path, LightNode* lightNodes);

	void Draw(glm::mat4 projection, glm::mat4 view, glm::vec3 viewPosWorld);

	void Translate(glm::vec3 translation);
	void Rotate(glm::vec3 axis, float rotation);
	void Rotation(glm::vec3 eulerRotation);

	glm::vec3 Forward();
	glm::vec3 Right();
	glm::vec3 Up();

	void SetModelStartRotation(glm::vec3 eulerRotation);

	glm::vec3 Position;
	glm::vec3 Scale;
	glm::vec3 EulerRotation;

	glm::quat QRotation;

private:
	GLuint programID;
	Model modelObj;

	bool ModelNeedsRotating = false;
	glm::quat modelStartRot{};

	LightNode* lightNodes;

	glm::vec3 direction;
	float speed = 1.0f;
};
