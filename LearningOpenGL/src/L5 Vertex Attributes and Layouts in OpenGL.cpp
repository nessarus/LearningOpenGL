#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

/* Lecture: Vertex Attributes and Layouts in OpenGL */

int main5(void)
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

	// Storing points in the vertex buffer
	unsigned int buffer;
	glGenBuffers(1, &buffer);

	// Selecting a buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// Geometric data of a triangle
	float positions[6] = {
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f
	};

	// Setting the size and adding data to the buffer.
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(positions[0]), positions, GL_STATIC_DRAW); 
	//----------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------
	// Lesson: declaring the type of data layout in the bound buffer.
	//----------------------------------------------------------------------------------
	// glVertexAttribPointer(index,size,type,normalize,stride,pointer):
	// - index: selects which attribute to change. eg {position, texture cood, normal} 
	//			position would be index 0, texture would be index 1, normal is index 2.
	// - size: how many dimensions or elements in cell for each vertex
	// - type: data type e.g. GL_INT, GL_FLOAT, etc...
	// - normalize: converting byte values like color int: 0-255 to a float: 0.00-1.00
	//				 Its probably better to do it on the cpu not gpu (i.e. pick GL_FALSE)
	// - stride: the number of bytes from one vertex to the next. (default: 0, vertex are 
	//			 tightly packed
	// - pointer: Must be set to the start of the attribute in the vertex. There will 
	//			  be a function to calculate that position in the data structure.
	//			  (offsetof function)
	//			  Memory location must be in cast to a (const void*)
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);
	// We have only one attribute so we only have to use glVertexAttribPointer once.
	// Declaring the type must occur after the buffer is selected

	//----------------------------------------------------------------------------------
	// Lesson: enabling the vertex attribute array
	//----------------------------------------------------------------------------------
	glEnableVertexAttribArray(0);
	// For some reason the vertex attribute array are not enabled by default.
	// Enabling the array can only be done after the buffer is bound.
	//----------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT); //The glClear function clears colour buffer to preset values.


		//----------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------
		// Previous:
		//----------------------------------------------------------------------------------
		// Simple non-standard draw method to quickly draw our triangle again
		glDrawArrays(GL_TRIANGLES, 0, 3);
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