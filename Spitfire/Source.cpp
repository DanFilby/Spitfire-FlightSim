#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define __STDC_LIB_EXT1__
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

//Window
#include "WindowInitialiser.hpp"
//Frame rate tracker
#include "FrameTracking.hpp"
//Shader Loader
#include "LoadShaders.hpp"
//GameObject including model as well
#include "GameObject.h"
//Controls is a fps controller
#include "Controls.h"
//lighting is just the light pos and colour
#include "Lighting.h"
//UIText is for printing text to screen
#include "UIText.hpp"
//Simple world creates a flat floor in the scene
#include "SimpleWorld.h"
//holds useful functions like perlin noise
#include "Utilities.hpp"

#include "PlaneController.h"
#include "physics.h"

//window sizes
#define SMALL
#if defined(SMALL)
//Window settings
int WIDTH = 1024;
int HEIGHT = 768;
#elif defined(WIDE)
int WIDTH = 2560;
int HEIGHT = 1080;
#else 
int WIDTH = 1920;
int HEIGHT = 1080;
#endif

//Frame Tracking
FrameTracker frameTracker = FrameTracker(true, true);

// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

//Light nodes stored here max 8. only using first atm
LightNode lights[8] = {};

int AppStart(WindowInitialiser& programStart, Controls *&controls) {
	//Create the window
	programStart = WindowInitialiser();
	int errorCode = programStart.Init(WIDTH, HEIGHT);

	Controls::window = programStart.Window;
	controls = new Controls(programStart.Window, WIDTH, HEIGHT);

	//enable depth check
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return errorCode;
}

void AppClose() {

}


int main() {
	//Initilaise and check for errors
	GLFWwindow* window{}; Controls* controls{}; WindowInitialiser programStart{};
	if (AppStart(programStart, controls) == -1) { return -1; }

	window = programStart.Window;

	lights[0] = LightNode(glm::vec3(10, 5, 0), glm::vec3(1, 1, 1), 1);

	GameObject* testPilot = GameObject::CreateGameObject("../Assets/Spitfire-Model.fbx", &lights[0]);
	testPilot->SetModelStartRotation(glm::vec3(glm::radians(270.0f), 0, 0));

	UIText UITextManager = UIText(Fonts::CourierNewBold);
	int textid = UITextManager.AddTextToScreen("Spining Spitfire", 300, 550, 20);

	SimpleWorld world = SimpleWorld((std::string)"Default", SkySetting::Noon, &lights[0]);

	PlaneController plane = PlaneController(testPilot);

	controls->position = testPilot->Position - glm::vec3(-0.5,-0.5,-0.5);

	do {
		//clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//update the frame tracker
		frameTracker.Update();

		glm::vec3 offset = testPilot->Forward() * -2.5f + testPilot->Up() * 0.6f;
		controls->Update(testPilot->Position, offset, testPilot->Up());
		//controls->Update();

		world.DrawWorld(projection, controls->viewMatrix);
		
		plane.Update(controls->deltaTime);

		//testPilot->Rotate(glm::vec3(0, 0, 0), 10 * (float)controls->deltaTime);
		testPilot->Draw(projection, controls->viewMatrix, controls->position);

		//lights[0] = LightNode(glm::vec3(sin(glfwGetTime()) * 10, 10, 0), glm::vec3(1, 1, 1), 1);


		//draws UI to screen, needs to be rendered last
		//UITextManager.DrawAllText();

		glfwSwapBuffers(window);

		glfwPollEvents();
	}while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	delete(testPilot);
	AppClose();
}