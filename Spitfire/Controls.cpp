#include "Controls.h"

Controls::Controls(GLFWwindow *_window, int screenWidth, int screenHeight):
	screenH(screenHeight), screenW(screenWidth)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, screenW / 2.0, screenH / 2.0);

	Update();
	position = -2.0f * forward;
	forward = glm::vec3(0,0,-0.9);
}

void Controls::Update()
{
	double currentTime = glfwGetTime();
	deltaTime = float(currentTime - prevTime);
	prevTime = currentTime;

	CalculateDirection(deltaTime);
	CheckmoveInput(deltaTime);

	viewMatrix = glm::lookAt(
		position,
		position + forward,
		up
	);
}

void Controls::Update(glm::vec3 pos, glm::vec3 offset, glm::vec3 upDir)
{
	double currentTime = glfwGetTime();
	deltaTime = float(currentTime - prevTime);
	prevTime = currentTime;

	CalculateDirection(deltaTime);

	viewMatrix = glm::lookAt(
		pos + offset,
		pos ,
		upDir
	);
}


void Controls::CalculateDirection(float deltaTime)
{
	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	//reset mouse to centre
	glfwSetCursorPos(window, screenW / 2.0, screenH / 2.0);

	// Compute new orientation
	horizontalAngle += mouseSpeed * deltaTime * float(screenW / 2 - xpos);
	verticalAngle += mouseSpeed * deltaTime * float(screenH / 2 - ypos);

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	forward = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	// Right vector
	right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	// Up vector : perpendicular to both direction and right
	up = glm::cross(right, forward);

	//printf("%f,%f,%f\n", forward.x,forward.y,forward.z);

}

void Controls::CheckmoveInput(float deltaTime)
{
	// Move forward
	if (glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS) {
		position += forward * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		position -= forward * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		position -= right * deltaTime * speed;
	}
	//move straight up or down at a slightly slower speed
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		position.y += deltaTime * speed * 0.8f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		position.y -= deltaTime * speed * 0.8f;
	}
}

int Controls::GetKey(int key)
{
	if (glfwGetKey(window, key) == GLFW_PRESS) {
		return 1;
	}
	return 0;
}