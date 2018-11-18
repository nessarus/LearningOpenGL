#include <iostream>
#include "GLFW/glfw3.h"

int main2()
{
	int a = glfwInit();
	std::cout << a << std::endl;

	std::cin.get();
	return 0;
}