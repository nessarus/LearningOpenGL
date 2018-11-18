#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

/* Lecture: Using Modern OpenGL in C++ */

int main3(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	// ----------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------
	// Lesson: Check if glewInt() work?
	// ----------------------------------------------------------------------------------
/*
	glewInit();
*/
	// ----------------------------------------------------------------------------------
	// Nope, it doesn't. Reading the documentation we need to include
	// the definition GLEW_STATIC in the C/C++ preprocessor definitions properties.
	//
	// ----------------------------------------------------------------------------------
	// Lesson: does glewInit() return an error?
	// ----------------------------------------------------------------------------------
/*
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		//Problem: glewInit failed, something is seriously wrong
		std::cout << "Error!" << std::endl;
	}
*/
	// ----------------------------------------------------------------------------------
	// glewInit() still returns an error. Reading documentation, we must call glewInit after
	// creating a valid OpenGL context.
	// ----------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);


	// ----------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------
	// Lesson: Move glewInit() after glfwMakeContextCurrent()
	// ----------------------------------------------------------------------------------
	GLenum err = glewInit();
	if (GLEW_OK != err)
		std::cout << "Error!" << std::endl;
	// ----------------------------------------------------------------------------------
	// Now our GLEW is up and running and we can now use all modern OpenGL functions.

	// ----------------------------------------------------------------------------------
	// Lesson: Check OpenGL version
	// ----------------------------------------------------------------------------------
	std::cout << glGetString(GL_VERSION) << std::endl;
	// ----------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT); //The glClear function clears colour buffer to preset values.

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}