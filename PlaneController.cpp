#include "PlaneController.h"


PlaneController::PlaneController(GameObject* _plane)
	: plane(_plane)
{
}

void PlaneController::start()
{


}

void PlaneController::Update(float dt)
{
	glm::vec3 moveInput;
	glm::vec3 steeringInput;

	GetInput(moveInput, steeringInput);

	plane->Translate(moveInput * speed * dt * 3.0f);
	plane->Rotate(plane->Right(), steeringInput.x * dt * 30);
	plane->Rotate(glm::vec3(0, 1, 0), steeringInput.y * dt * 20);
	plane->Rotate(plane->Forward(), steeringInput.z * dt * 60);


}

void PlaneController::GetInput(glm::vec3& movementInput, glm::vec3& steeringInput)
{
	movementInput = glm::vec3(0);
	steeringInput = glm::vec3(0);

	movementInput +=  (float)Controls::GetKey(GLFW_KEY_UP) * plane->Forward();
	//movementInput += (float)Controls::GetKey(GLFW_KEY_RIGHT) * plane->Right();

	movementInput += (float)Controls::GetKey(GLFW_KEY_DOWN) * -1.0f * plane->Forward();
	//movementInput += (float)Controls::GetKey(GLFW_KEY_LEFT) * -1.0f * plane->Right();

	steeringInput += (float)Controls::GetKey(GLFW_KEY_KP_8) * glm::vec3(-1,0,0);
	steeringInput += (float)Controls::GetKey(GLFW_KEY_KP_5) * glm::vec3(1, 0, 0);

	steeringInput += (float)Controls::GetKey(GLFW_KEY_LEFT) * glm::vec3(0, 1, 0);
	steeringInput += (float)Controls::GetKey(GLFW_KEY_RIGHT) * glm::vec3(0, -1, 0);

	steeringInput += (float)Controls::GetKey(GLFW_KEY_KP_6) * glm::vec3(0, 0, 1);
	steeringInput += (float)Controls::GetKey(GLFW_KEY_KP_4) * glm::vec3(0, 0, -1);

}
