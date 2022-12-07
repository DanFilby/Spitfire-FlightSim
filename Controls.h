#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>

class Controls
{
	
public:
	glm::vec3 position = glm::vec3(0, 0, -5);

	double deltaTime = 0.0;

	glm::vec3 forward{};
	glm::vec3 right{};
	glm::vec3 up{};

	glm::mat4 viewMatrix{};

	Controls(GLFWwindow *_window, int screenWidth, int screenHeight);

	//no parameters to use a free camera moved with wasd, or set a position to look at an a offset for the camera
	void Update();
	void Update(glm::vec3 pos, glm::vec3 offset, glm::vec3 upDir);

	static int GetKey(int key);

	inline static GLFWwindow* window;


private:
	void CalculateDirection(float deltaTime);
	void CheckmoveInput(float deltaTime);

	int screenW, screenH;

	double prevTime = 0.0;

	// horizontal angle : toward -Z
	float horizontalAngle = -3.14f;
	// vertical angle : 0, look at the horizon
	float verticalAngle = 0.0f;
	// Initial Field of View
	float initialFoV = 45.0f;

	float speed = 1.0f; // 3 units / second
	float mouseSpeed = 1.0f;

	int mouseXpos = 0;
	int mouseYPos = 0;

};

