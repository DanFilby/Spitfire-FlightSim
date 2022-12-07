#pragma once

//GLEW extends open-gl to make applications for all target platforms
#include <GL/glew.h>

//Using GLFW to handle the window and keyboard
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

class WindowInitialiser
{
public:
	int Init(int width, int height);
	GLFWwindow* Window;
	~WindowInitialiser();

};

/// <summary>
///	Creates a window of the given size. init GLEW, GLFW
/// </summary>
inline int WindowInitialiser::Init(int width, int height)
{
	glewExperimental = true; // Needed for core profile
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

	// Open a window and create its OpenGL context
	GLFWwindow* window; // (In the accompanying source code, this variable is global for simplicity)
	window = glfwCreateWindow(width, height, "Opengl Base", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glEnable(GL_FRAMEBUFFER_SRGB);

	Window = window;
}

inline WindowInitialiser::~WindowInitialiser()
{
	glfwDestroyWindow(Window);
}
