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

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

/* Lecture: ImGui in OpenGL */

int main18(void)
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
			100.0f, 100.0f, 0.0f, 0.0f,
			200.0f, 100.0f, 1.0f, 0.0f,
			200.0f, 200.0f, 1.0f, 1.0f,
			100.0f, 200.0f, 0.0f, 1.0f,
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

		// Creating a orthographic matrix
		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);

		// translation matrix to position of all objects right (moving camera left)
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));

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

		//----------------------------------------------------------------------------------
		// Lesson: Using ImGui
		//----------------------------------------------------------------------------------
		// Setting up UI by setting up ImGui context
		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);

		// Setup UI style
		ImGui::StyleColorsDark();

		// Setup UI window variables
		//bool show_demo_window = true;
		//bool show_another_window = false;
		//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		// setting up varable to move icon
		glm::vec3 translation(200, 200, 0);

		//----------------------------------------------------------------------------------

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

			//----------------------------------------------------------------------------------
			// Lesson: Using ImGui
			//----------------------------------------------------------------------------------
			// Sets up new ImGui frame (setup before any ImGui code for this frame)
			ImGui_ImplGlfwGL3_NewFrame();

			// the model matrix
			glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);

			// multiplying a translation matrices together to get final image
			glm::mat4 mvp = proj * view * model;

			// Rebinding buffers
			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

			// Setting uniform to pass in projection into vertex shader
			shader.SetUniformMat4f("u_MVP", mvp);
			//----------------------------------------------------------------------------------

			// Drawing primitives using the vertex array, index buffer and shader
			renderer.Draw(va, ib, shader);

			// Setting new values into uniform variable per draw
			if (r > 1.0f)
				increment = -0.05f;
			else if (r < 0.0f)
				increment = 0.05f;
			r += increment;

			//----------------------------------------------------------------------------------
			// Lesson: Using ImGui
			//----------------------------------------------------------------------------------
			// Creating UI window
			{
				//static float f = 0.0f;
				//static int counter = 0;
				//ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
				ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
				//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

				//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
				//ImGui::Checkbox("Another Window", &show_another_window);

				//if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
				//	counter++;
				//ImGui::SameLine();
				//ImGui::Text("counter = %d", counter);

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}

			// Draw ImGui into frame buffer
			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

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

	//----------------------------------------------------------------------------------
	// Lesson: Cleaning up ImGui
	//----------------------------------------------------------------------------------
	// Cleaning up ImGui
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	//----------------------------------------------------------------------------------

	glfwTerminate();
	return 0;
}