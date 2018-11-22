#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

/* Lecture: Textures in OpenGL */

int main(void)
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
		//----------------------------------------------------------------------------------
		// Lesson: Adding texture coordinates 
		//----------------------------------------------------------------------------------
		// Geometric data of a square
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
		//----------------------------------------------------------------------------------

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
		// Lesson: Setting up textures
		//----------------------------------------------------------------------------------
		// Setting up textures
		Texture texture("res/textures/Nessarus4.png");

		// Bind to texture slot
		texture.Bind();
		// bind() defaults to 0.

		// Sets the texture slot
		shader.SetUniform1i("u_Texture", 0);
		// If you texture.Bind(2) then shader.SetUniform1i("texture", 2)
		//----------------------------------------------------------------------------------

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