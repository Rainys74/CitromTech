#include "test.h"

#include <iostream>

#include "GLFW/glfw3.h"

void Test::PrintSomeShit(char* shit)
{
	std::cout << shit << "\n";
}

void Test::PrintComplexEmoji(const char* utf8Probably)
{
	//std::locale::global(std::locale("en_US.UTF-8")); // Set the locale to UTF-8
	std::cout << utf8Probably << "\n";
}

void Test::TestOutGLFW()
{
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(640, 480, "Test", NULL, NULL);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			std::cout << "W key is being pressed/held!" << "\n";
		}
	}

	glfwDestroyWindow(window);

	glfwTerminate();
}
