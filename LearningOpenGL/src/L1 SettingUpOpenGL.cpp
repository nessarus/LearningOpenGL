#include <GLFW/glfw3.h>

/* Lecture: Setting up OpenGL and Creating a Window in C++ */

int main1(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT); //The glClear function clears colour buffer to preset values.


		//----------------------------------------------------------------------------------
		/* Creating a simple triangle */
		glBegin(GL_TRIANGLES);		//Begins triangle mode 
		glVertex2f(-0.5f, -0.5f);	//First point (vertex, 2 dimensions, floats) in triangle
		glVertex2f( 0.0f,  0.5f);	//Second point in triangle
		glVertex2f( 0.5f, -0.5f);	//Third point in triangle
		glEnd();					//Ends triangle mode

		//----------------------------------------------------------------------------------


		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}