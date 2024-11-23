#include "test.h"

#include <iostream>

#include "GLFW/glfw3.h"

#include "glslcc.h"

void Test::PrintSomeShit(char* shit)
{
	std::cout << shit << "\n";
}

void Test::PrintComplexEmoji(const char* utf8Probably)
{
	//std::locale::global(std::locale("en_US.UTF-8")); // Set the locale to UTF-8
	std::setlocale(LC_ALL, ".UTF8");
	std::cout << utf8Probably << "\n";
}

void Test::PrintWideString(const wchar_t* utf16WideUnicode)
{
	std::wcout << utf16WideUnicode << std::endl;
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

void Test::TestOutGLSLCC(const char* filePath)
{
	const char* arguments[] = {
		filePath,
		"--help"
	};
	glslcc_exec(1, const_cast<char**>(arguments));
}