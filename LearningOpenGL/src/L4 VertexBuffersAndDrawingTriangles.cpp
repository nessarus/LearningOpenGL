#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

/* Lecture: Vertex Buffers and Drawing a Triangle in OpenGL */

int main4(void)
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

	//----------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------
	// Previous: 
	//----------------------------------------------------------------------------------
	// Move glewInit() after creating OpenGL context
	GLenum err = glewInit();
	if (GLEW_OK != err)
		std::cout << "Error!" << std::endl;

	// Check OpenGL version
	std::cout << glGetString(GL_VERSION) << std::endl;
	//----------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------
	// Lesson: Storing points in the vertex buffer
	//----------------------------------------------------------------------------------
	unsigned int buffer;
	glGenBuffers(1, &buffer);	
	//----------------------------------------------------------------------------------
	// glGenBuffers() defines the number of vertex buffers we want. We want one.
	// Then we save the id of the buffer to an unsigned int. 

	//----------------------------------------------------------------------------------
	// Lesson: Selecting a buffer
	//----------------------------------------------------------------------------------
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	//----------------------------------------------------------------------------------
	// glBindBuffer selects the buffer and assigns its target (where in GPU it will be stored)
	// Note: to bind (select) no buffer use the number 0 as the buffer.

	//----------------------------------------------------------------------------------
	// Lesson: Setting the size and adding data to the buffer.
	//----------------------------------------------------------------------------------
	float positions[6] = {
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f
	};

	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(positions[0]),positions, GL_STATIC_DRAW); //sizeof(position[0]) == sizeof(float)
	//----------------------------------------------------------------------------------
	// Check http://docs.gl for documentation for all OpenGL functions. We are using gl4. 
	// See the decription for STATIC, DYNAMIC, STREAM, DRAW, READ, COPY.
	// The usage (i.e. GL_STATIC_DRAW) optimises the GPU for that data.
	//----------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT); //The glClear function clears colour buffer to preset values.


		//----------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------
		// Lesson: Simple non-standard draw method to quickly draw our triangle again
		//----------------------------------------------------------------------------------
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//----------------------------------------------------------------------------------
		// We don't have an index buffer, so we are using glDrawArrays to quickly see our triangle.
		// See documentation for how to use.
		// Indices means vertex or the corners of the triangle.
		// Note: looks like we still get a black screen if we try to run it as we don't have the rest of the code done.
		//----------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------


		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}