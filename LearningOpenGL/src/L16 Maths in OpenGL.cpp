#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"


/* Lecture: Maths in OpenGL */

int main16(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	//----------------------------------------------------------------------------------
	// Previous:
	//----------------------------------------------------------------------------------
	// OpenGL Core profile and version (demonstration)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//----------------------------------------------------------------------------------

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
	// Previous: 
	//----------------------------------------------------------------------------------
	// slow down the frame rate so we can see the changes
	glfwSwapInterval(2);

	// Move glewInit() after creating OpenGL context
	GLenum err = glewInit();
	if (GLEW_OK != err)
		std::cout << "Error!" << std::endl;

	// Check OpenGL version
	std::cout << glGetString(GL_VERSION) << std::endl;
	{
		// Geometric data of a square with texture coordinates
		float positions[] = {
			-0.5f, -0.5f, 0.0f, 0.0f,
			 0.5f, -0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f
		};

		// Setup the vertex buffer layouts
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);

		// Enable blending of alpha (layers of transparency in textures)
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));

		// Index buffer indicates which positions each vertex has for each triangle
		unsigned int indices[] = {
			0, 1, 2,		// triangle 1
			2, 3, 0			// triangle 2
		};

		// Setting up Vertex Buffer and Index Buffer
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));
		IndexBuffer ib(indices, 6);

		// Setup the vertex array
		VertexArray va;

		// Adding vertex buffer and layout together to the vertex array
		va.AddBuffer(vb, layout);

		// Setting up shaders
		Shader shader("res/shaders/Basic.shader");
		shader.Bind();

		// Set data (color: pink) into new variable "u_Color"
		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

		//----------------------------------------------------------------------------------
		// Lesson: Mapping objects on a 2D plane 
		//----------------------------------------------------------------------------------
		// Creating a orthographic matrix
		glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
		// An orthographic view means objects don't get smaller the further away they are.
		// This opposed to a perspective view where objects scaled according to a camera.

		//glm::mat4 proj = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, -1.0f, 1.0f);
		// Lets try with making it twice as big -> image becomes smaller

		// Setting uniform to pass in projection into vertex shader
		shader.SetUniformMat4f("u_MVP", proj);
		// We need to create a new function in shader: SetUniformMat4f (see shader.cpp)
		//----------------------------------------------------------------------------------


		// Setting up textures
		Texture texture("res/textures/Nu Final.png");

		// Bind to texture slot
		texture.Bind();

		// Sets the texture slot
		shader.SetUniform1i("u_Texture", 0);
		
		// Unbinding all buffers
		va.Unbind();
		shader.Unbind();
		vb.Unbind();
		ib.Unbind();

		// Setting up Renderer
		Renderer renderer;

		// new data for uniform variable
		float r = 0.0f;
		float increment = 0.05f;

		//----------------------------------------------------------------------------------

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			//----------------------------------------------------------------------------------
			// Previous
			//----------------------------------------------------------------------------------
			// glClear(GL_COLOR_BUFFER_BIT); 
			renderer.Clear();

			// Rebinding buffers
			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

			// Drawing primitives using the vertex array, index buffer and shader
			renderer.Draw(va, ib, shader);

			// Setting new values into uniform variable per draw
			if (r > 1.0f)
				increment = -0.05f;
			else if (r < 0.0f)
				increment = 0.05f;
			r += increment;
			//----------------------------------------------------------------------------------


			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

		//----------------------------------------------------------------------------------
		// Previous: 
		//----------------------------------------------------------------------------------
	}
	// glfwTerminate deletes OpenGl context so need scope to destroy shader before it.
	//----------------------------------------------------------------------------------
	glfwTerminate();
	return 0;
}